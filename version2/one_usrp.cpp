#include <iostream>
#include <uhd/utils/safe_main.hpp>
#include <uhd/usrp/multi_usrp.hpp>
#include <boost/program_options.hpp>
#include <thread>
#include "util.hpp"
#include <uhd/utils/thread_priority.hpp>
#include <fstream>

bool stop_signal_called = false;

// Function responsible for controlling the operation
void controller(){
    boost::this_thread::sleep_for(boost::chrono::milliseconds(2000));
    stop_signal_called = true;
    std::cout << "Stop signal sent" << std::endl;
}

int UHD_SAFE_MAIN(int argc, char *argv[]) {
    stop_signal_called = false;
    uhd::set_thread_priority_safe();


    //Options setup
    uhd::usrp::multi_usrp::sptr usrp_device;
    startup(usrp_device, argc, argv);

    //Data structure to hold streams
    Usrp_data usrp_data(usrp_device, uhd::stream_args_t("fc32", "sc16"));

    uhd::time_spec_t start_time = usrp_device->get_time_now() + uhd::time_spec_t(0.001);
    usrp_data.tx_metadata.has_time_spec = true;
    usrp_data.rx_metadata.has_time_spec = true;
    usrp_data.tx_metadata.time_spec = start_time + uhd::time_spec_t(0.1);
    usrp_data.rx_metadata.time_spec = start_time;

    //Files
    std::ofstream outfile_stream;
    outfile_stream.open("outfile.bin", std::ofstream::binary);
    std::ifstream infile_stream;
    infile_stream.open("sqaure.bin", std::ifstream::binary);


    /*
     * TODO: Skicka kommandon till RX och TX att starta NU med stream_cmd. Kanske skicka något skräp
     */
    // How to interpret data from streams
    //boost::thread zeropadding_thread(&send_zeropadding,usrp_device);
    //zeropadding_thread.join();

    //Start send and receive threads, only handles bufferts
    boost::thread_group threads;
    threads.create_thread(boost::bind(&recv_to_file, usrp_device, &usrp_data, &outfile_stream));
    threads.create_thread(boost::bind(&controller));
    threads.create_thread(boost::bind(&send_from_file, usrp_device, &usrp_data, &infile_stream));
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