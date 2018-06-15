//
// Created by Jens Nilsson on 2018-06-14.
//

#include <uhd/types/tune_request.hpp>
#include <uhd/utils/thread_priority.hpp>
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
#include <chrono>
#include <csignal>
#include "util.hpp"
#include <hidapi.h>
#include <stdio.h>
#include <stdlib.h>

namespace po = boost::program_options;

void startup(uhd::usrp::multi_usrp::sptr &usrp_device, int argc, char *argv[]){

    //transmit & receive variables set by po
    double tx_gain, tx_bw;
    double rx_gain, rx_bw;
    double samp_rate, freq;

    //setup the program options
    po::options_description desc("Allowed options");
    desc.add_options()
            ("help", "help message")
            ("samp-rate", po::value<double>(&samp_rate), "rate of transmit outgoing samples")
            ("freq", po::value<double>(&freq), "receive RF center frequency in Hz")
            ("tx-gain", po::value<double>(&tx_gain), "gain for the transmit RF chain")
            ("rx-gain", po::value<double>(&rx_gain), "gain for the receive RF chain")
            ("tx-bw", po::value<double>(&tx_bw), "analog transmit filter bandwidth in Hz")
            ("rx-bw", po::value<double>(&rx_bw), "analog receive filter bandwidth in Hz")
            ;
    po::variables_map variables_map;
    po::store(po::parse_command_line(argc, argv, desc), variables_map);
    po::notify(variables_map);

    //print the help message
    if (variables_map.count("help")){
        std::cout << boost::format("UHD TXRX Loopback to File %s") % desc << std::endl;
        exit(~0);
    }

    //Lock mboard clocks
    usrp_device->set_clock_source("internal");

    // Set intial antenna config to tx/rx.
    usrp_device->set_tx_antenna("TX/RX");
    usrp_device->set_rx_antenna("RX2");

    //Setting tx_samp_rate
    std::cout << boost::format("Setting TX Rate: %f Msps...") % (samp_rate/1e6) << std::endl;
    usrp_device->set_tx_rate(samp_rate);
    std::cout << boost::format("Actual TX Rate: %f Msps...") % (usrp_device->get_tx_rate()/1e6) << std::endl << std::endl;

    //Setting rx_samp_rate
    std::cout << boost::format("Setting RX Rate: %f Msps...") % (samp_rate/1e6) << std::endl;
    usrp_device->set_rx_rate(samp_rate);
    std::cout << boost::format("Actual RX Rate: %f Msps...") % (usrp_device->get_rx_rate()/1e6) << std::endl << std::endl;

    //Check if center frequency is given
    if (not variables_map.count("freq")){
        std::cerr << "Please specify the transmit center frequency with --freq" << std::endl;
        exit(~0);
    }

    //Set center frequency
    //TX
    std::cout << boost::format("Setting TX Freq: %f MHz...") % (freq/1e6) << std::endl;
    uhd::tune_request_t tx_tune_request(freq);
    usrp_device->set_tx_freq(tx_tune_request);
    std::cout << boost::format("Actual TX Freq: %f MHz...") % (usrp_device->get_tx_freq()/1e6) << std::endl << std::endl;
    //RX
    std::cout << boost::format("Setting RX Freq: %f MHz...") % (freq/1e6) << std::endl;
    uhd::tune_request_t rx_tune_request(freq);
    usrp_device->set_rx_freq(rx_tune_request);
    std::cout << boost::format("Actual RX Freq: %f MHz...") % (usrp_device->get_rx_freq()/1e6) << std::endl << std::endl;

    //Set TX-gain
    if (variables_map.count("tx-gain")){
        std::cout << boost::format("Setting TX Gain: %f dB...") % tx_gain << std::endl;
        usrp_device->set_tx_gain(tx_gain);
        std::cout << boost::format("Actual TX Gain: %f dB...") % usrp_device->get_tx_gain() << std::endl << std::endl;
    }

    //Set RX-gain
    if (variables_map.count("rx-gain")){
        std::cout << boost::format("Setting RX Gain: %f dB...") % rx_gain << std::endl;
        usrp_device->set_rx_gain(rx_gain);
        std::cout << boost::format("Actual RX Gain: %f dB...") % usrp_device->get_rx_gain() << std::endl << std::endl;
    }

    //Set TX-bandwidth
    if (not variables_map.count("tx-bw")){
        std::cout << boost::format("TX Bandwidth: %f MHz...") % (usrp_device->get_tx_bandwidth()/1e6) << std::endl << std::endl;
    }else{
        std::cout << boost::format("Setting TX Bandwidth: %f MHz...") % (tx_bw/1e6) << std::endl << std::endl;
        usrp_device->set_tx_bandwidth(tx_bw);
        std::cout << boost::format("Actual TX Bandwidth: %f MHz...") % (usrp_device->get_tx_bandwidth()/1e6) << std::endl << std::endl;
    }

    //Set RX-bandwidth
    if (not variables_map.count("rx-bw")){
        std::cout << boost::format("RX Bandwidth: %f MHz...") % (usrp_device->get_rx_bandwidth()/1e6) << std::endl << std::endl;
    }else{
        std::cout << boost::format("Setting RX Bandwidth: %f MHz...") % (rx_bw/1e6) << std::endl << std::endl;
        usrp_device->set_tx_bandwidth(rx_bw);
        std::cout << boost::format("Actual RX Bandwidth: %f MHz...") % (usrp_device->get_rx_bandwidth()/1e6) << std::endl << std::endl;
    }

    //DC-offset is set to false
    usrp_device->set_rx_dc_offset(false);
    std::cout << "RX DC offset false" << std::endl << std::endl;

    //Check Ref and LO Lock detect
    std::vector<std::string> sensor_names;
    sensor_names = usrp_device->get_tx_sensor_names(0);
    if (std::find(sensor_names.begin(), sensor_names.end(), "lo_locked") != sensor_names.end()) {
        uhd::sensor_value_t lo_locked = usrp_device->get_tx_sensor("lo_locked",0);
        std::cout << boost::format("Checking lock: %s ...") % lo_locked.to_pp_string() << std::endl;
        UHD_ASSERT_THROW(lo_locked.to_bool());
    }

    /*TODO
     * Vill börja sända ut nonsenssignal för att "värma upp" kortet istället för zeropadding
     */

}

