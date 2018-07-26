//
// Created by Jens Nilsson on 2018-06-14.
//
#include "util.hpp"


namespace po = boost::program_options;

void Usrp_data::init(int argc, char *argv[]){

    usrp_device = uhd::usrp::multi_usrp::make(device_addr);

    //transmit & receive variables set by po
    double tx_gain, tx_bw;
    double rx_gain, rx_bw;
    double samp_rate, freq;
    std::string file_name, out_name;

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
            ("file-name", po::value<std::string>(&file_name), "name of signal file")
            ("out-name", po::value<std::string>(&out_name), "name of signal file")
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
    //usrp_device->set_tx_antenna("TX/RX");
    usrp_device->set_rx_antenna("TX/RX");

    //Setting tx_samp_rate
    std::cout << boost::format("Setting TX Rate: %f Msps...") % (samp_rate/1e6) << std::endl;
    usrp_device->set_tx_rate(samp_rate);
    std::cout << boost::format("Actual TX Rate: %f Msps...") % (usrp_device->get_tx_rate()/1e6) << std::endl;

    //Setting rx_samp_rate
    std::cout << boost::format("Setting RX Rate: %f Msps...") % (samp_rate/1e6) << std::endl;
    usrp_device->set_rx_rate(samp_rate);
    std::cout << boost::format("Actual RX Rate: %f Msps...") % (usrp_device->get_rx_rate()/1e6) << std::endl;

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
    std::cout << boost::format("Actual TX Freq: %f MHz...") % (usrp_device->get_tx_freq()/1e6) << std::endl;
    //RX
    std::cout << boost::format("Setting RX Freq: %f MHz...") % (freq/1e6) << std::endl;
    uhd::tune_request_t rx_tune_request(freq);
    usrp_device->set_rx_freq(rx_tune_request);
    std::cout << boost::format("Actual RX Freq: %f MHz...") % (usrp_device->get_rx_freq()/1e6) << std::endl;

    //Set TX-gain
    if (variables_map.count("tx-gain")){
        std::cout << boost::format("Setting TX Gain: %f dB...") % tx_gain << std::endl;
        usrp_device->set_tx_gain(tx_gain);
        std::cout << boost::format("Actual TX Gain: %f dB...") % usrp_device->get_tx_gain() << std::endl;
    }

    //Set RX-gain
    if (variables_map.count("rx-gain")){
        std::cout << boost::format("Setting RX Gain: %f dB...") % rx_gain << std::endl;
        usrp_device->set_rx_gain(rx_gain);
        std::cout << boost::format("Actual RX Gain: %f dB...") % usrp_device->get_rx_gain() << std::endl;
    }

    //Set TX-bandwidth
    if (not variables_map.count("tx-bw")){
        std::cout << boost::format("TX Bandwidth: %f MHz...") % (usrp_device->get_tx_bandwidth()/1e6) << std::endl;
    }else{
        std::cout << boost::format("Setting TX Bandwidth: %f MHz...") % (tx_bw/1e6) << std::endl;
        usrp_device->set_tx_bandwidth(tx_bw);
        std::cout << boost::format("Actual TX Bandwidth: %f MHz...") % (usrp_device->get_tx_bandwidth()/1e6) << std::endl;
    }

    //Set RX-bandwidth
    if (not variables_map.count("rx-bw")){
        std::cout << boost::format("RX Bandwidth: %f MHz...") % (usrp_device->get_rx_bandwidth()/1e6) << std::endl;
    }else{
        std::cout << boost::format("Setting RX Bandwidth: %f MHz...") % (rx_bw/1e6) << std::endl;
        usrp_device->set_rx_bandwidth(rx_bw);
        std::cout << boost::format("Actual RX Bandwidth: %f MHz...") % (usrp_device->get_rx_bandwidth()/1e6) << std::endl;
    }

    //DC-offset is set to false
    usrp_device->set_rx_dc_offset(false);
    std::cout << "RX DC offset false" << std::endl << std::endl;

    //neither tx dc-offset nor tx iq-balance possible to set on this device
    //usrp_device->set_tx_dc_offset(false);
    usrp_device->set_rx_iq_balance(true,0);
    //usrp_device->set_tx_iq_balance(true,0);

    //Check Ref and LO Lock detect
    std::vector<std::string> sensor_names;
    sensor_names = usrp_device->get_tx_sensor_names(0);
    if (std::find(sensor_names.begin(), sensor_names.end(), "lo_locked") != sensor_names.end()) {
        uhd::sensor_value_t lo_locked = usrp_device->get_tx_sensor("lo_locked",0);
        std::cout << boost::format("Checking lock: %s ...") % lo_locked.to_pp_string() << std::endl;
        UHD_ASSERT_THROW(lo_locked.to_bool());
    }

    /*
    //Print active filters:
    for(std::string filter_name : usrp_device->get_filter_names()) {
        std::cout << filter_name
                  << " " << (usrp_device->get_filter(filter_name)->is_bypassed() ? "bypassed" : "enabled")
                  << std::endl;
    }
     */

    //Signal files
    std::vector<std::string> files = boost::assign::list_of("constant.bin")("square.bin")("pulse.bin")("random.bin")("constnew.bin")
            ("superpos.bin")("k1sine.bin");

    //Checking for valid file name and opening corresponding file
    if(not variables_map.count("file-name")){
        //Default file to open
        if_open("constant.bin");
        std::cout << "Opened default file constant.bin" << std::endl;
    } else {
        //Open file
        std::cout << boost::format("Trying to open file %s") % (file_name) << std::endl;
        if (std::find(files.begin(), files.end(), file_name) != files.end()) {
            if_open(file_name);
            std::cout << boost::format("Opened file %s") % (file_name) << std::endl;
        } else {
            //No file found, terminate
            std::cout << boost::format("File %s not found \nProgram exit.\n") % (file_name) << std::endl;
            exit (EXIT_FAILURE);
        }
    }

    //Open output file
    if(not variables_map.count("out-name")){
        // Default outfile name
        //of_open("outfile.bin");
    } else{
        //of_open(file_name);
    }

    usrp_device->set_time_now(uhd::time_spec_t(0.0));
    uhd::stream_args_t stream_args("fc32", "sc16");
    rx_stream = usrp_device->get_rx_stream(stream_args);
    tx_stream = usrp_device->get_tx_stream(stream_args);
    rx_metadata = uhd::rx_metadata_t();
    tx_metadata = uhd::tx_metadata_t();
    num_rx_samps = 0;
    num_tx_samps = 0;
    is_sending = false;
    is_recieving = false;

    of_open(out_name);

    /*TODO
     * Vill börja sända ut nonsenssignal för att "värma upp" kortet istället för zeropadding
     */
}

