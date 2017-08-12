function [ r ] = houghCurves( theta, x, y )
% Returns r as a function of theta
r = x.*cos(theta) + y.*sin(theta);

end

