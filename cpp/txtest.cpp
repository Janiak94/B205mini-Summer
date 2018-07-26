//
// Created by Jens Nilsson on 2018-06-29.
//
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
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>
#include <algorithm>    // std::fill
#include <vector>       // std::vector
#include <boost/filesystem.hpp>
#include <iostream>
#include <fstream>
#include <thread>
#include <csignal>
#include <boost/assign/list_of.hpp>
#include <math.h>

namespace po = boost::program_options;

// control variables, mutex, cond and last two bool to get output in correct order
static bool stop_signal_called = false;
static boost::condition_variable cond_send, cond_recv;
static boost::mutex mut_send, mut_recv;
static bool is_sending = false;
static bool is_recieving = false;

// mean amplitude of "constant" signal
static float current_mean = 0;

void signal_handler(int){ stop_signal_called = true; }

void transmit_worker(uhd::usrp::multi_usrp::sptr usrp, std::ifstream &in_file){
    uhd::tx_streamer::sptr tx_stream = usrp->get_tx_stream(uhd::stream_args_t("fc32", "sc16"));
    std::vector<std::complex<float> > buff(tx_stream->get_max_num_samps());
    float timeout = 1.1;
    uhd::tx_metadata_t md;
    md.time_spec = usrp->get_time_now() + uhd::time_spec_t(0.01);
    md.has_time_spec = true;
    md.start_of_burst = true;
    md.end_of_burst = false;

    std::cout << "Starting transmission" << std::endl;
    while(not stop_signal_called){
        while(not stop_signal_called and not in_file.eof()){
            in_file.read((char *) &buff.front(), buff.size() * sizeof(std::complex<float>));
            size_t num_tx_samps(in_file.gcount() / sizeof(std::complex<float>));
            tx_stream->send(&buff.front(), num_tx_samps, md, timeout);
            md.start_of_burst = false;
            md.has_time_spec = false;
            if(not is_sending){
                // notify recv thread after first package
                boost::lock_guard<boost::mutex> lock(mut_send);
                is_sending = true;
                cond_send.notify_all();
            }
        }
        in_file.clear();
        in_file.seekg(0, std::ifstream::beg);
    }
    md.end_of_burst = true;
    tx_stream->send("", 0, md);
}

void temperature_thread(uhd::usrp::multi_usrp::sptr usrp, std::ofstream &out_file){
    std::vector<std::string> tx_sensors = usrp->get_tx_sensor_names();
    std::vector<std::string> rx_sensors = usrp->get_rx_sensor_names();
    UHD_ASSERT_THROW(std::find(tx_sensors.begin(), tx_sensors.end(),"temp") != tx_sensors.end());
    UHD_ASSERT_THROW(std::find(rx_sensors.begin(), rx_sensors.end(),"temp") != rx_sensors.end());
    UHD_ASSERT_THROW(std::find(rx_sensors.begin(), rx_sensors.end(),"rssi") != rx_sensors.end());

    /*out_file << boost::format("%s \t %s \t %s \t %s \t %s\n") % ("TX-Temp")
                % ("RX-Temp") % ("RX-RSSI") % ("Mean-amp")% ("USRP-Time")*/
    out_file << boost::format("%s \t %s\n") % ("TX-Temp")
                % ("USRP-Time");

    //change back to recieve
    // wait for recv thread to notify that first package has been sent
    boost::unique_lock<boost::mutex> lock(mut_send);
    while(not is_sending){
        cond_send.wait(lock);
    }

    /*std::cout << boost::format("%s \t %s \t %s \t %s \t %s\n") % ("TX-Temp")
                 % ("RX-Temp") % ("RX-RSSI") % ("Mean-amp")% ("USRP-Time");*/
    std::cout << boost::format("%s \t %s\n") % ("TX-Temp")
                 % ("USRP-Time");

    float tx_temp, rx_temp, rssi , usrp_time;
    while(not stop_signal_called) {
        tx_temp = usrp->get_tx_sensor("temp").to_real();
        //rx_temp = usrp->get_rx_sensor("temp").to_real();
        //rssi = usrp->get_rx_sensor("rssi").to_real();
        usrp_time = usrp->get_time_now().get_real_secs();
        /*out_file << boost::format("%f\t%f\t%f\t%f\t%f\n") % tx_temp % rx_temp %
                rssi % current_mean % usrp_time;*/
        out_file << boost::format("%f\t%f\n") % tx_temp %
                     usrp_time;

        /*std::cout << boost::format("\r%f\t%f\t%f\t%f\t%f") % tx_temp % rx_temp %
                rssi % current_mean % usrp_time << std::flush;*/
        std::cout << boost::format("\r%f\t%f") % tx_temp %
                      usrp_time << std::flush;
        boost::this_thread::sleep_for(boost::chrono::milliseconds(100));
    }
    out_file << "\n";
}

