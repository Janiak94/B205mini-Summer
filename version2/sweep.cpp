//
// Created by Jens Nilsson on 2018-06-26.
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
#include <boost/algorithm/string.hpp>
#include <boost/filesystem.hpp>
#include <iostream>
#include <fstream>
#include <csignal>
#include <thread>

static const size_t wave_table_len = 8192;

class wave_table_class{
public:
    wave_table_class(const std::string &wave_type, const float ampl):
            _wave_table(wave_table_len)
    {
        //compute real wave table with 1.0 amplitude
        std::vector<float> real_wave_table(wave_table_len);
        if (wave_type == "CONST"){
            for (size_t i = 0; i < wave_table_len; i++)
                real_wave_table[i] = 1.0;
        }
        else if (wave_type == "SQUARE"){
            for (size_t i = 0; i < wave_table_len; i++)
                real_wave_table[i] = (i < wave_table_len/2)? 0.0 : 1.0;
        }
        else if (wave_type == "RAMP"){
            for (size_t i = 0; i < wave_table_len; i++)
                real_wave_table[i] = 2.0*i/(wave_table_len-1) - 1.0;
        }
        else if (wave_type == "SINE"){
            static const double tau = 2*std::acos(-1.0);
            for (size_t i = 0; i < wave_table_len; i++)
                real_wave_table[i] = std::sin((tau*i)/wave_table_len);
        }
        else throw std::runtime_error("unknown waveform type: " + wave_type);

        //compute i and q pairs with 90% offset and scale to amplitude
        for (size_t i = 0; i < wave_table_len; i++){
            const size_t q = (i+(3*wave_table_len)/4)%wave_table_len;
            _wave_table[i] = std::complex<float>(
                    ampl*real_wave_table[i],
                    ampl*real_wave_table[q]
            );
        }
    }

