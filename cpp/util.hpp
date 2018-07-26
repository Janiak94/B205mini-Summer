//
// Created by Jens Nilsson on 2018-06-14.
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
#include <chrono>
#include <csignal>
#include <hidapi.h>
#include <stdio.h>
#include <stdlib.h>
#include <boost/assign/list_of.hpp>
#include <math.h>

extern bool stop_signal_called;
extern bool stop_receive_called;

struct Usrp_data{
    uhd::usrp::multi_usrp::sptr usrp_device;
    // Ptrs to rx and tx streams. Should only be declared once
    uhd::rx_streamer::sptr rx_stream;
    uhd::tx_streamer::sptr tx_stream;
    // Metadata from send and recieve. Tell streams when to start and so on
    uhd::tx_metadata_t tx_metadata;
    uhd::rx_metadata_t rx_metadata;
    // Total number of samples sent and recieved
    size_t num_rx_samps;
    size_t num_tx_samps;
    // Keep track of stream statuses
    bool is_sending;
    bool is_recieving;
    uhd::device_addr_t device_addr;
    std::ifstream infile_stream;
    std::ofstream outfile_stream;

    Usrp_data(uhd::device_addr_t device_addr = uhd::device_addr_t()){
        this->device_addr = device_addr;
    }

    ~Usrp_data(){
        infile_stream.close();
        outfile_stream.close();
    }

    void init(int argc, char *argv[]);

    void if_open(const std::string if_name){
        infile_stream.open(if_name, std::ifstream::binary);
    }

    void of_open(const std::string of_name){
        outfile_stream.open(of_name, std::ofstream::binary);
    }

};

//void startup(Usrp_data *usrp_data, int argc, char *argv[]);

void recv_to_file(Usrp_data *usrp_data);

void send_from_file(Usrp_data *usrp_data);

std::vector<std::complex<float> > generate_sine(const int freq, const int sample_rate);
