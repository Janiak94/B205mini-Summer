//
// Created by Jens Nilsson on 2018-06-11.
//

static bool stop_signal_called = false;

void send_from_file(uhd::usrp::multi_usrp::sptr usrp_device, double start_time, const std::string &file);

void recv_to_file(uhd::usrp::multi_usrp::sptr usrp_device, double start_time, const std::string &file);

void save_from_buffer(
        const std::vector<std::complex<float> > *buff, uhd::usrp::multi_usrp::sptr usrp_device,
        std::ofstream &outfile, size_t num_tx_samps, std::ofstream &timefile);


