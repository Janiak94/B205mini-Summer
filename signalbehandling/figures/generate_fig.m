function generate_fig(x,y,x_label,y_label,head_title)
    figure('rend','painters','pos',[10 10 700 300])
    plot(x,y,'Color','black','LineWidth',1)
    set(gca, 'FontSize', 16)
    xlabel(x_label,'FontSize',20)
    ylabel(y_label,'FontSize',20)
    %title(head_title,'FontSize',25)
end