    inline std::complex<float> operator()(const size_t index) const{
        return _wave_table[index % wave_table_len];
    }

private:
    std::vector<std::complex<float> > _wave_table;
};
/*
static bool stop_signal_called = false;

namespace po = boost::program_options;

void transmit_worker(
        std::vector<std::complex<float> > buff,
        wave_table_class wave_table,
        uhd::tx_streamer::sptr tx_stream,
        uhd::tx_metadata_t metadata,
        size_t step,
        size_t index
){

    while(not stop_signal_called){
        for (size_t n = 0; n < buff.size(); n++){
            buff[n] = wave_table(index += step);
        }

        tx_stream->send(&buff.front(), buff.size(), metadata);

        metadata.start_of_burst = false;
        metadata.has_time_spec = false;
    }

    metadata.end_of_burst = true;
    tx_stream->send("", 0, metadata);
}


void recv_to_file(
        uhd::usrp::multi_usrp::sptr usrp,
        uhd::rx_streamer::sptr rx_stream,
        size_t samps_per_buff,
        int num_requested_samples,
        double settling_time,
        std::ofstream &signal_file,
        std::ofstream &average_file
){
    int num_total_samps = 0;
    uhd::rx_metadata_t md;
    std::vector<std::complex<float> > buff(samps_per_buff);
    bool overflow_message = true;
    double timeout = settling_time + 0.1f;

    uhd::stream_cmd_t stream_cmd((num_requested_samples == 0)?
                                uhd::stream_cmd_t::STREAM_MODE_START_CONTINUOUS:
                                uhd::stream_cmd_t::STREAM_MODE_NUM_SAMPS_AND_DONE
    );
    stream_cmd.num_samps = num_requested_samples;
    stream_cmd.stream_now = false;
    stream_cmd.time_spec = usrp->get_time_now() + uhd::time_spec_t(settling_time);
    rx_stream->issue_stream_cmd(stream_cmd);

    float average = 0;

    while(not stop_signal_called and
            (num_requested_samples > num_total_samps or num_requested_samples == 0)){
        size_t num_rx_samps = rx_stream->recv(&buff.front(), samps_per_buff, md, timeout);
        timeout = 0.1f;
        md.has_time_spec = false;

        if(md.error_code == uhd::rx_metadata_t::ERROR_CODE_TIMEOUT) {
            std::cout << boost::format("Timeout while streaming") << std::endl;
            break;
        }
        if(md.error_code == uhd::rx_metadata_t::ERROR_CODE_OVERFLOW){
            if(overflow_message){
                overflow_message = false;
                std::cerr << boost::format("Overflow while streaming at rate %.3f") %
                        (usrp->get_rx_rate() * sizeof(std::complex<float>)/1e6);
            }
            continue;
        }
        if(md.error_code != uhd::rx_metadata_t::ERROR_CODE_NONE){
            throw std::runtime_error(str(boost::format(
                    "Receiver error %s"
            ) % md.strerror()));
        }

        num_total_samps += num_rx_samps;

        signal_file.write((const char*) &buff.front(), num_rx_samps*sizeof(std::complex<float>));

        for(size_t i = 0; i < num_rx_samps; i++){
            average += std::abs(buff[i]);
        }
    }

    average /= num_total_samps;
    boost::format temp = (boost::format("%f\n") % average);
    std::string average_str = temp.str();
    average_file.write(&average_str.front(), average_str.length() * sizeof(char));

    stream_cmd.stream_mode = uhd::stream_cmd_t::STREAM_MODE_STOP_CONTINUOUS;
    rx_stream->issue_stream_cmd(stream_cmd);
    stop_signal_called = true;

}

int UHD_SAFE_MAIN(int argc, char *argv[]){
    uhd::set_thread_priority_safe();

    std::string tx_args, wave_type, tx_ant, tx_subdev, ref, otw, tx_channels;
    double tx_rate, tx_freq, tx_gain, wave_freq, tx_bw;
    float ampl;
    double freq;

    std::string rx_args, file, type, rx_ant, rx_subdev, rx_channels;
    size_t total_num_samps, spb;
    double rx_rate, rx_freq, rx_gain, rx_bw;
    double settling;

    po::options_description desc("Allowed options");
    desc.add_options()
            ("help", "help message")
            ("file", po::value<std::string>(&file)->default_value("usrp_samples.dat"), "name of the file to write binary samples to")
            ("nsamps", po::value<size_t>(&total_num_samps)->default_value(0), "total number of samples to receive")
            ("settling", po::value<double>(&settling)->default_value(double(0.2)), "settling time (seconds) before receiving")
            ("spb", po::value<size_t>(&spb)->default_value(0), "samples per buffer, 0 for default")
            ("tx-rate", po::value<double>(&tx_rate), "rate of transmit outgoing samples")
            ("rx-rate", po::value<double>(&rx_rate), "rate of receive incoming samples")
            ("freq", po::value<double>(&freq), "RF center frequency in Hz")
            ("ampl", po::value<float>(&ampl)->default_value(float(0.3)), "amplitude of the waveform [0 to 0.7]")
            ("tx-gain", po::value<double>(&tx_gain), "gain for the transmit RF chain")
            ("rx-gain", po::value<double>(&rx_gain), "gain for the receive RF chain")
            ("tx-bw", po::value<double>(&tx_bw), "analog transmit filter bandwidth in Hz")
            ("rx-bw", po::value<double>(&rx_bw), "analog receive filter bandwidth in Hz")
            ("wave-type", po::value<std::string>(&wave_type)->default_value("CONST"), "waveform type (CONST, SQUARE, RAMP, SINE)")
            ("wave-freq", po::value<double>(&wave_freq)->default_value(0), "waveform frequency in Hz")
            ("tx-int-n", "tune USRP TX with integer-N tuning")
            ("rx-int-n", "tune USRP RX with integer-N tuning")
            ;
    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm);

    if(vm.count("help")){
        std::cout << boost::format("UHD TXRX Loopback to File %s") % desc << std::endl;
        return ~0;
    }

    uhd::usrp::multi_usrp::sptr usrp = uhd::usrp::multi_usrp::make(uhd::device_addr_t());

    usrp->set_clock_source("internal");
    usrp->set_rx_dc_offset(false);

    if (not vm.count("tx-rate")){
        std::cerr << "Please specify TX sample rate" << std::endl;
        return ~0;
    }
    if (not vm.count("rx-rate")){
        std::cerr << "Please specify RX sample rate" << std::endl;
        return ~0;
    }
    usrp->set_tx_rate(tx_rate);
    usrp->set_rx_rate(rx_rate);
    std::cout << boost::format("Actual TX Rate: %f Msps") % (usrp->get_tx_rate()/1e6) << std::endl;
    std::cout << boost::format("Actual RX Rate: %f Msps") % (usrp->get_rx_rate()/1e6) << std::endl;
    if (vm.count("tx-bw")){
        usrp->set_tx_bandwidth(tx_bw);
        std::cout << boost::format("Actual TX BW: %f MHz") % (usrp->get_tx_bandwidth()/1e6) << std::endl;
    }
    if (vm.count("rx-bw")){
        usrp->set_rx_bandwidth(rx_bw);
        std::cout << boost::format("Actual RX BW: %f MHz") % (usrp->get_rx_bandwidth()/1e6) << std::endl;
    }
    if (vm.count("tx-gain")){
        usrp->set_tx_gain(tx_gain);
        std::cout << boost::format("Actual TX Gain: %f dB") % (usrp->get_tx_gain()) << std::endl;
    }
    if(vm.count("rx-gain")){
        usrp->set_rx_gain(rx_gain);
        std::cout << boost::format("Actual RX Gain: %f dB") % (usrp->get_rx_gain()) << std::endl;
    }

    usrp->set_tx_antenna("TX/RX");
    usrp->set_rx_antenna("RX2");
    if(wave_freq == 0 and wave_type == "CONST"){
        wave_freq = usrp->get_tx_rate()/2;
    }
    if(std::abs(wave_freq) > usrp->get_tx_rate()/2){
        throw std::runtime_error("wave freq out of Nyquist zone");
    }
    if(usrp->get_tx_rate()/std::abs(wave_freq) >  wave_table_len/2){
        throw std::runtime_error("wave freq too small for table");
    }

    const wave_table_class wave_table(wave_type, 0.7);
    const size_t step = boost::math::iround(wave_freq/usrp->get_tx_rate() * wave_table_len);
    size_t index = 0;

    uhd::stream_args_t stream_args("fc32", "sc16");
    uhd::tx_streamer::sptr tx_stream = usrp->get_tx_stream(stream_args);
    uhd::rx_streamer::sptr rx_stream = usrp->get_rx_stream(stream_args);

    if (spb == 0) spb = tx_stream->get_max_num_samps()*10;
    std::vector<std::complex<float> > buff(spb);
    for(int i = 0; i < spb; i++){
        buff[i].real(1.0f);
        buff[i].imag(0.0f);
    }


    std::cout << "Setting device timestap to 0..." << std::endl;
    usrp->set_time_now(uhd::time_spec_t(0.0));

    std::ofstream signal_file, average_file;
    signal_file.open("sweep.bin", std::ofstream::binary);
    average_file.open("average.txt");

    double freq_step = 1e8;
    double max_freq = 1.2e9;
    for(; freq < max_freq; freq += freq_step){
        usrp->set_tx_freq(freq);
        usrp->set_rx_freq(freq);
        std::cout << boost::format("Actual TX Freq: %f MHz") % (usrp->get_tx_freq()/1e6) << std::endl;
        std::cout << boost::format("Actual RX Freq: %f MHz") % (usrp->get_rx_freq()/1e6) << std::endl;

        std::vector<std::string> tx_sensor_names, rx_sensor_names;
        tx_sensor_names = usrp->get_tx_sensor_names(0);
        if (std::find(tx_sensor_names.begin(), tx_sensor_names.end(), "lo_locked") != tx_sensor_names.end()) {
            uhd::sensor_value_t lo_locked = usrp->get_tx_sensor("lo_locked",0);
            std::cout << boost::format("Checking TX: %s ...") % lo_locked.to_pp_string() << std::endl;
            UHD_ASSERT_THROW(lo_locked.to_bool());
        }
        rx_sensor_names = usrp->get_rx_sensor_names(0);
        if (std::find(rx_sensor_names.begin(), rx_sensor_names.end(), "lo_locked") != rx_sensor_names.end()) {
            uhd::sensor_value_t lo_locked = usrp->get_rx_sensor("lo_locked",0);
            std::cout << boost::format("Checking RX: %s ...") % lo_locked.to_pp_string() << std::endl;
            UHD_ASSERT_THROW(lo_locked.to_bool());
        }
        std::this_thread::sleep_for(std::chrono::seconds(5));
        uhd::tx_metadata_t md;
        md.start_of_burst = true;
        md.end_of_burst = false;
        md.has_time_spec = true;
        md.time_spec = usrp->get_time_now() + uhd::time_spec_t(1.0);

        boost::thread_group transmit_thread;
        transmit_thread.create_thread(boost::bind(&transmit_worker, buff, wave_table, tx_stream, md, step, index));
        recv_to_file(usrp,rx_stream, spb, total_num_samps, settling, signal_file, average_file);

        stop_signal_called = true;
        transmit_thread.join_all();
        stop_signal_called = false;
    }
    signal_file.close();
    average_file.close();
    std::cout << std::endl << "Done!" << std::endl << std::endl;
    return EXIT_SUCCESS;
}
*/

