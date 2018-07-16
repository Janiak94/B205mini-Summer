#include <iostream>
#include <uhd/utils/safe_main.hpp>
#include <uhd/usrp/multi_usrp.hpp>
#include <boost/program_options.hpp>
#include <thread>
#include "util.hpp"
#include <limits>


bool stop_signal_called = false;
bool stop_receive_called = false;

void waitForKey(){
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

// Function responsible for controlling the operation
void waitForSeconds(int time){
    std::this_thread::sleep_for(std::chrono::milliseconds(time));
    std::cout << "Done!" << std::endl;
    stop_signal_called = true;
}

int UHD_SAFE_MAIN(int argc, char *argv[]) {
    stop_signal_called = false;
    uhd::set_thread_priority_safe();

    //Files
    //std::ofstream outfile_stream;
    //outfile_stream.open("output.bin", std::ofstream::binary);
    //std::ifstream infile_stream;
    //Options setup
    Usrp_data usrp_data;
    usrp_data.init(argc, argv);
    //usrp_data.of_open("outfile.bin");
    //usrp_data.init(argc, argv);

    //Data structure to hold streams


    // Time spec for when to start recv and send
    uhd::time_spec_t start_time = usrp_data.usrp_device->get_time_now() + uhd::time_spec_t(0.1);
    //usrp_data.tx_metadata.time_spec = start_time + uhd::time_spec_t(0.5);
    //usrp_data.rx_metadata.time_spec = start_time + uhd::time_spec_t(0.0);
    usrp_data.tx_metadata.has_time_spec = false;
    usrp_data.rx_metadata.has_time_spec = false;
    //Start send and receive threads, only handles bufferts
    boost::thread_group threads;

    // To recv to file, make sure that the flag is_sending is set to true somewhere, else no data is written

    threads.create_thread(boost::bind(&send_from_file, &usrp_data));
    std::cout << "Press Enter to start receiving" << std::endl;
    waitForKey();
    threads.create_thread(boost::bind(recv_to_file, &usrp_data));
    std::cout << "Started receiving. Press enter to stop sending" << std::endl;
    //waitForKey();
    waitForSeconds(2000);
    threads.join_all();

    /*
     * TODO: Hålla koll på om TX-bufferten är tom vid varje sändning, kanske ligger kvar från gammal
     * TODO: Implementera zeropadding
     * TODO: Fixa fragment_offset = 0;
     */

    //std::this_thread::sleep_for(std::chrono::seconds(1));
    // Empty async message from tx stream to get rid of weird U-message after program closes
    uhd::async_metadata_t async_metadata;
    usrp_data.tx_stream->recv_async_msg(async_metadata);

    //std::cout << "Samples sent: " << usrp_data.num_tx_samps << std::endl;
    //std::cout << "Samples recieved: " << usrp_data.num_rx_samps << std::endl;
    //std::cout << usrp_data.rx_metadata.fragment_offset << std::endl;

    return EXIT_SUCCESS;
}