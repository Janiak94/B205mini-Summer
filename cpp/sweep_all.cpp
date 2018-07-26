#include <uhd/types/tune_request.hpp>
#include <uhd/utils/thread.hpp>
#include <uhd/utils/safe_main.hpp>
#include <uhd/utils/static.hpp>
#include <uhd/usrp/multi_usrp.hpp>
#include <uhd/exception.hpp>
#include <boost/thread/thread.hpp>
#include <boost/program_options.hpp>
#include <boost/math/special_functions/round.hpp>
#include <boost/format.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/filesystem.hpp>
#include <iostream>
#include <fstream>
#include <csignal>
#include <thread>

static bool stop_signal_called = false;

static const char *default_filepath = "../../../../../../Volumes/JENS_USB1/sweep_data/";

void transmit_thread(uhd::usrp::multi_usrp::sptr usrp){

    std::ifstream in_file("test_signal.bin", std::ifstream::binary);
    assert(in_file.is_open());
    uhd::tx_streamer::sptr tx_stream = usrp->get_tx_stream(uhd::stream_args_t("fc32", "sc16"));
    std::vector<std::complex<float> > buff(tx_stream->get_max_num_samps());
    float timeout = 1.1;
    uhd::tx_metadata_t md;
    md.time_spec = usrp->get_time_now() + uhd::time_spec_t(0.01);
    md.has_time_spec = true;
    md.start_of_burst = true;
    md.end_of_burst = false;

    while(not stop_signal_called){
        while(not stop_signal_called and not in_file.eof()){
            in_file.read((char *) &buff.front(), buff.size() * sizeof(std::complex<float>));
            size_t num_tx_samps(in_file.gcount() / sizeof(std::complex<float>));
            tx_stream->send(&buff.front(), num_tx_samps, md, timeout);
            md.start_of_burst = false;
            md.has_time_spec = false;
        }
        in_file.clear();
        in_file.seekg(0, std::ifstream::beg);
    }
    md.end_of_burst = true;
    tx_stream->send("", 0, md);
};

void recieve_thread(uhd::usrp::multi_usrp::sptr usrp, std::ofstream &out_file, std::ofstream &usrp_datafile){
    uhd::rx_streamer::sptr rx_stream = usrp->get_rx_stream(uhd::stream_args_t("fc32","sc16"));
    uhd::rx_metadata_t md;
    md.time_spec = usrp->get_time_now() + uhd::time_spec_t(0.05);
    md.has_time_spec = true;
    md.start_of_burst = true;
    md.end_of_burst = false;
    uhd::stream_cmd_t stream_cmd(uhd::stream_cmd_t::STREAM_MODE_NUM_SAMPS_AND_DONE);
    size_t total_num_samps = 2e4;
    stream_cmd.num_samps = total_num_samps;
    stream_cmd.time_spec = md.time_spec;
    stream_cmd.stream_now = false;
    rx_stream->issue_stream_cmd(stream_cmd);
    std::vector<std::complex<float> > buff(2000);

    bool record_sensors = true;
    while(not stop_signal_called){
        size_t num_rx_samps = rx_stream->recv(&buff.front(), buff.size(), md, 0.1f);
        md.start_of_burst = false;
        md.has_time_spec = false;
        out_file.write((const char *) &buff.front(), buff.size() * sizeof(std::complex<float>));
        stop_signal_called = md.end_of_burst;
        if(record_sensors) {
            usrp_datafile << boost::format("%.1f\t%.3f\t%.3f\t%.2f\t%.6f\n") % (usrp->get_rx_freq()/ 1e6) %
                             (usrp->get_tx_sensor("temp").to_real()) % (usrp->get_rx_sensor("temp").to_real())
                             % (usrp->get_rx_sensor("rssi").to_real()) % (usrp->get_time_now().get_real_secs());
        }
        record_sensors = false;
    }
    md.end_of_burst = true;
    rx_stream->recv(&buff.front(),0, md);
};

bool lo_lock_t(uhd::usrp::multi_usrp::sptr usrp){
    bool lo_lock;
    std::vector<std::string> sensor_names;
    sensor_names = usrp->get_tx_sensor_names(0);
    if (std::find(sensor_names.begin(), sensor_names.end(), "lo_locked") != sensor_names.end()) {
        uhd::sensor_value_t lo_locked = usrp->get_tx_sensor("lo_locked",0);
        lo_lock = lo_locked.to_bool();
    }

    sensor_names = usrp->get_rx_sensor_names(0);
    if (std::find(sensor_names.begin(), sensor_names.end(), "lo_locked") != sensor_names.end()) {
        uhd::sensor_value_t lo_locked = usrp->get_rx_sensor("lo_locked",0);
        lo_lock = lo_lock and lo_locked.to_bool();
    }

    return lo_lock;
}

//bool allowed_gain(float attenuation, float tx_gain, float rx_gain);

namespace po = boost::program_options;