namespace po = boost::program_options;

/***********************************************************************
 * Signal handlers
 **********************************************************************/
static bool stop_signal_called = false;
void sig_int_handler(int){stop_signal_called = true;}

/***********************************************************************
 * Utilities
 **********************************************************************/
//! Change to filename, e.g. from usrp_samples.dat to usrp_samples.00.dat,
//  but only if multiple names are to be generated.
std::string generate_out_filename(const std::string &base_fn, size_t n_names, size_t this_name)
{
    if (n_names == 1) {
        return base_fn;
    }

    boost::filesystem::path base_fn_fp(base_fn);
    base_fn_fp.replace_extension(
            boost::filesystem::path(
                    str(boost::format("%02d%s") % this_name % base_fn_fp.extension().string())
            )
    );
    return base_fn_fp.string();
}


/***********************************************************************
 * transmit_worker function
 * A function to be used as a boost::thread_group thread for transmitting
 **********************************************************************/
void transmit_worker(
        std::vector<std::complex<float> > buff,
        wave_table_class wave_table,
        uhd::tx_streamer::sptr tx_streamer,
        uhd::tx_metadata_t metadata,
        size_t step,
        size_t index,
        int num_channels
){
    std::vector<std::complex<float> *> buffs(num_channels, &buff.front());

    //send data until the signal handler gets called
    while(not stop_signal_called){
        //fill the buffer with the waveform
        for (size_t n = 0; n < buff.size(); n++){
            buff[n] = wave_table(index += step);
        }

        //send the entire contents of the buffer
        tx_streamer->send(buffs, buff.size(), metadata);

        metadata.start_of_burst = false;
        metadata.has_time_spec = false;
    }

    //send a mini EOB packet
    metadata.end_of_burst = true;
    tx_streamer->send("", 0, metadata);
}


/***********************************************************************
 * recv_to_file function
 **********************************************************************/
