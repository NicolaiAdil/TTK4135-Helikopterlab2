function [c,ceq] = constraints(z)
    alpha = 0.2;
    beta = 20;
    lambda_t = 2*pi/3;
    nx = 6;
    N = 40;
    c = zeros(N,1);
    for k=1:N
        c(k) = alpha*exp(-beta*(z(1+(k-1)*nx)-lambda_t)^2) - z(5+(k-1)*nx);
    end
    ceq = [];
end