%Output are 50*26*6*6 matrices on the form num_run*freq*tx_gain*rx_gain
%With amplitude data, error terms, amplitude from i and q channel    
function [amp, name_vec] = read_files(attenuation)

    num_run = 50;
    
    min_freq = 0.5;
    max_freq = 3.0;
    freq_step = 0.1;
    num_freq = (max_freq - min_freq)/freq_step + 1;

    min_tx_gain = 0; 
    switch attenuation
       case {0, 10}
           max_tx_gain = 0.6;
       case 20
           max_tx_gain = 0.8;
       case {30, 50, 70, 999}
           max_tx_gain = 1.0;
       otherwise
           fprintf(['Requested attenuation:  %.1f \n'...
               'Allowed values: 0,10,20,30,50,70, 999(cross-talk) \n'], attenuation);
           return
    end
    tx_step = 0.2;
    num_tx_gain = round((max_tx_gain - min_tx_gain)/tx_step + 1);

    min_rx_gain = 0;
    max_rx_gain = 1;
    rx_step = 0.2;
    num_rx_gain = (max_rx_gain - min_rx_gain)/rx_step +1;

    %if we want to save all samples in a matrix
    %ettus_data = zeros(num_samples, num_freq, num_tx_gain, num_rx_gain);
  
    amp = zeros(num_run, num_freq, num_tx_gain, num_rx_gain);
    name_vec = cell(num_run, num_freq, num_tx_gain, num_rx_gain);
    %eA = zeros(num_freq, num_tx_gain, num_rx_gain);
    %e0 = zeros(num_freq, num_tx_gain, num_rx_gain);
    
    %specify path to folder
    path = sprintf('200MHz_BW/%.0f/', attenuation);
    for  run = 0:(num_run  - 1)
        run_ind = run + 1;
        for tx_gain = min_tx_gain:tx_step:max_tx_gain
            tx_ind = (tx_gain - min_tx_gain)/tx_step + 1;

            for rx_gain = min_rx_gain:rx_step:max_rx_gain
                rx_ind = (rx_gain - min_rx_gain)/rx_step + 1;
                %Read dat_file
%                 name = ['run' num2str(run) '_' 'tx' num2str(10*tx_gain, '%.0f') '_rx' num2str(10*rx_gain, '%.0f')];
%                 
%                 fileName = [path num2str(run) '/' num2str(tx_gain, '%.1f') '/'...
%                 num2str(rx_gain, '%.1f') '/' 'data.dat'];
%                 fileID = fopen(fileName, 'r');
%                 fmt = ('%f %f %f %f %f %f'); 
%                 dat_file.(name)(1,1:6) = cell2mat(textscan(fileID, fmt , 1, 'HeaderLines', 1));
%                 fclose(fileID);
%                 fileID = fopen(fileName, 'r');
%                 fmt = ('%f %f %f %f %f');
%                 dat_file.(name)(2:27,1:5) = cell2mat(textscan(fileID, fmt, 26, 'HeaderLines', 4));
%                 fclose(fileID);
                 
                 
                 
                for freq = min_freq:freq_step:max_freq
                    freq_ind = round((freq - min_freq)/freq_step +1);
                    
                    str = sprintf('run: %.0f tx_gain: %.1f rx_gain: %.1f freq: %.1f',...
                        run, tx_gain, rx_gain, freq);
                    
                    name_str = sprintf('%.0f/%.0f/%.1f/%.1f/%.1f',...
                        attenuation, run, tx_gain, rx_gain, freq);
                    name_vec{run_ind, freq_ind, tx_ind, rx_ind} = name_str;
                    fprintf(str)

                    fileName = [path num2str(run) '/' num2str(tx_gain, '%.1f') '/'...
                        num2str(rx_gain, '%.1f') '/' num2str(freq, '%.1f')];
                    fileID = fopen(fileName, 'r');
                    raw_data = fread(fileID, 'float');
                    fclose(fileID);

                    %ettus_data(:,freq_ind, tx_ind, rx_ind) = i_channel + 1i*q_channel;

                    i_chan = raw_data(1:2:end);
                    q_chan = raw_data(2:2:end);

                    sig = i_chan + 1i*q_chan;
                    [i_filt, q_filt] = filter_usrp(sig);
                    
                    amp_temp  = iq_corr(i_filt, q_filt);
                    
                    %if (amp_temp < 0.3)
                        amp(run_ind, freq_ind, tx_ind, rx_ind) ...%eA(freq_ind, rx_ind, tx_ind),%e0(freq_ind, rx_ind, tx_ind)]...
                            = amp_temp;
                    %else
                        %amp(run_ind, freq_ind, tx_ind, rx_ind) = NaN;
                    %end
                    
                    back = '';
                    for i = 1:length(str)
                        back = strcat(back,'\b');
                    end
                    fprintf(back)
               end
            end
        end
    end
    fprintf('\nDone!\n');
end