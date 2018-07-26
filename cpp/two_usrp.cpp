#include <iostream>
#include <uhd/utils/safe_main.hpp>
#include <uhd/usrp/multi_usrp.hpp>
#include <boost/program_options.hpp>
#include <thread>
#include "util.hpp"


bool stop_signal_called = false;
bool stop_recevie_called = false;

// Function responsible for controlling the operation
void controller(){
    char x;
    //std::cin.get(x);
    boost::this_thread::sleep_for(boost::chrono::milliseconds(100));
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
    uhd::device_addr_t tx_hint;
    uhd::device_addr_t rx_hint;
    rx_hint["serial"] = "30DBB29";
    tx_hint["serial"] = "30DBB0D";
    Usrp_data *usrp_data_ptr1 = new Usrp_data(tx_hint);
    usrp_data_ptr1->init(argc, argv);
    Usrp_data *usrp_data_ptr2 = new Usrp_data(rx_hint);
    usrp_data_ptr2->init(argc, argv);
    //usrp_data_ptr2->usrp_device->set_rx_antenna("TX/RX");
    usrp_data_ptr2->usrp_device->set_tx_gain(0);
    usrp_data_ptr1->usrp_device->set_time_now(uhd::time_spec_t(0.0));
    usrp_data_ptr2->usrp_device->set_time_now(uhd::time_spec_t(0.0));
    std::this_thread::sleep_for(std::chrono::seconds(1));
    //usrp_data.init(argc, argv);

    //Data structure to hold streams


    // Time spec for when to start recv and send
    //uhd::time_spec_t start_time = usrp_data.usrp_device->get_time_now() + uhd::time_spec_t(0.1);
    //usrp_data.tx_metadata.time_spec = start_time + uhd::time_spec_t(0.5);
    //usrp_data.rx_metadata.time_spec = start_time + uhd::time_spec_t(0.0);
    usrp_data_ptr1->tx_metadata.has_time_spec = false;
    usrp_data_ptr1->rx_metadata.has_time_spec = false;
    usrp_data_ptr2->tx_metadata.has_time_spec = false;
    usrp_data_ptr2->rx_metadata.has_time_spec = false;
    usrp_data_ptr2->of_open("outfile.bin");
    //Start send and receive threads, only handles bufferts
    boost::thread_group threads;
    usrp_data_ptr2->is_sending = true;
    usrp_data_ptr2->rx_stream->issue_stream_cmd(uhd::stream_cmd_t(uhd::stream_cmd_t::STREAM_MODE_START_CONTINUOUS));
    // To recv to file, make sure that the flag is_sending is set to true somewhere, else no data is written
    threads.create_thread(boost::bind(&recv_to_file, usrp_data_ptr2));
    threads.create_thread(boost::bind(&send_from_file, usrp_data_ptr1));
    threads.create_thread(boost::bind(&controller));
    threads.join_all();

    /*
     * TODO: Hålla koll på om TX-bufferten är tom vid varje sändning, kanske ligger kvar från gammal
     * TODO: Implementera zeropadding
     * TODO: Fixa fragment_offset = 0;
     */

    //std::this_thread::sleep_for(std::chrono::seconds(1));
    // Empty async message from tx stream to get rid of weird U-message after program closes
    uhd::async_metadata_t async_metadata;
    usrp_data_ptr1->tx_stream->recv_async_msg(async_metadata);

    //std::cout << "Samples sent: " << usrp_data.num_tx_samps << std::endl;
    //std::cout << "Samples recieved: " << usrp_data.num_rx_samps << std::endl;
    //std::cout << usrp_data.rx_metadata.fragment_offset << std::endl;
    delete usrp_data_ptr1;
    delete usrp_data_ptr2;

    return EXIT_SUCCESS;
}