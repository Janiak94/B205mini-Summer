num_samps = 2e4;
f(1:num_samps) = 0;
f(4) = num_samps;
signal = ifft(f);
i = real(signal);
q = imag(signal);
out(1:2:2*num_samps) = 0.5*i;
out(2:2:2*num_samps) = 0.5*q;
out_handler = fopen('test_signal.bin','w');
fwrite(out_handler, out,'float')

syms e0 eA A
[V,D] = eig([A*(1+eA)*cos(e0) -A*(1+eA)*sin(e0); -A*(1-eA)*sin(e0) A*(1-eA)*cos(e0)])