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

%% Simulation and data saving
lab_day = 1; %For example 1,2,3,4
test_condition = 'low_kp'; %Descpritive of what we're doing
model_name = 'helicopter';

set_param(model_name, 'SimulationCommand', 'start')
while ~strcmp('stopped', get_param(model_name, 'SimulationStatus'))
    pause(0.5);
end

file_name=sprintf('LabDay%d_%s.mat',lab_day, test_condition);
save(file_name, 'time', 'travel', 'travel_rate', 'pitch', 'pitch_rate', 'elevation', 'elevation_rate');

%% Plotting data
% plot_file = file_name;
% time = plot_file(:,1);
% travel = plot_file(:,2);
% travel_rate = plot_file(:,3);
% % pitch = plot_file.pitch;
% % pitch_rate = plot_file.pitch_rate;
% % elevation = plot_file.elevation;
% % elevation_rate = plot_file.elevation_rate;
% 
% figure;
% subplot(2,1,1); 
% plot(time,travel)
% xlabel('Time')
% ylabel('Travel')
% title('Travel vs Time')
% 
% subplot(2,1,2); 
% plot(time,travel_rate)
% xlabel('Time')
% ylabel('Travel Rate')
% title('Travel Rate vs Time')
