function [i_ch,q_ch] = filter_usrp(in)
    % filter measurement data to only keep main frequency and its harmonics
    % [i_ch, q_ch] = filter_usrp(in)
    % 'in' is complex data
    
     fft_signal = abs(fft(in));
     noisefloor = mean(fft_signal(5:end));
     signal = fft_signal(4);
    
    l = length(in);
    
     %if signal > 10*noisefloor
        in(1:30) = 0;
        t = (-l/2+1:l/2);
        x = (mod(t,4) == 0 & t > 3 & t < 5)';
        f = fftshift(fft(in)).*x;
        f = fftshift(f);
        f(1) = 0;
        sig = ifft(f);
        i_ch = real(sig);
        q_ch = imag(sig);
     %else
         %i_ch(1:l) = nan;
         %q_ch(1:l) = nan;
     %end
end