% TTK4135 - Helicopter lab
% Hints/template for problem 2.
% Updated spring 2018, Andreas L. Fl�ten

%run('init08.m')   % closes all figures, clears all and clc + init parameters

%% Initialization and model definition
%init_simulator; % Change this to the init file corresponding to your helicopter

% Discrete time system model. x = [lambda r p p_dot e e_dot]'
delta_t	= 0.25; % sampling time

A = [ 1 delta_t 0                 0                  0                 0;
      0 1       -delta_t*K_2      0                  0                 0;
      0 0       1                 delta_t            0                 0;
      0 0       -delta_t*K_1*K_pp 1-delta_t*K_1*K_pd 0                 0;
      0 0       0                 0                  1                 delta_t;
      0 0       0                 0                  -delta_t*K_3*K_ep 1-delta_t*K_3*K_ed];

B = [ 0                0;
      0                0;
      0                0;
      delta_t*K_1*K_pp 0;
      0                0;
      0                delta_t*K_3*K_ep];

% Number of states and inputs
mx = size(A,2); % Number of states (number of columns in A)
mu = size(B,2); % Number of inputs(number of columns in B)

q1 = 1; q2 = q1;
alpha = 0.2;    beta = 20;
lambda_0 = pi;  lambda_f = 0;   lambda_t = 2*pi/3;
time_padding = 5; sim_t = 10;

% Time horizon and initialization
N  = 40;                                  % Time horizon for states
M  = N;                                 % Time horizon for inputs
n = N*mx+M*mu;
z  = zeros(N*mx+M*mu,1);                % Initialize z for the whole horizon
z0 = z;                                 % Initial value for optimization

% Bounds
ul 	    = -60*pi/360;                   % Lower bound on control
uu 	    = 60*pi/360;                   % Upper bound on control

xl      = -Inf*ones(mx,1);              % Lower bound on states (no bound)
xu      = Inf*ones(mx,1);               % Upper bound on states (no bound)
xl(3)   = ul;                           % Lower bound on state x3
xu(3)   = uu;                           % Upper bound on state x3

% Generate constraints on measurements and inputs
[vlb,vub]       = gen_constraints(N,M,xl,xu,ul,uu); % hint: gen_constraints
vlb(N*mx+M*mu)  = 0;                    % We want the last input to be zero
vub(N*mx+M*mu)  = 0;                    % We want the last input to be zero

x0 = [lambda_0 0 0 0 0 0]';               % Initial values
x_f = [lambda_f 0 0 0 0 0]';            % Final values
x = zeros(N*6,1);
x(1:6) = x0;
Q1 = 2*diag([1 0 0 0 0 0]);
P1 = 2*diag([q1 q2]);

Q = gen_q(Q1,P1,N,M);                                  % Generate Q, hint: gen_q
c = zeros(size(Q,1),1);                                  % Generate c, this is the linear constant term in the QP

%% Generate system matrixes for linear model
Aeq = gen_aeq(A,B,N,mx,mu);             % Generate A, hint: gen_aeq
beq = [A*x0; repmat(zeros(size(A*x0)),N-1,1)]; % Generate b

%% Solve QP with unlinear constraint
f = @(z) 1/2*z'*Q*z;
opt = optimoptions('fmincon','Algorithm','sqp','MaxFunEvals',40000);
tic
[Z, ZVAL, EXITFLAG] = fmincon(f, z0, [], [], Aeq, beq, ...
    vlb, vub, @constraints, opt); 
t1=toc;

% Calculate objective value
phi1 = 0.0;
PhiOut = zeros(N*mx+M*mu,1);
for i=1:N*mx+M*mu
  phi1=phi1+Q(i,i)*z(i)*z(i);
  PhiOut(i) = phi1;
end

% Extract control inputs and states
u1 = [Z(N*mx+1:mu:n);Z(n-1)];           % Control input 1 from solution
u2 = [Z(N*mx+2:mu:n);Z(n)];             % Control input 2 from solution

x1 = [x0(1);Z(1:mx:N*mx)];              % State x1 from solution
x2 = [x0(2);Z(2:mx:N*mx)];              % State x2 from solution
x3 = [x0(3);Z(3:mx:N*mx)];              % State x3 from solution
x4 = [x0(4);Z(4:mx:N*mx)];              % State x4 from solution
x5 = [x0(5);Z(5:mx:N*mx)];              % State x5 from solution
x6 = [x0(6);Z(6:mx:N*mx)];              % State x6 from solution

num_pads = time_padding/delta_t;
zero_padding = zeros(num_pads,1);
unit_padding  = ones(num_pads,1);

u1  = [zero_padding; u1; zero_padding];
u2  = [zero_padding; u2; zero_padding];
x1  = [lambda_0*unit_padding; x1; zero_padding];
x2  = [zero_padding; x2; zero_padding];
x3  = [zero_padding; x3; zero_padding];
x4  = [zero_padding; x4; zero_padding];
x5  = [zero_padding; x5; zero_padding];
x6  = [zero_padding; x6; zero_padding];

t = 0:delta_t:delta_t*(length(u1)-1);

figure(2)
subplot(611)
stairs(t,u1),grid
ylabel('u1')
subplot(612)
stairs(t,u2),grid
ylabel('u2')
subplot(613)
plot(t,x1,'m',t,x1,'mo'),grid
ylabel('lambda')
subplot(614)
plot(t,x2,'m',t,x2','mo'),grid
ylabel('r')
subplot(615)
plot(t,x3,'m',t,x3,'mo'),grid
ylabel('p')
subplot(616)
plot(t,x5,'m',t,x5','mo'),grid
xlabel('tid (s)'),ylabel('e')

