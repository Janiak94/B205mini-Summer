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
#include <chrono>

namespace po = boost::program_options;

// control variables, mutex, cond and last two bool to get output in correct order
static bool stop_signal_called = false;

// mean amplitude of "constant" signal
static float current_mean = 0;

void signal_handler(int){ stop_signal_called = true; }

void print_data(uhd::usrp::multi_usrp::sptr usrp) {
    std::vector<std::string> tx_sensors = usrp->get_tx_sensor_names();
    std::vector<std::string> rx_sensors = usrp->get_rx_sensor_names();
    UHD_ASSERT_THROW(std::find(tx_sensors.begin(), tx_sensors.end(), "temp") != tx_sensors.end());
    UHD_ASSERT_THROW(std::find(rx_sensors.begin(), rx_sensors.end(), "temp") != rx_sensors.end());
    UHD_ASSERT_THROW(std::find(rx_sensors.begin(), rx_sensors.end(), "rssi") != rx_sensors.end());

    std::cout << boost::format("%s \t %s \t %s \t %s \t %s\n") % ("TX-Temp")
                 % ("RX-Temp") % ("RX-RSSI") % ("Mean-amp") % ("USRP-Time");

    float tx_temp, rx_temp, rssi , usrp_time;
    while(not stop_signal_called) {
        tx_temp = usrp->get_tx_sensor("temp").to_real();
        rx_temp = usrp->get_rx_sensor("temp").to_real();
        rssi = usrp->get_rx_sensor("rssi").to_real();
        usrp_time = usrp->get_time_now().get_real_secs();

        std::cout << boost::format("\r%f\t%f\t%f\t%f\t%f") % tx_temp % rx_temp %
                     rssi % current_mean % usrp_time << std::flush;
        boost::this_thread::sleep_for(boost::chrono::milliseconds(100));
    }
}

void recieve_worker(uhd::usrp::multi_usrp::sptr usrp, std::ofstream &out_file){
    uhd::rx_streamer::sptr rx_stream = usrp->get_rx_stream(uhd::stream_args_t("fc32","sc16"));
    uhd::rx_metadata_t md;
    md.time_spec = usrp->get_time_now() + uhd::time_spec_t(0.1);
    md.has_time_spec = true;
    md.start_of_burst = true;
    md.end_of_burst = false;
    uhd::stream_cmd_t stream_cmd(uhd::stream_cmd_t::STREAM_MODE_START_CONTINUOUS);
    stream_cmd.time_spec = md.time_spec + uhd::time_spec_t(0.1);
    stream_cmd.stream_now = false;
    rx_stream->issue_stream_cmd(stream_cmd);
    std::vector<std::complex<float> > buff(rx_stream->get_max_num_samps());

    while(not stop_signal_called){
        size_t num_rx_samps = rx_stream->recv(&buff.front(), buff.size(), md, 0.1f);
        md.start_of_burst = false;
        md.has_time_spec = false;
        float mean = 0;
        for(std::vector<std::complex<float> >::const_iterator i = buff.cbegin(); i < buff.cend(); i++){
            mean += std::abs(*i);
        }
        current_mean = mean;
        out_file.write((const char *) &buff.front(), buff.size() * sizeof(std::complex<float>));
    }
    md.end_of_burst = true;
    rx_stream->recv(&buff.front(),0, md);
    stream_cmd.stream_mode = uhd::stream_cmd_t::STREAM_MODE_STOP_CONTINUOUS;
    rx_stream->issue_stream_cmd(stream_cmd);
}

void control_thread(void){
    // just wait for any input and shut down program
    std::string s;
    std::cin.getline(&s.front(), 100);
    signal_handler(1);
}

typedef std::function<uhd::sensor_value_t(const std::string&)> get_sensor_fn_t;