void recieve_worker(uhd::usrp::multi_usrp::sptr usrp){
    usrp->set_rx_bandwidth(0.2e6);
    uhd::rx_streamer::sptr rx_stream = usrp->get_rx_stream(uhd::stream_args_t("fc32","sc16"));
    uhd::rx_metadata_t md;
    md.time_spec = usrp->get_time_now() + uhd::time_spec_t(0.1);
    md.has_time_spec = true;
    md.start_of_burst = true;
    md.end_of_burst = false;
    uhd::stream_cmd_t stream_cmd(uhd::stream_cmd_t::STREAM_MODE_START_CONTINUOUS);
    stream_cmd.time_spec = md.time_spec + uhd::time_spec_t(0.2);
    stream_cmd.stream_now = false;
    rx_stream->issue_stream_cmd(stream_cmd);
    std::vector<std::complex<float> > buff(rx_stream->get_max_num_samps());

    std::ofstream out_file;
    out_file.open("minimum_signal.bin", std::ofstream::binary);

    // wait for transmit thread to notify that the first package has been sent
    //boost::unique_lock<boost::mutex> lock(mut_send);
    /*while(not is_sending){
        cond_send.wait(lock);
    }*/
    std::cout << std::endl <<std::endl;
    while(not stop_signal_called){
        size_t num_rx_samps = rx_stream->recv(&buff.front(), buff.size(), md, 0.1f);
        md.start_of_burst = false;
        md.has_time_spec = false;
        float mean = 0;
        for(std::vector<std::complex<float> >::const_iterator i = buff.cbegin(); i < buff.cend(); i++){
            mean += std::abs(*i);
        }
        current_mean = mean / buff.size();
        out_file.write((const char *) &buff.front(), buff.size() * sizeof(std::complex<float>));
        if(not is_recieving){
            // notify output thread that the first package has been recieved
            boost::lock_guard<boost::mutex> lock(mut_recv);
            is_recieving = true;
            cond_recv.notify_one();
        }
    }
    md.end_of_burst = true;
    rx_stream->recv(&buff.front(),0, md);
    stream_cmd.stream_mode = uhd::stream_cmd_t::STREAM_MODE_STOP_CONTINUOUS;
    rx_stream->issue_stream_cmd(stream_cmd);
    out_file.close();
}

void control_thread(void){
    // just wait for any input and shut down program
    std::string s;
    //std::cin.getline(&s.front(), 100);
    std::this_thread::sleep_for(std::chrono::milliseconds(1200));
    signal_handler(1);
}

