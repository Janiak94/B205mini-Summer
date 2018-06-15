//
// Created by Jens Nilsson on 2018-06-14.
//

static bool stop_signal_called = false;

void startup(uhd::usrp::multi_usrp::sptr &usrp_device, int argc, char *argv[]);

void recv_to_file(uhd::usrp::multi_usrp::sptr usrp_device, std::ofstream *outfile_stream);

void send_from_file(uhd::usrp::multi_usrp::sptr usrp_device, std::ifstream *infile_stream);