void recv_to_file(Usrp_data *usrp_data){
    // Set number of data points to get from the rx stream at one time
    size_t size_per_buffer = usrp_data->rx_stream->get_max_num_samps();
    // Vector to hold data from the rx stream in the form of complex numbers (I Q)
    std::vector<std::complex<float> > buffer(size_per_buffer);
    usrp_data->rx_metadata.start_of_burst = true;
    usrp_data->rx_metadata.end_of_burst = false;
    usrp_data->of_open("iqimbalance.bin");

    //uhd::stream_cmd_t stream_cmd(uhd::stream_cmd_t::STREAM_MODE_START_CONTINUOUS);
    //stream_cmd.time_spec = usrp_device->get_time_now();
    //usrp_data->rx_stream->issue_stream_cmd(stream_cmd);

    //usrp_data->is_sending = true;

    int totalNumSamp = 0;

    //while(not stop_signal_called and not stop_receive_called){
    while(not stop_signal_called and totalNumSamp < 3.5e6){
        // Wait for send_from_file() function to signal

        if(usrp_data->is_sending) {
            // Get number of samples written to buffer and fill buffer and set metadata
            size_t num_rx_samps = usrp_data->rx_stream->
                    recv(&buffer.front(), buffer.size(), usrp_data->rx_metadata, 0.1);

            totalNumSamp += num_rx_samps;
            usrp_data->is_recieving = true;
            usrp_data->rx_metadata.start_of_burst = false;
            usrp_data->rx_metadata.has_time_spec = false;

            usrp_data->num_rx_samps += num_rx_samps;
            if (usrp_data->rx_metadata.error_code == uhd::rx_metadata_t::ERROR_CODE_TIMEOUT) {
                std::cout << "Timeout while streaming" << std::endl;
                continue;
            }

            if (usrp_data->rx_metadata.error_code == uhd::rx_metadata_t::ERROR_CODE_OVERFLOW) {
                std::cout << "Overflow" << std::endl;
                continue;
            }

            if (usrp_data->rx_metadata.error_code != uhd::rx_metadata_t::ERROR_CODE_NONE) {
                throw std::runtime_error(str(boost::format("Receiver error %s") % usrp_data->rx_metadata.strerror()));
            }

            if (usrp_data->outfile_stream.is_open()) {
                usrp_data->outfile_stream.write((const char *) &buffer.front(), num_rx_samps * sizeof(std::complex<float>));
            } else {
                std::cout << "Outfile not open!" << std::endl;
            }
        }
    }

    usrp_data->rx_metadata.end_of_burst = true;
    //stop_signal_called = true;
}