void recv_to_file(uhd::usrp::multi_usrp::sptr usrp_device, std::ofstream *outfile_stream){
    // Set number of data points to get from the rx stream at one time
    size_t size_per_buffer = 5000;
    // Vector to hold data from the rx stream in the form of complex numbers (I Q)
    std::vector<std::complex<float> > buffer(size_per_buffer);
    // fc32: complex<float>, s16: Q16 I16
    uhd::stream_args_t stream_args("fc32", "sc16");
    // Ptr to rx stream
    uhd::rx_streamer::sptr rx_stream = usrp_device->get_rx_stream(stream_args);
    // Metadata from rx stream
    uhd::rx_metadata_t rx_metadata;
    uhd::stream_cmd_t stream_cmd(uhd::stream_cmd_t::STREAM_MODE_START_CONTINUOUS);
    rx_stream->issue_stream_cmd(stream_cmd);

    while(not stop_signal_called){
        // Get number of samples written to buffer and fill buffer and set metadata
        size_t num_rx_samps = rx_stream->recv(&buffer.front(), buffer.size(), rx_metadata,1.0f);
        std::cout << "1";
        if (rx_metadata.error_code == uhd::rx_metadata_t::ERROR_CODE_TIMEOUT){
            std::cout << "Timeout while streaming" << std::endl;
            continue;
        }

        if(rx_metadata.error_code == uhd::rx_metadata_t::ERROR_CODE_OVERFLOW){
            std::cout << "Overflow" << std::endl;
            continue;
        }

        if (rx_metadata.error_code != uhd::rx_metadata_t::ERROR_CODE_NONE) {
            throw std::runtime_error(str(boost::format("Receiver error %s") % rx_metadata.strerror()));
        }

        if(outfile_stream->is_open()){
            outfile_stream->write((const char *) &buffer.front(), num_rx_samps * sizeof(std::complex<float>));
        }else{
            std::cout << "Outfile not open!" << std::endl;
        }
    }
}

void send_from_file(uhd::usrp::multi_usrp::sptr usrp_device, std::ifstream *infile_stream){
    // Set number of data points to get from the tx stream at one time
    size_t size_per_buffer = 5000;
    // Vector to hold data from the tx stream in the form of complex numbers (I Q)
    std::vector<std::complex<float> > buffer(size_per_buffer);
    // fc32: complex<float>, s16: Q16 I16
    uhd::stream_args_t stream_args("fc32", "sc16");
    // Ptr to tx stream
    uhd::tx_streamer::sptr tx_stream = usrp_device->get_tx_stream(stream_args);
    // Metadata from tx stream
    uhd::tx_metadata_t tx_metadata;
    uhd::stream_cmd_t stream_cmd(uhd::stream_cmd_t::STREAM_MODE_START_CONTINUOUS);

    // Ptr to rx stream
    uhd::rx_streamer::sptr rx_stream = usrp_device->get_rx_stream(stream_args);
    //rx_stream->issue_stream_cmd(stream_cmd);
    bool is_recv = false;

    while(not stop_signal_called){
        if(not is_recv){
            std::cout << "Start streaming, to stop: ctrl + c" << std::endl << std::endl;
        }
        if(not is_recv){
            //rx_stream->issue_stream_cmd(stream_cmd);
            std::cout << "Start streaming!";
            is_recv = true;
        }
        while(not infile_stream->eof()){
            std::cout << "2" << std::flush;
            // Read in data from binary as complex<float> and put in buffer vector
            infile_stream->read((char *) &buffer.front(), buffer.size() * sizeof(std::complex<float>));
            // Number of samples read from file
            size_t num_tx_samps = size_t(infile_stream->gcount() / sizeof(std::complex<float>));
            tx_stream->send(&buffer.front(), num_tx_samps, tx_metadata);
            // Start rx stream after first sent sample
        }
        // Read file from beginning
        infile_stream->clear();
        infile_stream->seekg(0, std::ios::beg);
    }
    rx_stream->issue_stream_cmd(uhd::stream_cmd_t::STREAM_MODE_STOP_CONTINUOUS);

}
