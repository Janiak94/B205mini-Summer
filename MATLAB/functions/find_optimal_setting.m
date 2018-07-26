function [tx_i, rx_i] = find_optimal_setting(amp_data, amp_ref, attenuation)

    % load reference data
    load Data/s12.mat
    % Use 0dB as reference and minimize the error from actual attenuation
    % Calculate the mean from the 0dB data for reference
    switch attenuation
           case {0, 10}
               tx_length = 4;
           case 20
               tx_length = 5;
           case {30, 50, 70, 999}
               tx_length = 6;
           otherwise
               fprintf(['Requested attenuation:  %.1f \n'...
                   'Allowed values: 0,10,20,30,50,70, 999(cross-talk) \n'], attenuation);
               return
    end

    amp_0dB_norm = zeros(50, 26, 4, 6);
    amp_data_norm = zeros(50, 26, tx_length, 6);
    
    % Convert to dB 
    for rx=1:6
        amp_0dB_norm(:,:,:,rx) = 20*log10(amp_ref(:,:,:,rx)); 
    end
    
    for rx=1:6
        amp_data_norm(:,:,:,rx) = 20*log10(amp_data(:,:,:,rx))-amp_0dB_norm(:,:,:,rx);
    end

    % amp matricies: 50*26*6*6: run*freq*tx*rx
    % create reference matrix
    refMat = repmat((s12(:,3)-s12(:,2))',size(amp_0dB_norm)-[0 25 0 0]);
    errorMat = abs(amp_data_norm-refMat);
    % Calculate the sum of the errors for each tx and rx setting and divide by number of
    % terms the get a rough estimate of the error compared to the VNA data
    sizeA = size(errorMat);
    % Setting the value to something large so the omitted values are not
    % present
    err = zeros(tx_length, 6);
    for tx=1:tx_length
        for rx=1:6
            count = 0;
            for freq=1:sizeA(2)
                for run=1:50
                    err(tx,rx) = err(tx,rx) + errorMat(run,freq,tx,rx).^2;
                    count = count+1;
                end
            end
            err(tx,rx) = sqrt(err(tx,rx))/count;
        end
    end
    V = min(err(:))
    [tx_i,rx_i] = find(err==V);
    % TODO: We may need a better way of error estimation. This one may not be
    % optimal. Some settings seem to produce more regular pattern than the
    % found one
end