int UHD_SAFE_MAIN(int argc, char *argv[]){

    //transmit & receive variables set by po
    double tx_gain, tx_bw;
    double rx_gain, rx_bw;
    double samp_rate, freq;
    std::string file;

    //setup the program options
    po::options_description desc("Allowed options");
    desc.add_options()
            ("help", "help message")
            ("samp-rate", po::value<double>(&samp_rate), "rate of transmit outgoing samples")
            ("freq", po::value<double>(&freq), "receive RF center frequency in Hz")
            ("tx-gain", po::value<double>(&tx_gain), "gain for the transmit RF chain")
            ("tx-bw", po::value<double>(&tx_bw), "analog transmit filter bandwidth in Hz")
            ("file", po::value<std::string>(&file)->default_value("constant.bin"), "name of signal file")
            ;
    po::variables_map variables_map;
    po::store(po::parse_command_line(argc, argv, desc), variables_map);
    po::notify(variables_map);

    //print the help message
    if (variables_map.count("help")){
        std::cout << boost::format("TX-test %s") % desc << std::endl;
        exit(~0);
    }

    if (not variables_map.count("freq")){
        std::cout << "Please specify center frequency" << std::endl;
        exit(~0);
    }
    if (not variables_map.count("samp-rate")){
        std::cout << "Please specify sample rate" << std::endl;
        exit(~0);
    }

    uhd::usrp::multi_usrp::sptr usrp = uhd::usrp::multi_usrp::make(uhd::device_addr_t());

    usrp->set_rx_dc_offset(false);

    usrp->set_tx_rate(samp_rate);
    usrp->set_rx_rate(samp_rate);
    std::cout << boost::format("TX/RX Sample rate set to %f MSPS") % (usrp->get_tx_rate()/1e6) << std::endl;

    usrp->set_tx_freq(uhd::tune_request_t(freq,0));
    usrp->set_rx_freq(uhd::tune_request_t(freq,0));
    std::cout << boost::format("TX/RX Centerfrequency set to %f MHz") % (usrp->get_tx_freq()/1e6) << std::endl;

    if(variables_map.count("tx-gain")){
        usrp->set_tx_gain(tx_gain);
    }
    std::cout << boost::format("TX Gain: %f dB") % usrp->get_tx_gain() << std::endl;

    if(variables_map.count("tx-bw")){
        usrp->set_tx_bandwidth(tx_bw);
    }
    std::cout << boost::format("TX BW set to %f MHz") % (usrp->get_tx_bandwidth()/1e6) << std::endl;

    usrp->set_time_now(uhd::time_spec_t(0.0));

    std::vector<std::string> sensor_names;
    sensor_names = usrp->get_tx_sensor_names(0);
    if (std::find(sensor_names.begin(), sensor_names.end(), "lo_locked") != sensor_names.end()) {
        uhd::sensor_value_t lo_locked = usrp->get_tx_sensor("lo_locked",0);
        std::cout << boost::format("Checking lock: %s ...") % lo_locked.to_pp_string() << std::endl;
        UHD_ASSERT_THROW(lo_locked.to_bool());
    }

    sensor_names = usrp->get_rx_sensor_names(0);
    if (std::find(sensor_names.begin(), sensor_names.end(), "lo_locked") != sensor_names.end()) {
        uhd::sensor_value_t lo_locked = usrp->get_rx_sensor("lo_locked",0);
        std::cout << boost::format("Checking lock: %s ...") % lo_locked.to_pp_string() << std::endl;
        UHD_ASSERT_THROW(lo_locked.to_bool());
    }

    std::ofstream temp_file("temperature/temperature.dat", std::ofstream::out | std::ofstream::app);
    std::ifstream in_file;

    in_file.open(file, std::ifstream::binary);
    assert(in_file.is_open());

    boost::thread_group thread_group;
    thread_group.create_thread(boost::bind(&transmit_worker, usrp, std::ref(in_file)));
    thread_group.create_thread(boost::bind(&recieve_worker, usrp));
    thread_group.create_thread(boost::bind(&temperature_thread, usrp, std::ref(temp_file)));
    control_thread();
    thread_group.join_all();

    temp_file.close();
    in_file.close();
    std::cout << "Done!\a" << std::endl;
    return EXIT_SUCCESS;
}