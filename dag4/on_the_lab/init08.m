% Initialization for the helicopter assignment in TTK4135.
% Run this file before you execute QuaRC -> Build.

% Updated spring 2018, Andreas L. Flåten
% Updated Spring 2019, Joakim R. Andersen

clear all;
close all;
clc;

% The encoder for travel for Helicopter 2 is different from the rest.
travel_gain = 1; %
elevation_gain = 1; %

%% Physical constants
m_h = 0.4; % Total mass of the motors.
m_g = 0.03; % Effective mass of the helicopter.
l_a = 0.65; % Distance from elevation axis to helicopter body
l_h = 0.17; % Distance from pitch axis to motor

% Moments of inertia
J_e = 2 * m_h * l_a *l_a;         % Moment of interia for elevation
J_p = 2 * ( m_h/2 * l_h * l_h);   % Moment of interia for pitch
J_t = 2 * m_h * l_a *l_a;         % Moment of interia for travel

% Identified voltage sum and difference
V_s_eq = 7.55;%6.6; % Identified equilibrium voltage sum.
V_d_eq = 0.1;%0.5 Identified equilibrium voltage difference.

% Model parameters
K_p = m_g*9.81; % Force to lift the helicopter from the ground.
K_f = K_p/V_s_eq; % Force motor constant.
K_1 = l_h*K_f/J_p;
K_2 = K_p*l_a/J_t;
K_3 = K_f*l_a/J_e;
K_4 = K_p*l_a/J_e;

%% Pitch closed loop syntesis
% Controller parameters
w_p = 1.8; % Pitch controller bandwidth.
d_p = 1.0; % Pitch controller rel. damping.
K_pp = w_p^2/K_1;
K_pd = 2*d_p*sqrt(K_pp/K_1);
Vd_ff = V_d_eq;

% Closed loop transfer functions
Vd_max = 10 - V_s_eq; % Maximum voltage difference
deg2rad = @(x) x*pi/180;
Rp_max = deg2rad(15); % Maximum reference step
s = tf('s');
G_p = K_1/(s^2);
C_p = K_pp + K_pd*s/(1+0.1*w_p*s);
L_p = G_p*C_p;
S_p = (1 + L_p)^(-1);

plot_pitch_response = 0;
if plot_pitch_response
    figure()
    step(S_p*Rp_max); hold on;
    step(C_p*S_p*Rp_max/Vd_max);
    legend('norm error', 'norm input')
    title('Pitch closed loop response')
end

%% Elevation closed loop analysis
% Controller parameters
w_e = 0.5; % Elevation controller bandwidth.
d_e = 1.0; % Elevation controller rel. damping.
K_ep = w_e^2/K_3;
K_ed = 2*d_e*sqrt(K_ep/K_3);
K_ei = K_ep*0.1;
Vs_ff = V_s_eq;

% Closed loop transfer functions
Vs_max = 10 - V_s_eq; % Maximum voltage sum
Re_max = deg2rad(10); % Maximum elevation step
G_e = K_3/(s^2);
C_e = K_ep + K_ed*s/(1+0.1*w_e*s) + K_ei/s;
L_e = G_e*C_e;
S_e = (1 + L_e)^(-1);

plot_elev_response = 0;
if plot_elev_response
    figure()
    step(S_e*Re_max);
    hold on;
    step(C_e*S_e*Re_max/Vs_max);
    legend('norm error', 'norm input')
    title('Elevation closed loop response')
end

%% Task 1 + 2
% Rewriting system with two extra states, e and e_dot and discretizing with
% forward euler




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

q1 = 0.1; q2 = q1;
alpha = 0.2;    beta = 20;
lambda_0 = pi;  lambda_f = 0;   lambda_t = 2*pi/3;
time_padding = 5; sim_t = 10;

% Time horizon and initialization
N  = 55;                                  % Time horizon for states
M  = N;                                 % Time horizon for inputs
n = N*mx+M*mu;
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