int UHD_SAFE_MAIN(int argc, char *argv[]){
    uhd::set_thread_priority_safe();

    int number;
    float attenuation;
    //transmit & receive variables set by po
    double tx_gain, tx_bw;
    double rx_gain, rx_bw;
    double samp_rate;
    std::string file;

    //setup the program options
    po::options_description desc("Allowed options");
    desc.add_options()
            ("help", "help message")
            ("tx-bw", po::value<double>(&tx_bw), "analog transmit filter bandwidth in Hz")
            ("rx-bw", po::value<double>(&rx_bw), "analog recieve filter bandwidth in Hz")
            //("number", po::value<int>(&number)->default_value(0), "Number of the run")
            ("attenuation", po::value<float>(&attenuation)->default_value(0), "External attenuation")
            ;
    po::variables_map variables_map;
    po::store(po::parse_command_line(argc, argv, desc), variables_map);
    po::notify(variables_map);

    //print the help message
    if (variables_map.count("help")){
        std::cout << boost::format("Sweep %s") % desc << std::endl;
        exit(~0);
    }

    samp_rate = 3.5e6;

    //create main directory
    std::string main_path = (boost::format("%s%.0f") % default_filepath % attenuation).str();
    boost::filesystem::create_directory(main_path);

    uhd::usrp::multi_usrp::sptr usrp = uhd::usrp::multi_usrp::make(uhd::device_addr_t());

    usrp->set_rx_dc_offset(false);

    usrp->set_tx_rate(samp_rate);
    usrp->set_rx_rate(samp_rate);
    std::cout << boost::format("TX/RX Sample rate set to %f MSPS") % (usrp->get_tx_rate()/1e6) << std::endl;

    if(variables_map.count("tx-bw")){
        usrp->set_tx_bandwidth(tx_bw);
    }

    if(variables_map.count("rx-bw")){
        usrp->set_rx_bandwidth(rx_bw);
    }


    // Set max-value of allowed tx-gain
    float max_tx_gain = 1;

    if(attenuation < 30){
        max_tx_gain -= 0.2;
        if(attenuation < 12.1){
            max_tx_gain -= 0.2;
        }
    }

    //TODO behövs kanske funktion som kollar om mapparna redan finns och om det gör, skapar ingen ny utan använder denna

    for(size_t number = 0; number < 50; number++) {
        std::string number_path = main_path + "/" + (boost::format("%i") % number).str();
        boost::filesystem::create_directory(number_path);
        for (float tx_gain = 0; tx_gain <= max_tx_gain; tx_gain += 0.2) {
            std::string tx_path = number_path + "/" + (boost::format("%.1f") % (tx_gain)).str();
            boost::filesystem::create_directory(tx_path);
            usrp->set_normalized_tx_gain(tx_gain);

            for (float rx_gain = 0; rx_gain <= 1; rx_gain += 0.2) {
                std::string rx_path = tx_path + "/" + (boost::format("%.1f") % (rx_gain)).str();
                boost::filesystem::create_directory(rx_path);
                rx_path += "/";

                usrp->set_normalized_rx_gain(rx_gain);

                // save metadata
                std::ofstream usrp_datafile(rx_path + "data.dat", std::ofstream::out);
                usrp_datafile << boost::format("TX-samp\tRX-samp\tTX-bw\tRX-bw\tTX-gain\tRX-gain\n");
                usrp_datafile << boost::format("%.1f\t%.1f\t%.1f\t%.1f\t%.2f\t%.2f\n\n") % (usrp->get_tx_rate()/1e6) %
                        (usrp->get_rx_rate()/1e6) % (usrp->get_tx_bandwidth()/1e6) % (usrp->get_rx_bandwidth()/1e6) %
                        (usrp->get_tx_gain()) % (usrp->get_rx_gain());
                usrp_datafile << boost::format("Freq\tTX-temp\tRX-temp\tRSSI\tUsrp-time\n");

                for (double freq = 0.5e9; freq <= 3e9; freq += 0.1e9) {

                    stop_signal_called = false;
                    std::ofstream out_file(
                            rx_path + (boost::format("%.1f") % (freq / 1e9)).str(),
                            std::ofstream::binary
                    );
                    UHD_ASSERT_THROW(out_file.is_open());

                    usrp->set_rx_freq(uhd::tune_request_t(freq,0));
                    usrp->set_tx_freq(uhd::tune_request_t(freq,0));

                    UHD_ASSERT_THROW(lo_lock_t(usrp));

                    std::cout << "\r" << std::flush <<
                              boost::format("TX: %.1f RX: %.1f freq: %.1f run: %i ")
                              % tx_gain % rx_gain % ((usrp->get_rx_freq()+usrp->get_tx_freq())/(2*1e6)) % number;

                    boost::thread_group thread_group;
                    thread_group.create_thread(boost::bind(&transmit_thread, usrp));
                    thread_group.create_thread(boost::bind(&recieve_thread, usrp, std::ref(out_file), std::ref(usrp_datafile)));
                    thread_group.join_all();

                    out_file.close();
                }
                usrp_datafile.close();
            }
        }
    }
    std::cout << std::endl << "Done!" << std::endl << std::endl;
    return EXIT_SUCCESS;
}