function [c,ceq] = constraints(z)
    alpha = 0.2;
    beta = 20;
    lambda_t = 2*pi/3;
    mx = 6;
    N = 60;
    c = alpha*exp(-beta*(z(1:mx:N*mx)-lambda_t).^2) - z(5:mx:N*mx);
    ceq = [];
end