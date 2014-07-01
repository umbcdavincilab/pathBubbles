function [c0, c1, A, err] = fit_sine_series(x, y, w, N)

% Fit a linear trend and a sine series (order N) to the data described by [x,y]
%
% the function fit is c0 + c1*x + A(1)*sin(pi*x) + A(2)*sin(2*pi*x) + ....
%
% Kenny Breuer, March 2009

M = length(x);
if (length(x) ~= length(y) | length(x) ~= length(w))
    disp('ERROR: length of x, y, and w must be the same');
    return
end

if (max(x) > 1.0 | min(x) < 0.0)
    disp('ERROR: X values must lie between [0-1]');
    return
end

if (N > M+2)
    disp('ERROR: The order of the sine series (N) cannot be larger than M-2, where M is the number of data points');
    return
end

% Fill in the matrix, use the weight function to determine which data points are the most important ones
for i = 1:M, 
  for j = 1:N,
    AA(j,i) = w(i) * sin(j*pi*x(i));
  end
  AA(N+1,i) = 1;
  AA(N+2,i) = w(i) * x(i);
  BB(i) = w(i) * y(i);
end

%% Solve the linear system
C =  AA'\BB';

c0 = C(N+1);
c1 = C(N+2);
A  = C(1:N);
err = std(AA'*C - BB');

return
end