void send_from_file(Usrp_data *usrp_data){
    // Set number of data points to get from the tx stream at one time
    size_t size_per_buffer = usrp_data->tx_stream->get_max_num_samps();
    // Vector to hold data from the tx stream in the form of complex numbers (I Q)
    std::vector<std::complex<float> > buffer(size_per_buffer);

    usrp_data->tx_metadata.start_of_burst = true;
    usrp_data->tx_metadata.end_of_burst = false;
    std::cout << "size per buffer: " << size_per_buffer << std::endl;

    while(not stop_signal_called){
        if(usrp_data->tx_metadata.start_of_burst){
            std::cout << "Started streaming" << std::endl;
        }
        while(not usrp_data->infile_stream.eof() and not stop_signal_called){
            // Read in data from binary as complex<float> and put in buffer vector
            usrp_data->infile_stream.read((char *) &buffer.front(), buffer.size() * sizeof(std::complex<float>));

            // Number of samples read from file
            size_t num_tx_samps = size_t(usrp_data->infile_stream.gcount() / sizeof(std::complex<float>));
            usrp_data->tx_stream->send(&buffer.front(), num_tx_samps, usrp_data->tx_metadata);
            //Used to communicate with recv function
            usrp_data->is_sending = true;

            //Do not use time spec för subsequent packets
            usrp_data->num_tx_samps += num_tx_samps;
            usrp_data->tx_metadata.has_time_spec = false;

            //  Previous (Start rx stream after first sent sample)
            //  EDIT! Start rx stream after start of burst in receive function
            if(usrp_data->rx_metadata.start_of_burst){
                uhd::stream_cmd_t stream_cmd(uhd::stream_cmd_t::STREAM_MODE_START_CONTINUOUS);
                stream_cmd.time_spec = usrp_data->rx_metadata.time_spec; //usrp_device->get_time_now();
                usrp_data->rx_stream->issue_stream_cmd(stream_cmd);
            }
            usrp_data->tx_metadata.start_of_burst = false;
        }
        // Read file from beginning
        usrp_data->infile_stream.clear();
        usrp_data->infile_stream.seekg(0, std::ios::beg);
    }
    usrp_data->tx_metadata.end_of_burst = true;
    usrp_data->tx_stream->send("", 0, usrp_data->tx_metadata);
    usrp_data->rx_stream->issue_stream_cmd(uhd::stream_cmd_t::STREAM_MODE_STOP_CONTINUOUS);
}

std::vector<std::complex<float> > generate_sine(const int freq, const int sample_rate) {
    //std::complex<float> temp[sample_rate];
    std::vector<std::complex<float> > sine;
    for(int i = 0; i < sample_rate; ++i)
        sine.push_back(std::complex<float>(sin(M_PI * (double)freq * (((double)i) / (double)sample_rate)),0));
    std::cout << sine.at(7e6-1);
    return sine;
}