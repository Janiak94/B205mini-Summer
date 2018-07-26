function [Amp, eA, e0] = iq_corr(i_ch, q_ch)
    % correct for DC-offset
    
    if isnan(i_ch(1)) == 1
        Amp = nan;
        eA = nan;
        e0 = nan;
        return
    end
        
    %d1 = (max(i_ch)+min(i_ch))/2;
    %d2 = (max(q_ch)+min(q_ch))/2;
    x(1,:) = i_ch; %- d1;
    x(2,:) = q_ch; %- d2;
    % Find the ellipse of the data

    % Find the amplitude mismatch
    f = max(x(1,:))/max(x(2,:));
    eA = (f-1)/(f+1);
    Amp = max(x(1,:))/(1+eA);
    [A,~] = MinVolEllipse(x(:,1:100:end),0.01);
    [~,Q,~] = svd(A);
    rx = 1/sqrt(Q(1,1));
    ry = 1/sqrt(Q(2,2));
    e0 = 2*acos((rx+ry)/(2*Amp));
end