template<typename samp_type> void recv_to_file(
        uhd::usrp::multi_usrp::sptr usrp,
        const std::string &cpu_format,
        const std::string &wire_format,
        std::ofstream &signal_file,
        std::ofstream &average_file,
        size_t samps_per_buff,
        int num_requested_samples,
        double settling_time,
        std::vector<size_t> rx_channel_nums
){
    int num_total_samps = 0;
    //create a receive streamer
    uhd::stream_args_t stream_args(cpu_format,wire_format);
    stream_args.channels = rx_channel_nums;
    uhd::rx_streamer::sptr rx_stream = usrp->get_rx_stream(stream_args);

    // Prepare buffers for received samples and metadata
    uhd::rx_metadata_t md;
    std::vector <std::vector< samp_type > > buffs(
            rx_channel_nums.size(), std::vector< samp_type >(samps_per_buff)
    );
    //create a vector of pointers to point to each of the channel buffers
    std::vector<samp_type *> buff_ptrs;
    for (size_t i = 0; i < buffs.size(); i++) {
        buff_ptrs.push_back(&buffs[i].front());
    }

    // Create one ofstream object per channel
    // (use shared_ptr because ofstream is non-copyable)
    UHD_ASSERT_THROW(buffs.size() == rx_channel_nums.size());
    bool overflow_message = true;
    double timeout = settling_time + 0.1f; //expected settling time + padding for first recv

    //setup streaming
    uhd::stream_cmd_t stream_cmd((num_requested_samples == 0)?
                                 uhd::stream_cmd_t::STREAM_MODE_START_CONTINUOUS:
                                 uhd::stream_cmd_t::STREAM_MODE_NUM_SAMPS_AND_DONE
    );
    stream_cmd.num_samps = num_requested_samples;
    stream_cmd.stream_now = false;
    stream_cmd.time_spec = uhd::time_spec_t(settling_time);
    rx_stream->issue_stream_cmd(stream_cmd);

    double average = 0;
    while(not stop_signal_called and (num_requested_samples > num_total_samps or num_requested_samples == 0)){
        size_t num_rx_samps = rx_stream->recv(buff_ptrs, samps_per_buff, md, timeout);
        timeout = 0.1f; //small timeout for subsequent recv

        if (md.error_code == uhd::rx_metadata_t::ERROR_CODE_TIMEOUT) {
            std::cout << boost::format("Timeout while streaming") << std::endl;
            break;
        }
        if (md.error_code == uhd::rx_metadata_t::ERROR_CODE_OVERFLOW){
            if (overflow_message){
                overflow_message = false;
                std::cerr << boost::format(
                        "Got an overflow indication. Please consider the following:\n"
                                "  Your write medium must sustain a rate of %fMB/s.\n"
                                "  Dropped samples will not be written to the file.\n"
                                "  Please modify this example for your purposes.\n"
                                "  This message will not appear again.\n"
                ) % (usrp->get_rx_rate()*sizeof(samp_type)/1e6);
            }
            continue;
        }
        if (md.error_code != uhd::rx_metadata_t::ERROR_CODE_NONE){
            throw std::runtime_error(str(boost::format(
                    "Receiver error %s"
            ) % md.strerror()));
        }

        num_total_samps += num_rx_samps;
        for(int i = 0; i < num_rx_samps; i++){
            average += std::abs(*(buff_ptrs[0] + i));
        }

        signal_file.write((const char*) buff_ptrs[0], num_rx_samps*sizeof(samp_type));
    }
    std::string avg_str = (boost::format("%f\n") % (average/num_total_samps)).str();
    average_file.write((const char *) &avg_str.front(), avg_str.length() * sizeof(char));
    // Shut down receiver
    stream_cmd.stream_mode = uhd::stream_cmd_t::STREAM_MODE_STOP_CONTINUOUS;
    rx_stream->issue_stream_cmd(stream_cmd);

    // Close files
}


/***********************************************************************
 * Main function
 **********************************************************************/
