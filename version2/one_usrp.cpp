#include <iostream>
#include <uhd/utils/safe_main.hpp>
#include <uhd/usrp/multi_usrp.hpp>
#include <boost/program_options.hpp>
#include <thread>
#include "util.hpp"
#include <uhd/utils/thread_priority.hpp>
#include <fstream>

// Function responsible for controlling the operation
void controller(){
    boost::this_thread::sleep_for(boost::chrono::seconds(1));
    //stop_signal_called = true;
}

int UHD_SAFE_MAIN(int argc, char *argv[]) {
    stop_signal_called = false;
    uhd::set_thread_priority_safe();

    uhd::usrp::multi_usrp::sptr usrp_device =  uhd::usrp::multi_usrp::make(uhd::device_addr_t());

    //Options setup
    startup(usrp_device, argc, argv);

    //Files
    std::ofstream outfile_stream;
    outfile_stream.open("outfile.bin", std::ofstream::binary);
    std::ifstream infile_stream;
    infile_stream.open("constant.bin", std::ifstream::binary);


    /*
     * TODO: Skicka kommandon till RX och TX att starta NU med stream_cmd. Kanske skicka något skräp
     */
    // How to interpret data from streams


    //Start send and receive threads, only handles bufferts
    boost::thread_group threads;
    threads.create_thread(boost::bind(&recv_to_file,usrp_device, &outfile_stream));
    //threads.create_thread(boost::bind(&controller));
    threads.create_thread(boost::bind(&send_from_file, usrp_device, &infile_stream));
    threads.join_all();

    outfile_stream.close();
    infile_stream.close();

    return EXIT_SUCCESS;
}