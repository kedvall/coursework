
% guessing here due to unknown gear ratio
Imotor2 = 0.00375;
Imotor2 = Imotor2*20;

Imotor3 = 0.00375;
Imotor3 = Imotor3*10;  %guess

% center of mass of link one
lc2 = 0.125;
% Total moment of Inertia of Link one about its center of mass
Il2 = 0.0114;
ml2 = 1.3;
L2 = 10*.0254; %meters

% Total mass of link two
ml3 = 1.14;
% Center of mass of link two
lc3 = 0.157;
% Total moment of Inertia of Link Two about its center of mass
Il3 = 0.0111;


%Mass of disks
Mw = 1.54;
%Center of mass of disks
Lw = .32;
%Moment of Intertia of Disks
Iw = (1/12)*1.54*(3*(.0508)^2+(.0254)^2)

%Link 3 Center with disks attached
lc3T = (ml3*lc3 + Mw*Lw)/(ml3+Mw);


% generate the five parameters
pars(1,1) = (ml2*lc2^2 + (ml3+Mw)*L2^2 + Il2) + Imotor2;
pars(2,1) = ((ml3+Mw)*lc3T^2 + Il3 + ml3*(lc3T-lc3)^2 + Iw + (Lw - lc3T)^2) + Imotor3;
pars(3,1) = ((ml3+Mw)*L2*lc3T);
pars(4,1) = (ml2*lc2 + (ml3+Mw)*L2);
pars(5,1) = (ml3*lc3T + Mw*Lw);  %Mike had (m3*lc3T + Mw*Lw) 

% add the Torque Constant to the parameters
TorqueConst = 6.0; %N-m/UnitIN
pars = pars/TorqueConst

%for FeedForward Control
%big guess here
J1 = 0.1/TorqueConst;
J2 = (Imotor2+ml2*lc2^2+ml3*L2^2+Il2)/TorqueConst;
J3 = (Imotor3+Il3+ml3*lc3T^2)/TorqueConst;
