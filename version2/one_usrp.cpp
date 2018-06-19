#include <iostream>
#include <uhd/utils/safe_main.hpp>
#include <uhd/usrp/multi_usrp.hpp>
#include <boost/program_options.hpp>
#include <thread>
#include "util.hpp"
#include <uhd/utils/thread_priority.hpp>
#include <fstream>
#include <stdio.h>

bool stop_signal_called = false;

// Function responsible for controlling the operation
void controller(){
    /*char x;
    std::cin.get(x);          */
    boost::this_thread::sleep_for(boost::chrono::seconds(1));
    stop_signal_called = true;
    std::cout << "Stop signal sent" << std::endl;
}

int UHD_SAFE_MAIN(int argc, char *argv[]) {
    stop_signal_called = false;
    uhd::set_thread_priority_safe();

    //Files
    std::ofstream outfile_stream;
    outfile_stream.open("outfile.bin", std::ofstream::binary);
    std::ifstream infile_stream;
    
    //Options setup
    uhd::usrp::multi_usrp::sptr usrp_device;
    startup(usrp_device, infile_stream, argc, argv);

    //Data structure to hold streams
    Usrp_data usrp_data(usrp_device, uhd::stream_args_t("fc32", "sc16"));

    //boost::thread zeropadding_thread(boost::bind(&send_zeropadding, usrp_device, &usrp_data));
    //zeropadding_thread.join();

    // Time spec for when to start recv and send
    uhd::time_spec_t start_time = usrp_device->get_time_now() + uhd::time_spec_t(0.1);
    usrp_data.tx_metadata.time_spec = start_time + uhd::time_spec_t(0.0);
    usrp_data.rx_metadata.time_spec = start_time + uhd::time_spec_t(0.0);
    usrp_data.tx_metadata.has_time_spec = true;
    usrp_data.rx_metadata.has_time_spec = true;

    //Start send and receive threads, only handles bufferts
    boost::thread_group threads;
    // To recv to file, make sure that the flag is_sending is set to true somewhere, else no data is written
    threads.create_thread(boost::bind(&recv_to_file, usrp_device, &usrp_data, &outfile_stream));
    threads.create_thread(boost::bind(&send_from_file, usrp_device, &usrp_data, &infile_stream));
    threads.create_thread(boost::bind(&controller));
    threads.join_all();

    outfile_stream.close();
    infile_stream.close();

    std::cout << std::endl;

    std::cout << usrp_data.num_tx_samps << std::endl;
    std::cout << usrp_data.num_rx_samps << std::endl;
    std::cout << usrp_data.rx_metadata.fragment_offset << std::endl;

    /*
     * TODO: Hålla koll på om TX-bufferten är tom vid varje sändning, kanske ligger kvar från gammal
     * TODO: Implementera zeropadding
     * TODO: Fixa fragment_offset = 0;
     */

    return EXIT_SUCCESS;
}