int UHD_SAFE_MAIN(int argc, char *argv[]){
    uhd::set_thread_priority_safe();

    //transmit variables to be set by po
    std::string tx_args, wave_type, tx_ant, tx_subdev, ref, otw, tx_channels;
    double tx_rate, tx_freq, tx_gain, wave_freq, tx_bw;
    float ampl;

    //receive variables to be set by po
    std::string rx_args, file, type, rx_ant, rx_subdev, rx_channels;
    size_t total_num_samps, spb;
    double rx_rate, rx_freq, rx_gain, rx_bw;
    double settling;

    //setup the program options
    po::options_description desc("Allowed options");
    desc.add_options()
            ("help", "help message")
            ("tx-args", po::value<std::string>(&tx_args)->default_value(""), "uhd transmit device address args")
            ("rx-args", po::value<std::string>(&rx_args)->default_value(""), "uhd receive device address args")
            ("file", po::value<std::string>(&file)->default_value("usrp_samples.dat"), "name of the file to write binary samples to")
            ("type", po::value<std::string>(&type)->default_value("short"), "sample type in file: double, float, or short")
            ("nsamps", po::value<size_t>(&total_num_samps)->default_value(0), "total number of samples to receive")
            ("settling", po::value<double>(&settling)->default_value(double(0.2)), "settling time (seconds) before receiving")
            ("spb", po::value<size_t>(&spb)->default_value(0), "samples per buffer, 0 for default")
            ("tx-rate", po::value<double>(&tx_rate), "rate of transmit outgoing samples")
            ("rx-rate", po::value<double>(&rx_rate), "rate of receive incoming samples")
            ("tx-freq", po::value<double>(&tx_freq), "transmit RF center frequency in Hz")
            ("rx-freq", po::value<double>(&rx_freq), "receive RF center frequency in Hz")
            ("ampl", po::value<float>(&ampl)->default_value(float(0.3)), "amplitude of the waveform [0 to 0.7]")
            ("tx-gain", po::value<double>(&tx_gain), "gain for the transmit RF chain")
            ("rx-gain", po::value<double>(&rx_gain), "gain for the receive RF chain")
            ("tx-ant", po::value<std::string>(&tx_ant), "transmit antenna selection")
            ("rx-ant", po::value<std::string>(&rx_ant), "receive antenna selection")
            ("tx-subdev", po::value<std::string>(&tx_subdev), "transmit subdevice specification")
            ("rx-subdev", po::value<std::string>(&rx_subdev), "receive subdevice specification")
            ("tx-bw", po::value<double>(&tx_bw), "analog transmit filter bandwidth in Hz")
            ("rx-bw", po::value<double>(&rx_bw), "analog receive filter bandwidth in Hz")
            ("wave-type", po::value<std::string>(&wave_type)->default_value("CONST"), "waveform type (CONST, SQUARE, RAMP, SINE)")
            ("wave-freq", po::value<double>(&wave_freq)->default_value(0), "waveform frequency in Hz")
            ("ref", po::value<std::string>(&ref)->default_value("internal"), "clock reference (internal, external, mimo)")
            ("otw", po::value<std::string>(&otw)->default_value("sc16"), "specify the over-the-wire sample mode")
            ("tx-channels", po::value<std::string>(&tx_channels)->default_value("0"), "which TX channel(s) to use (specify \"0\", \"1\", \"0,1\", etc)")
            ("rx-channels", po::value<std::string>(&rx_channels)->default_value("0"), "which RX channel(s) to use (specify \"0\", \"1\", \"0,1\", etc)")
            ("tx-int-n", "tune USRP TX with integer-N tuning")
            ("rx-int-n", "tune USRP RX with integer-N tuning")
            ;
    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm);

    //print the help message
    if (vm.count("help")){
        std::cout << boost::format("UHD TXRX Loopback to File %s") % desc << std::endl;
        return ~0;
    }

    //create a usrp device
    std::cout << std::endl;
    std::cout << boost::format("Creating the transmit usrp device with: %s...") % tx_args << std::endl;
    uhd::usrp::multi_usrp::sptr tx_usrp = uhd::usrp::multi_usrp::make(tx_args);
    std::cout << std::endl;
    std::cout << boost::format("Creating the receive usrp device with: %s...") % rx_args << std::endl;
    uhd::usrp::multi_usrp::sptr rx_usrp = uhd::usrp::multi_usrp::make(rx_args);

    //always select the subdevice first, the channel mapping affects the other settings
    if (vm.count("tx-subdev")) tx_usrp->set_tx_subdev_spec(tx_subdev);
    if (vm.count("rx-subdev")) rx_usrp->set_rx_subdev_spec(rx_subdev);

    //detect which channels to use
    std::vector<std::string> tx_channel_strings;
    std::vector<size_t> tx_channel_nums;
    boost::split(tx_channel_strings, tx_channels, boost::is_any_of("\"',"));
    for(size_t ch = 0; ch < tx_channel_strings.size(); ch++){
        size_t chan = std::stoi(tx_channel_strings[ch]);
        if(chan >= tx_usrp->get_tx_num_channels()){
            throw std::runtime_error("Invalid TX channel(s) specified.");
        }
        else tx_channel_nums.push_back(std::stoi(tx_channel_strings[ch]));
    }
    std::vector<std::string> rx_channel_strings;
    std::vector<size_t> rx_channel_nums;
    boost::split(rx_channel_strings, rx_channels, boost::is_any_of("\"',"));
    for(size_t ch = 0; ch < rx_channel_strings.size(); ch++){
        size_t chan = std::stoi(rx_channel_strings[ch]);
        if(chan >= rx_usrp->get_rx_num_channels()){
            throw std::runtime_error("Invalid RX channel(s) specified.");
        }
        else rx_channel_nums.push_back(std::stoi(rx_channel_strings[ch]));
    }

    //Lock mboard clocks
    tx_usrp->set_clock_source(ref);
    rx_usrp->set_clock_source(ref);

    std::cout << boost::format("Using TX Device: %s") % tx_usrp->get_pp_string() << std::endl;
    std::cout << boost::format("Using RX Device: %s") % rx_usrp->get_pp_string() << std::endl;

    rx_usrp->set_rx_dc_offset(false);

    //set the transmit sample rate
    if (not vm.count("tx-rate")){
        std::cerr << "Please specify the transmit sample rate with --tx-rate" << std::endl;
        return ~0;
    }
    std::cout << boost::format("Setting TX Rate: %f Msps...") % (tx_rate/1e6) << std::endl;
    tx_usrp->set_tx_rate(tx_rate);
    std::cout << boost::format("Actual TX Rate: %f Msps...") % (tx_usrp->get_tx_rate()/1e6) << std::endl << std::endl;

    //set the receive sample rate
    if (not vm.count("rx-rate")){
        std::cerr << "Please specify the sample rate with --rx-rate" << std::endl;
        return ~0;
    }
    std::cout << boost::format("Setting RX Rate: %f Msps...") % (rx_rate/1e6) << std::endl;
    rx_usrp->set_rx_rate(rx_rate);
    std::cout << boost::format("Actual RX Rate: %f Msps...") % (rx_usrp->get_rx_rate()/1e6) << std::endl << std::endl;

    //set the transmit center frequency
    if (not vm.count("tx-freq")){
        std::cerr << "Please specify the transmit center frequency with --tx-freq" << std::endl;
        return ~0;
    }

    for(size_t ch = 0; ch < tx_channel_nums.size(); ch++) {
        size_t channel = tx_channel_nums[ch];
        if (tx_channel_nums.size() > 1) {
            std::cout << "Configuring TX Channel " << channel << std::endl;
        }
        std::cout << boost::format("Setting TX Freq: %f MHz...") % (tx_freq/1e6) << std::endl;
        uhd::tune_request_t tx_tune_request(tx_freq);
        if(vm.count("tx-int-n")) tx_tune_request.args = uhd::device_addr_t("mode_n=integer");
        tx_usrp->set_tx_freq(tx_tune_request, channel);
        std::cout << boost::format("Actual TX Freq: %f MHz...") % (tx_usrp->get_tx_freq(channel)/1e6) << std::endl << std::endl;

        //set the rf gain
        if (vm.count("tx-gain")){
            std::cout << boost::format("Setting TX Gain: %f dB...") % tx_gain << std::endl;
            tx_usrp->set_tx_gain(tx_gain, channel);
            std::cout << boost::format("Actual TX Gain: %f dB...") % tx_usrp->get_tx_gain(channel) << std::endl << std::endl;
        }

        //set the analog frontend filter bandwidth
        if (vm.count("tx-bw")){
            std::cout << boost::format("Setting TX Bandwidth: %f MHz...") % tx_bw << std::endl;
            tx_usrp->set_tx_bandwidth(tx_bw, channel);
            std::cout << boost::format("Actual TX Bandwidth: %f MHz...") % tx_usrp->get_tx_bandwidth(channel) << std::endl << std::endl;
        }

        //set the antenna
        if (vm.count("tx-ant")) tx_usrp->set_tx_antenna(tx_ant, channel);
    }

    for(size_t ch = 0; ch < rx_channel_nums.size(); ch++) {
        size_t channel = rx_channel_nums[ch];
        if (rx_channel_nums.size() > 1) {
            std::cout << "Configuring RX Channel " << channel << std::endl;
        }

        //set the receive center frequency
        if (not vm.count("rx-freq")){
            std::cerr << "Please specify the center frequency with --rx-freq" << std::endl;
            return ~0;
        }
        std::cout << boost::format("Setting RX Freq: %f MHz...") % (rx_freq/1e6) << std::endl;
        uhd::tune_request_t rx_tune_request(rx_freq);
        if(vm.count("rx-int-n")) rx_tune_request.args = uhd::device_addr_t("mode_n=integer");
        rx_usrp->set_rx_freq(rx_tune_request, channel);
        std::cout << boost::format("Actual RX Freq: %f MHz...") % (rx_usrp->get_rx_freq(channel)/1e6) << std::endl << std::endl;

        //set the receive rf gain
        if (vm.count("rx-gain")){
            std::cout << boost::format("Setting RX Gain: %f dB...") % rx_gain << std::endl;
            rx_usrp->set_rx_gain(rx_gain, channel);
            std::cout << boost::format("Actual RX Gain: %f dB...") % rx_usrp->get_rx_gain(channel) << std::endl << std::endl;
        }

        //set the receive analog frontend filter bandwidth
        if (vm.count("rx-bw")){
            std::cout << boost::format("Setting RX Bandwidth: %f MHz...") % (rx_bw/1e6) << std::endl;
            rx_usrp->set_rx_bandwidth(rx_bw, channel);
            std::cout << boost::format("Actual RX Bandwidth: %f MHz...") % (rx_usrp->get_rx_bandwidth(channel)/1e6) << std::endl << std::endl;
        }
    }
    //set the receive antenna
    if (vm.count("ant")) rx_usrp->set_rx_antenna(rx_ant);

    //for the const wave, set the wave freq for small samples per period
    if (wave_freq == 0 and wave_type == "CONST"){
        wave_freq = tx_usrp->get_tx_rate()/2;
    }

    //error when the waveform is not possible to generate
    if (std::abs(wave_freq) > tx_usrp->get_tx_rate()/2){
        throw std::runtime_error("wave freq out of Nyquist zone");
    }
    if (tx_usrp->get_tx_rate()/std::abs(wave_freq) > wave_table_len/2){
        throw std::runtime_error("wave freq too small for table");
    }

    //pre-compute the waveform values
    const wave_table_class wave_table(wave_type, ampl);
    const size_t step = boost::math::iround(wave_freq/tx_usrp->get_tx_rate() * wave_table_len);
    size_t index = 0;

    //create a transmit streamer
    //linearly map channels (index0 = channel0, index1 = channel1, ...)
    uhd::stream_args_t stream_args("fc32", otw);
    stream_args.channels = tx_channel_nums;
    uhd::tx_streamer::sptr tx_stream = tx_usrp->get_tx_stream(stream_args);

    //allocate a buffer which we re-use for each channel
    if (spb == 0) spb = tx_stream->get_max_num_samps()*10;
    std::vector<std::complex<float> > buff(spb);
    int num_channels = tx_channel_nums.size();

    //setup the metadata flags
    uhd::tx_metadata_t md;
    md.start_of_burst = true;
    md.end_of_burst   = false;
    md.has_time_spec  = true;
    md.time_spec = uhd::time_spec_t(0.05); //give us 0.5 seconds to fill the tx buffers

    //Check Ref and LO Lock detect
    std::vector<std::string> tx_sensor_names, rx_sensor_names;
    tx_sensor_names = tx_usrp->get_tx_sensor_names(0);
    if (std::find(tx_sensor_names.begin(), tx_sensor_names.end(), "lo_locked") != tx_sensor_names.end()) {
        uhd::sensor_value_t lo_locked = tx_usrp->get_tx_sensor("lo_locked",0);
        std::cout << boost::format("Checking TX: %s ...") % lo_locked.to_pp_string() << std::endl;
        UHD_ASSERT_THROW(lo_locked.to_bool());
    }
    rx_sensor_names = rx_usrp->get_rx_sensor_names(0);
    if (std::find(rx_sensor_names.begin(), rx_sensor_names.end(), "lo_locked") != rx_sensor_names.end()) {
        uhd::sensor_value_t lo_locked = rx_usrp->get_rx_sensor("lo_locked",0);
        std::cout << boost::format("Checking RX: %s ...") % lo_locked.to_pp_string() << std::endl;
        UHD_ASSERT_THROW(lo_locked.to_bool());
    }

    tx_sensor_names = tx_usrp->get_mboard_sensor_names(0);
    if ((ref == "mimo") and (std::find(tx_sensor_names.begin(), tx_sensor_names.end(), "mimo_locked") != tx_sensor_names.end())) {
        uhd::sensor_value_t mimo_locked = tx_usrp->get_mboard_sensor("mimo_locked",0);
        std::cout << boost::format("Checking TX: %s ...") % mimo_locked.to_pp_string() << std::endl;
        UHD_ASSERT_THROW(mimo_locked.to_bool());
    }
    if ((ref == "external") and (std::find(tx_sensor_names.begin(), tx_sensor_names.end(), "ref_locked") != tx_sensor_names.end())) {
        uhd::sensor_value_t ref_locked = tx_usrp->get_mboard_sensor("ref_locked",0);
        std::cout << boost::format("Checking TX: %s ...") % ref_locked.to_pp_string() << std::endl;
        UHD_ASSERT_THROW(ref_locked.to_bool());
    }

    rx_sensor_names = rx_usrp->get_mboard_sensor_names(0);
    if ((ref == "mimo") and (std::find(rx_sensor_names.begin(), rx_sensor_names.end(), "mimo_locked") != rx_sensor_names.end())) {
        uhd::sensor_value_t mimo_locked = rx_usrp->get_mboard_sensor("mimo_locked",0);
        std::cout << boost::format("Checking RX: %s ...") % mimo_locked.to_pp_string() << std::endl;
        UHD_ASSERT_THROW(mimo_locked.to_bool());
    }
    if ((ref == "external") and (std::find(rx_sensor_names.begin(), rx_sensor_names.end(), "ref_locked") != rx_sensor_names.end())) {
        uhd::sensor_value_t ref_locked = rx_usrp->get_mboard_sensor("ref_locked",0);
        std::cout << boost::format("Checking RX: %s ...") % ref_locked.to_pp_string() << std::endl;
        UHD_ASSERT_THROW(ref_locked.to_bool());
    }

    if (total_num_samps == 0){
        std::signal(SIGINT, &sig_int_handler);
        std::cout << "Press Ctrl + C to stop streaming..." << std::endl;
    }

    std::ofstream signal_file;
    std::ofstream average_file;
    signal_file.open("sweep3.bin", std::ofstream::binary);
    average_file.open("average3.txt");

    for(int i = 0; i < 26; i++) {
        if(i != 0){
            tx_freq += 1e8;
            rx_freq += 1e8;
            //std::cout << boost::format("Setting RX Freq: %f MHz...") % (rx_freq/1e6) << std::endl;
            uhd::tune_request_t rx_tune_request(rx_freq);
            if(vm.count("rx-int-n")) rx_tune_request.args = uhd::device_addr_t("mode_n=integer");
            rx_usrp->set_rx_freq(rx_tune_request, 0);
            std::cout << boost::format("Actual RX Freq: %f MHz...") % (rx_usrp->get_rx_freq(0)/1e6) << std::endl;

            //std::cout << boost::format("Setting TX Freq: %f MHz...") % (tx_freq/1e6) << std::endl;
            uhd::tune_request_t tx_tune_request(tx_freq);
            if(vm.count("tx-int-n")) tx_tune_request.args = uhd::device_addr_t("mode_n=integer");
            tx_usrp->set_tx_freq(tx_tune_request, 0);
            std::cout << boost::format("Actual TX Freq: %f MHz...") % (tx_usrp->get_tx_freq(0)/1e6) << std::endl;

            //Check Ref and LO Lock detect
            std::vector<std::string> tx_sensor_names, rx_sensor_names;
            tx_sensor_names = tx_usrp->get_tx_sensor_names(0);
            if (std::find(tx_sensor_names.begin(), tx_sensor_names.end(), "lo_locked") != tx_sensor_names.end()) {
                uhd::sensor_value_t lo_locked = tx_usrp->get_tx_sensor("lo_locked",0);
                //std::cout << boost::format("Checking TX: %s ...") % lo_locked.to_pp_string() << std::endl;
                UHD_ASSERT_THROW(lo_locked.to_bool());
            }
            rx_sensor_names = rx_usrp->get_rx_sensor_names(0);
            if (std::find(rx_sensor_names.begin(), rx_sensor_names.end(), "lo_locked") != rx_sensor_names.end()) {
                uhd::sensor_value_t lo_locked = rx_usrp->get_rx_sensor("lo_locked",0);
                //std::cout << boost::format("Checking RX: %s ...") % lo_locked.to_pp_string() << std::endl;
                UHD_ASSERT_THROW(lo_locked.to_bool());
            }

            tx_sensor_names = tx_usrp->get_mboard_sensor_names(0);
            if ((ref == "mimo") and (std::find(tx_sensor_names.begin(), tx_sensor_names.end(), "mimo_locked") != tx_sensor_names.end())) {
                uhd::sensor_value_t mimo_locked = tx_usrp->get_mboard_sensor("mimo_locked",0);
                std::cout << boost::format("Checking TX: %s ...") % mimo_locked.to_pp_string() << std::endl;
                UHD_ASSERT_THROW(mimo_locked.to_bool());
            }
            if ((ref == "external") and (std::find(tx_sensor_names.begin(), tx_sensor_names.end(), "ref_locked") != tx_sensor_names.end())) {
                uhd::sensor_value_t ref_locked = tx_usrp->get_mboard_sensor("ref_locked",0);
                std::cout << boost::format("Checking TX: %s ...") % ref_locked.to_pp_string() << std::endl;
                UHD_ASSERT_THROW(ref_locked.to_bool());
            }

            rx_sensor_names = rx_usrp->get_mboard_sensor_names(0);
            if ((ref == "mimo") and (std::find(rx_sensor_names.begin(), rx_sensor_names.end(), "mimo_locked") != rx_sensor_names.end())) {
                uhd::sensor_value_t mimo_locked = rx_usrp->get_mboard_sensor("mimo_locked",0);
                std::cout << boost::format("Checking RX: %s ...") % mimo_locked.to_pp_string() << std::endl;
                UHD_ASSERT_THROW(mimo_locked.to_bool());
            }
            if ((ref == "external") and (std::find(rx_sensor_names.begin(), rx_sensor_names.end(), "ref_locked") != rx_sensor_names.end())) {
                uhd::sensor_value_t ref_locked = rx_usrp->get_mboard_sensor("ref_locked",0);
                std::cout << boost::format("Checking RX: %s ...") % ref_locked.to_pp_string() << std::endl;
                UHD_ASSERT_THROW(ref_locked.to_bool());
            }
        }

        //reset usrp time to prepare for transmit/receive
        std::cout << boost::format("Setting device timestamp to 0...") << std::endl;
        tx_usrp->set_time_now(uhd::time_spec_t(0.0));

        //start transmit worker thread
        boost::thread_group transmit_thread;
        transmit_thread.create_thread(
                boost::bind(&transmit_worker, buff, wave_table, tx_stream, md, step, index, num_channels));

        //recv to file
        if (type == "double")
            recv_to_file<std::complex<double> >(rx_usrp, "fc64", otw, signal_file, average_file, spb, total_num_samps, settling,
                                                rx_channel_nums);
        else if (type == "float")
            recv_to_file<std::complex<float> >(rx_usrp, "fc32", otw, signal_file, average_file, spb, total_num_samps, settling,
                                               rx_channel_nums);
        else if (type == "short")
            recv_to_file<std::complex<short> >(rx_usrp, "sc16", otw, signal_file, average_file, spb, total_num_samps, settling,
                                               rx_channel_nums);
        else {
            //clean up transmit worker
            stop_signal_called = true;
            transmit_thread.join_all();
            throw std::runtime_error("Unknown type " + type);
        }

        //clean up transmit worker
        stop_signal_called = true;
        transmit_thread.join_all();

        stop_signal_called = false;
    }
    signal_file.close();
    //finished
    std::cout << std::endl << "Done!" << std::endl << std::endl;
    return EXIT_SUCCESS;
}