bool lock_sensor(const std::string sensor_name,
                 std::vector<std::string> sensor_names,
                 get_sensor_fn_t get_sensor_fn
){
    float setup_time = 1.0;
    if (std::find(sensor_names.begin(), sensor_names.end(), sensor_name) == sensor_names.end())
        return false;

    auto setup_timeout =
            std::chrono::steady_clock::now()
            + std::chrono::milliseconds(int64_t(setup_time * 1000));
    bool lock_detected = false;

    std::cout << boost::format("Waiting for \"%s\": ") % sensor_name;
    std::cout.flush();

    while (true) {
        if (lock_detected and
            (std::chrono::steady_clock::now() > setup_timeout)) {
            std::cout << " locked." << std::endl;
            break;
        }
        if (get_sensor_fn(sensor_name).to_bool()) {
            std::cout << "+";
            std::cout.flush();
            lock_detected = true;
        }
        else {
            if (std::chrono::steady_clock::now() > setup_timeout) {
                std::cout << std::endl;
                throw std::runtime_error(str(
                        boost::format("timed out waiting for consecutive locks on sensor \"%s\"")
                        % sensor_name
                ));
            }
            std::cout << "_";
            std::cout.flush();
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    std::cout << std::endl;
    return true;
}

int UHD_SAFE_MAIN(int argc, char *argv[]){

    //transmit & receive variables set by po
    double rx_gain, rx_bw;
    double samp_rate, freq;
    std::string file, ref;

    //setup the program options
    po::options_description desc("Allowed options");
    desc.add_options()
            ("help", "help message")
            ("samp-rate", po::value<double>(&samp_rate)->default_value(3.5e6), "rate of transmit outgoing samples")
            ("freq", po::value<double>(&freq)->default_value(1e9), "receive RF center frequency in Hz")
            ("rx-gain", po::value<double>(&rx_gain)->default_value(10), "gain for the transmit RF chain")
            ("rx-bw", po::value<double>(&rx_bw), "analog recieve filter bandwidth in Hz")
            ("file", po::value<std::string>(&file)->default_value("synctest.bin"), "name of signal file")
            ("ref", po::value<std::string>(&ref)->default_value("external"), "reference source (internal, external)")
            ;
    po::variables_map variables_map;
    po::store(po::parse_command_line(argc, argv, desc), variables_map);
    po::notify(variables_map);

    //print the help message
    if (variables_map.count("help")){
        std::cout << boost::format("RX-test %s") % desc << std::endl;
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
    //if (not variables_map.count("file")){
    //    std::cout << "Please specify output file name" << std::endl;
    //    exit(~0);
    //}

    uhd::usrp::multi_usrp::sptr usrp = uhd::usrp::multi_usrp::make(uhd::device_addr_t());

    //usrp->set_clock_source(ref);
    usrp->set_time_source(ref);

    usrp->set_rx_dc_offset(false);

    usrp->set_rx_rate(samp_rate);
    std::cout << boost::format("RX Sample rate set to %f MSPS") % (usrp->get_rx_rate()/1e6) << std::endl;

    usrp->set_rx_freq(freq);
    std::cout << boost::format("RX Centerfrequency set to %f MHz") % (usrp->get_rx_freq()/1e6) << std::endl;

    if(variables_map.count("rx-gain")){
        usrp->set_rx_gain(rx_gain);
    }
    std::cout << boost::format("RX Gain: %f dB") % usrp->get_rx_gain() << std::endl;

    if(variables_map.count("rx-bw")){
        usrp->set_rx_bandwidth(rx_bw);
    }
    std::cout << boost::format("RX BW set to %f MHz") % (usrp->get_rx_bandwidth()/1e6) << std::endl;

    usrp->set_rx_antenna("RX2");
    std::cout << "RX antenna: " << usrp->get_rx_antenna() << std::endl;

    usrp->set_time_now(uhd::time_spec_t(0.0));

    lock_sensor(
            "lo_locked",
            usrp->get_rx_sensor_names(),
            [usrp](const std::string& sensor_name){
                return usrp->get_rx_sensor(sensor_name);
            }
    );
    if(ref == "external"){
        lock_sensor(
                "ref_locked",
                usrp->get_mboard_sensor_names(),
                [usrp](const std::string& sensor_name){
                    return usrp->get_mboard_sensor(sensor_name);
                }
        );
    }

    std::ofstream out_file(file, std::ofstream::binary);

    boost::thread_group thread_group;
    thread_group.create_thread(boost::bind(&recieve_worker, usrp, std::ref(out_file)));
    thread_group.create_thread(boost::bind(&print_data, usrp));
    control_thread();
    thread_group.join_all();

    out_file.close();

    std::cout << "Done!\a" << std::endl;
    return EXIT_SUCCESS;
}