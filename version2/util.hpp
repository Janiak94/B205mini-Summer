//
// Created by Jens Nilsson on 2018-06-14.
//

extern bool stop_signal_called;

struct Usrp_data{
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
    //size_t current_tx_samps;
    Usrp_data(uhd::usrp::multi_usrp::sptr &usrp_device, uhd::stream_args_t stream_args){
        rx_stream = usrp_device->get_rx_stream(stream_args);
        tx_stream = usrp_device->get_tx_stream(stream_args);
        rx_metadata = uhd::rx_metadata_t();
        tx_metadata = uhd::tx_metadata_t();
        num_rx_samps = 0;
        num_tx_samps = 0;
        is_sending = false;
        is_recieving = false;
        //current_tx_samps = 0;
    }
};

void startup(uhd::usrp::multi_usrp::sptr &usrp_device, std::ifstream &infile_stream, int argc, char *argv[]);

void recv_to_file(uhd::usrp::multi_usrp::sptr &usrp_device, Usrp_data *usrp_data, std::ofstream *outfile_stream);

void send_from_file(uhd::usrp::multi_usrp::sptr &usrp_device, Usrp_data *usrp_data, std::ifstream *infile_stream);
