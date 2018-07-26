%% Read in multiple .bin-files, with filter and iq-corr
[amp_999dB, name_vec_999dB] = read_files(999);

%% Read dat_file
dat_file_999dB  = read_files(999);

%% See I and Q channel from a single run

%To open file containing minimum or maximum amplitude 
%[V, I] = min(amp(:))
%name_vec(I)
%file = fopen(name_vec{I},'r');

file = fopen('200MHz_BW/0/40/0.0/0.4/2.5','r');
dat = fread(file,'float');

i_chan = dat(1:2:end);
q_chan = dat(2:2:end);
 
plot(q_chan)
%%

sig = i_chan + 1i*q_chan;

[i_filt, q_filt] = filter_usrp(sig);
plot(i_filt, q_filt)
axis equal
%amp_unfilt = 20*log10(iq_corr(i_chan, q_chan))
%amp_filt = 20*log10(iq_corr(i_filt, q_filt))

%% Plot magnitude plots for corrected RX-gain. Want to see straight lines for each TX-gain 
mu =zeros(4,6);
yneg = zeros(4,6);
ypos = zeros (4,6);

for freq = 0.5:0.5:3.0
    freq_ind = round((freq-0.5)/0.1 + 1);
    for tx = 1:4
        for rx = 1:6                 
            mu(tx,rx) = mean(20*log10(amp_999dB_unfiltered(:,freq_ind, tx, rx)));
            yneg(tx,rx) = mu(tx,rx) - min(20*log10(amp_999dB_unfiltered(:,freq_ind,tx,rx)),[], 1);
            ypos(tx,rx) = max(20*log10(amp_999dB_unfiltered(:,freq_ind,tx,rx)),[], 1) - mu(tx,rx); 
        end
    end
    figure(freq/0.5 + 6)
    x = [0, 15, 30, 45, 60, 76];
    tx_gain = [0, 17.75, 35.75, 53.75];
    tx_gain_cell = cell(1,4);
    for i = 1:4
        
        y = squeeze(mu(i,:));
        errorbar(x, y, yneg(i,:), ypos(i,:)) 
        grid on
        hold on
        xlabel('RX-gain dB')
        ylabel('Magnitude')
        title(sprintf('Attenuation: crosstalk freq: %.1f', freq))
        tx_gain_cell{i} = sprintf('%.2f', tx_gain(i));
    end
    legend(tx_gain_cell)
end

%% Magnitude-Frequency plot, with different gain-settings


rx = 4;
%tx = 2;

mu = zeros(1,26);
yneg = zeros(1,26);
ypos = zeros(1,26);

tx_gain_cell = cell(1,4);
tx_gain = [0, 17.75, 35.75, 53.75];
for tx = 1:4  % switch to attenuation when we have more data
    for freq = 0.5:0.1:3.0
        freq_ind = round(freq/0.1 - 4);
        mu(freq_ind) = mean(20*log10(amp_0dB(:,freq_ind, tx, rx)));
        yneg(freq_ind) = mu(freq_ind) - min(20*log10(amp_0dB(:,freq_ind,tx,rx)),[], 1);
        ypos(freq_ind) = max(20*log10(amp_0dB(:,freq_ind,tx,rx)),[], 1) - mu(freq_ind);    
    end
    freq = 0.5:0.1:3.0;
    errorbar(freq, mu, yneg, ypos)
    tx_gain_cell{tx} = sprintf('Tx-Gain: %.2f', tx_gain(tx));
    hold on
end
legend(tx_gain_cell)


%% Magnitude difference between attenuation settings

rx = 1;

freq_vec = 0.5:0.1:3;

mu_diff = zeros(1,26);

min_diff = zeros(1,26);
max_diff = zeros(1,26);
max_ind = zeros(4,26);
min_ind = zeros(4,26);
stdev  = zeros(4,26);

for tx = 1:4
    figure(tx)
    for freq = 1:1:26

        min_0dB = min(20*log10(amp_0dB(:, freq, tx, rx)), [],1);
        [max_0dB, max_ind(tx,freq)] = max(20*log10(amp_0dB(:, freq, tx, rx)), [],1);

        [min_10dB, min_ind(tx,freq)] = min(20*log10(amp(:, freq, tx, rx)), [],1);
        max_10dB = max(20*log10(amp(:, freq, tx, rx)), [],1);

        mu_diff(freq) = mean(20*log10(amp_0dB(:,freq, tx, rx))) - ...
            mean(20*log10(amp(:,freq, tx, rx)));

        min_diff(freq) = mu_diff(freq) - (min_0dB - max_10dB);
        max_diff(freq) = (max_0dB - min_10dB) - mu_diff(freq);
        
        stdev(tx,freq) = std(20*log10(amp(:, freq, tx, rx)));
    end
    errorbar(freq_vec, mu_diff, min_diff, max_diff)
    hold on
    plot(s12(:,1), s12(:,2)-s12(:,3))
    title(['TX-gain ' num2str(tx)])
    legend('b205mini', 'VNA')
    hold on
    grid on
end

%%
load Data/ampData_10dB.mat
load Data/ampData_0dB.mat
[tx_i, rx_i] = find_optimal_setting(amp_10dB, amp_0dB, 10);
freq = 0.5:0.1:3;

amp = 20*log10(amp_10dB) - 20*log10(amp_0dB);
for run = 1:50
    subplot(1,2,1)
    plot(freq, amp(run, :,tx_i,rx_i))
    axis([0.5 3 -13 -7])
    hold on
    subplot(1,2,2)
    plot(freq,amp(run, :, 3, 5),'*')
    axis([0.5 3 -13 -7])
    hold on
end