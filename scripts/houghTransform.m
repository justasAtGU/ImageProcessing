theta = linspace(0,2*pi);

close all
%plot(theta, 3), hold on

hough1 = plot(theta, houghCurves(theta, 1, 0)); hold on
hough2 = plot(theta, houghCurves(theta, 3, 10)); hold on
hough3 = plot(theta, houghCurves(theta, 4, 15)); hold on
hough4 = plot(theta, houghCurves(theta, 6, 14)); hold on
hough5 = plot(theta, houghCurves(theta, -1, -10)); hold on
set(hough1, 'linewidth', 1.5);
set(hough2, 'linewidth', 1.5);
set(hough3, 'linewidth', 1.5);
set(hough4, 'linewidth', 1.5);
set(hough5, 'linewidth', 1.5);


%plot settings
title('Hough Curves');
xl = xlabel('\theta');
yl = ylabel('r');
set(yl, 'fontsize', 14);
set(xl, 'fontsize', 14);
lgd = legend('hough1', 'hough2', 'hough3', 'hough4');
set(lgd, 'location', 'northeast');
grid MINOR;
