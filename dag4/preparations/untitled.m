% TTK4135 - Helicopter lab
% Hints/template for problem 2.
% Updated spring 2018, Andreas L. Fl�ten

%% Initialization and model definition
%init09; % Change this to the init file corresponding to your helicopter

% Discrete time system model. x = [lambda r p p_dot]'

A = [0 1 0 0 0 0; 
    0 0 -K_2 0 0 0; 
    0 0 0 1 0 0; 
    0 0 -K_1*K_pp -K_1*K_pd 0 0;
    0 0 0 0 0 1;
    0 0 0 0 -K_3*K_ep -K_3*K_ed
    ]; % Your A matrix for the continuous system
B = [0 0; 0 0; 0 0; K_1*K_pp 0; 0 0; 0 K_3*K_ep]; % Your B matrix for the continuous system
delta_t	= 0.25; % sampling time

A1 = eye(size(A)) + delta_t*A;
B1 = delta_t*B;
% Number of states and inputs
mx = size(A1,2); % Number of states (number of columns in A)
mu = size(B1,2); % Number of inputs(number of columns in B)

% Initial values
x1_0 = pi;                               % Lambda
x2_0 = 0;                               % r
x3_0 = 0;                               % p
x4_0 = 0;                               % p_dot
x5_0 = 0;
x6_0 = 0;
x0 = [x1_0 x2_0 x3_0 x4_0 x5_0 x6_0]';           % Initial values

% Time horizon and initialization
N  = 55;                                  % Time horizon for states
M  = N;                                 % Time horizon for inputs
z  = ones(N*mx+M*mu,1);                % Initialize z for the whole horizon
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

% Generate the matrix Q and the vector c (objecitve function weights in the QP problem) 
Q1 = zeros(mx,mx);                      % Weight on state
Q1(1,1) = 2;                         % Weight on state x1
Q1(2,2) = 0;                         % Weight on state x2
Q1(3,3) = 0;                         % Weight on state x3
Q1(4,4) = 0;                         % Weight on state x4
Q1(5,5) = 0;                         % Weight on state x5
Q1(6,6) = 0;                         % Weight on state x6

q1 = 1;
q2 = 1;
P1 = [q1 0
      0 q2];                                 % Weight on input
Q = gen_q(Q1,P1,N,M);                   % Generate Q, hint: gen_q
%c = zeros(size(Q,1),1);              % Generate c, this is the linear constant term in the QP

%% Generate system matrixes for linear model
Aeq = gen_aeq(A1,B1,N,mx,mu);           % Generate A, hint: gen_aeq
beq = [A1*x0; repmat(zeros(size(A1*x0)), N-1, 1)];             % Generate b
%% Solve QP problem with linear model
f = @(z) 1/2*z'*Q*z;
opt = optimoptions('fmincon','Algorithm','sqp','MaxFunEvals',60000);
tic
[z,lambda] = fmincon(f, z0, [],  [], Aeq, beq, vlb, vub, @constraints, opt); % hint: quadprog. Type 'doc quadprog' for more info 
t1=toc;

% Calculate objective value
phi1 = 0.0;
PhiOut = zeros(N*mx+M*mu,1);
for i=1:N*mx+M*mu
  phi1=phi1+Q(i,i)*z(i)*z(i);
  PhiOut(i) = phi1;
end

%% Extract control inputs and states
u1 = [z(N*mx+1:mu:N*mx+M*mu);z(N*mx+M*mu-1)];           % Control input 1 from solution
u2 = [z(N*mx+2:mu:N*mx+M*mu);z(N*mx+M*mu)];             % Control input 2 from solution
x1 = [x0(1);z(1:mx:N*mx)];              % State x1 from solution
x2 = [x0(2);z(2:mx:N*mx)];              % State x2 from solution
x3 = [x0(3);z(3:mx:N*mx)];              % State x3 from solution
x4 = [x0(4);z(4:mx:N*mx)];              % State x4 from solution
x5 = [x0(5);z(5:mx:N*mx)];              % State x5 from solution
x6 = [x0(6);z(6:mx:N*mx)];              % State x6 from solution

num_variables = 5/delta_t;
zero_padding = zeros(num_variables,1);
unit_padding  = ones(num_variables,1);

u1   = [zero_padding; u1; zero_padding];
u2   = [zero_padding; u2; zero_padding];
x1  = [pi*unit_padding; x1; zero_padding];
x2  = [zero_padding; x2; zero_padding];
x3  = [zero_padding; x3; zero_padding];
x4  = [zero_padding; x4; zero_padding];
x5  = [zero_padding; x5; zero_padding];
x6  = [zero_padding; x6; zero_padding];

%% Plotting
t = 0:delta_t:delta_t*(length(u1)-1);

figure(2)
subplot(811)
stairs(t,u1),grid
ylabel('u1')
subplot(812)
stairs(t,u2),grid
ylabel('u2')
subplot(813)
plot(t,x1,'m',t,x1,'mo'),grid
ylabel('lambda')
subplot(814)
plot(t,x2,'m',t,x2','mo'),grid
ylabel('r')
subplot(815)
plot(t,x3,'m',t,x3,'mo'),grid
ylabel('p')
subplot(816)
plot(t,x4,'m',t,x4','mo'),grid
ylabel('pdot')
subplot(817)
plot(t,x5,'m',t,x5','mo'),grid
ylabel('e')
subplot(818)
plot(t,x6,'m',t,x6','mo'),grid
xlabel('tid (s)'),ylabel('edot')