x0 = [lambda_0 0 0 0 0 0]';               % Initial values
x_f = [lambda_f 0 0 0 0 0]';            % Final values
x = zeros(N*6,1);
x(1:6) = x0;
Q1 = 2*diag([1 0 0 0 0 0]);
P1 = 2*diag([q1 q2]);

Q = gen_q(Q1,P1,N,M);                                  % Generate Q, hint: gen_q                               % Generate c, this is the linear constant term in the QP

%% Generate system matrixes for linear model
Aeq = gen_aeq(A,B,N,mx,mu);             % Generate A, hint: gen_aeq
beq = [A*x0; repmat(zeros(size(A*x0)),N-1,1)]; % Generate b

%% Solve QP problem with linear model
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

%%Caclulate LQR gain

Q = diag([1 1 1 1 1 1]);
R = diag([0.1 0.1]);

[K,P,e] = dlqr(A,B,Q,R,[]);

%% Extract control inputs and states
% Extract control inputs and states
u1 = [Z(N*mx+1:mu:n);Z(n-1)];           % Control input 1 from solution
u2 = [Z(N*mx+2:mu:n);Z(n)];             % Control input 2 from solution
x1 = [x0(1);Z(1:mx:N*mx)];              % State x1 from solution
x2 = [x0(2);Z(2:mx:N*mx)];              % State x2 from solution
x3 = [x0(3);Z(3:mx:N*mx)];              % State x3 from solution
x4 = [x0(4);Z(4:mx:N*mx)];              % State x4 from solution
x5 = [x0(5);Z(5:mx:N*mx)];              % State x5 from solution
x6 = [x0(6);Z(6:mx:N*mx)];              % State x6 from solution

num_variables = 5/delta_t;
zero_padding = zeros(num_variables,1);
unit_padding  = ones(num_variables,1);
 
u1  = [zero_padding; u1; zero_padding];
u2  = [zero_padding; u2; zero_padding];
x1  = [lambda_0*unit_padding; x1; zero_padding];
x2  = [zero_padding; x2; zero_padding];
x3  = [zero_padding; x3; zero_padding];
x4  = [zero_padding; x4; zero_padding];
x5  = [zero_padding; x5; zero_padding];
x6  = [zero_padding; x6; zero_padding];

t_u = 0:delta_t:delta_t*(length(u1)-1);
t_x = 0:delta_t:delta_t*(length(x1)-1);

u1_simulink = timeseries(u1,t_u);
u2_simulink = timeseries(u2,t_u);
u = [u1'; u2'];
u_simulink = timeseries(u, t_u);

x = [x1'; x2'; x3'; x4'; x5'; x6'];
x_simulink = timeseries(x,t_x);

%% Plot Optimal Trajectory
t = 0:delta_t:delta_t*(length(u)-1);

figure(2)
subplot(711)
stairs(t,u'),grid
ylabel('u')

subplot(712)
plot(t,x1,'m',t,x1,'mo'),grid
ylabel('lambda')

subplot(713)
plot(t,x2,'m',t,x2','mo'),grid
ylabel('r')

subplot(714)
plot(t,x3,'m',t,x3,'mo'),grid
ylabel('p')

subplot(715)
plot(t,x4,'m',t,x4','mo'),grid
ylabel('pdot')

subplot(716)
plot(t,x5,'m',t,x5','mo'),grid
ylabel('e')

subplot(717)
plot(t,x6,'m',t,x6','mo'),grid
xlabel('tid (s)'),ylabel('edot')


%% Simulation and data saving
lab_day = 4; %For example 1,2,3,4
test_condition = 'N_equals_55_unit_tune'; %Descpritive of what we're doing

% --- Don't change this ---
model_name = 'helicopter'; 

set_param(model_name, 'SimulationCommand', 'start')
while ~strcmp('stopped', get_param(model_name, 'SimulationStatus'))
    pause(0.5);
end

file_name=sprintf('LabDay%d_%s.mat',lab_day, test_condition);
save(file_name, 'time', 'travel', 'travel_rate', 'pitch', 'pitch_rate', 'elevation', 'elevation_rate', 'pitch_reference', 'elevation_reference', 'V_d', 'V_s', 'x_star');



