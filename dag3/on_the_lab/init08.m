% Initialization for the helicopter assignment in TTK4135.
% Run this file before you execute QuaRC -> Build.

% Updated spring 2018, Andreas L. Fl�ten
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

%% Task 2
% Optimal open loop control




% Discrete time system model. x = [lambda r p p_dot]'
delta_t	= 0.25; % sampling time

A1 = [1 delta_t 0                 0;
      0 1       -delta_t*K_2      0;
      0 0       1                 delta_t;
      0 0       -delta_t*K_1*K_pp 1-delta_t*K_1*K_pd];

B1 = [0;
      0;
      0;
      delta_t*K_1*K_pp];

% Number of states and inputs
mx = size(A1,2); % Number of states (number of columns in A)
mu = size(B1,2); % Number of inputs(number of columns in B)

% Initial values
x1_0 = pi;                               % Lambda
x2_0 = 0;                               % r
x3_0 = 0;                               % p
x4_0 = 0;                               % p_dot
x0 = [x1_0 x2_0 x3_0 x4_0]';           % Initial values

% Time horizon and initialization
N  = 100;                                  % Time horizon for states
M  = N;                                 % Time horizon for inputs
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

% Generate the matrix Q and the vector c (objecitve function weights in the QP problem) 
Q1 = zeros(mx,mx);
Q1(1,1) = 2;                            % Weight on state x1
Q1(2,2) = 0;                            % Weight on state x2
Q1(3,3) = 0;                            % Weight on state x3
Q1(4,4) = 0;                            % Weight on state x4

q = 1;
P1 = 2*q;                                % Weight on input
Q = gen_q(Q1,P1,N,M);                                  % Generate Q, hint: gen_q
c = zeros(size(Q,1),1);                                  % Generate c, this is the linear constant term in the QP

%% Generate system matrixes for linear model
Aeq = gen_aeq(A1,B1,N,mx,mu);             % Generate A, hint: gen_aeq
beq = [A1*x0; repmat(zeros(size(A1*x0)),N-1,1)]; % Generate b

%% Solve QP problem with linear model
tic
[z,lambda] = quadprog(Q,c,[],[],Aeq,beq,vlb,vub,x0); % hint: quadprog. Type 'doc quadprog' for more info 
t1=toc;

% Calculate objective value
phi1 = 0.0;
PhiOut = zeros(N*mx+M*mu,1);
for i=1:N*mx+M*mu
  phi1=phi1+Q(i,i)*z(i)*z(i);
  PhiOut(i) = phi1;
end

%%Caclulate LQR gain

% Unit tune
%Q = diag([1,1,1,1]);
%R = 1;

% Good tune
%Q = diag([3,2,0.1,0.5]);
%R = 0.1;

% Prioritize
Q = diag([15,1,1,1]);
R = 100;

[K,P,e] = dlqr(A1,B1,Q,R,[]);

%Matrix to go from 6 dimentional vector to 4 dimentioanl
Transform = [1 0 0 0 0 0;
             0 1 0 0 0 0;
             0 0 1 0 0 0;
             0 0 0 1 0 0];

%% Extract control inputs and states
u  = [z(N*mx+1:N*mx+M*mu);z(N*mx+M*mu)]; % Control input from solution

x1 = [x0(1);z(1:mx:N*mx)];              % State x1 from solution
x2 = [x0(2);z(2:mx:N*mx)];              % State x2 from solution
x3 = [x0(3);z(3:mx:N*mx)];              % State x3 from solution
x4 = [x0(4);z(4:mx:N*mx)];              % State x4 from solution

num_variables = 5/delta_t;
zero_padding = zeros(num_variables,1);
unit_padding  = ones(num_variables,1);
 
u   = [zero_padding; u; zero_padding];

x1  = [pi*unit_padding; x1; zero_padding];
x2  = [zero_padding; x2; zero_padding];
x3 = [zero_padding; x3; zero_padding];
x4  = [zero_padding; x4; zero_padding];

t_u = 0:delta_t:delta_t*(length(u)-1);
t_x = 0:delta_t:delta_t*(length(x1)-1);

u_simulink = timeseries(u,t_u);

x1_simulink = timeseries(x1,t_x);
x2_simulink = timeseries(x2,t_x);
x3_simulink = timeseries(x3,t_x);
x4_simulink = timeseries(x4,t_x);

%% Simulation and data saving
lab_day = 3; %For example 1,2,3,4
test_condition = 'pitch_disturbance_good_tune'; %Descpritive of what we're doing
model_name = 'helicopter'; %Don't change this

set_param(model_name, 'SimulationCommand', 'start')
while ~strcmp('stopped', get_param(model_name, 'SimulationStatus'))
    pause(0.5);
end

file_name=sprintf('LabDay%d_%s.mat',lab_day, test_condition);
%save(file_name, 'time', 'travel', 'travel_rate', 'pitch', 'pitch_rate', 'elevation', 'elevation_rate', 'pitch_reference', 'elevation_reference', 'V_d', 'V_s', 'x_star');



