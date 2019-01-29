% function cubic_trajectory()
t = 0;
tf = 0.33;
matrix = [1 t t^2 t^3; 
          0 1 2*t 3*t^2;
          1 tf tf^2 tf^3;
          0 1 2*tf 3*tf^2];
B = [0.25;0;0.75;0];
x = matrix\B;
a3 = x(1);
a2 = x(2);
a1 = x(3);
a0 = x(4);

t = 4;
tf = 4.33;
matrix = [1 t t^2 t^3; 
          0 1 2*t 3*t^2;
          1 tf tf^2 tf^3;
          0 1 2*tf 3*tf^2];
B = [0.75;0;0.25;0];
x = matrix\B;
b3 = x(1);
b2 = x(2);
b1 = x(3);
b0 = x(4);

% t = 2;
% tf = 3;
% matrix = [1 t t^2 t^3; 
%           0 1 2*t 3*t^2;
%           1 tf tf^2 tf^3;
%           0 1 2*tf 3*tf^2];
% B = [0;0;0.5;0];
% x = matrix\B;
% c3 = x(1);
% c2 = x(2);
% c1 = x(3);
% c0 = x(4);
% 
% t = 3;
% tf = 4;
% matrix = [1 t t^2 t^3; 
%           0 1 2*t 3*t^2;
%           1 tf tf^2 tf^3;
%           0 1 2*tf 3*tf^2];
% B = [0.5;0;0;0];
% x = matrix\B;
% d3 = x(1);
% d2 = x(2);
% d1 = x(3);
% d0 = x(4);
%%
t = linspace(0,1,100);
t2 = linspace(1,2,100);
% t3 = linspace(2,3,100);
% t4 = linspace(3,4,100);

y1 = a0*t.^3 + a1*t.^2 + a2*t + a3;
yd1 = 3*a0*t.^2 + 2*a1*t + a2;
ydd1 = 6*a0*t +  2*a1;
figure(1)
plot(t,y1); hold on;
plot(t,yd1,'o'); hold on;
plot(t,ydd1,'^'); hold on;

y2 = b0*t2.^3 + b1*t2.^2 + b2*t2 + b3;
yd2 = 3*b0*t2.^2 + 2*b1*t2 + b2;
ydd2 = 6*b0*t2 +  2*b1;
plot(t2,y2); hold on;
plot(t2,yd2,'o'); hold on;
plot(t2,ydd2,'^');

% 
% y3 = c0*t3.^3 + c1*t3.^2 + c2*t3 + c3;
% yd3 = 3*c0*t3.^2 + 2*c1*t3 + c2;
% ydd3 = 6*c0*t3 +  2*c1;
% plot(t3,y3); hold on;
% plot(t3,yd3,'o'); hold on;
% plot(t3,ydd3,'^');
% 
% y4 = d0*t4.^3 + d1*t4.^2 + d2*t4 + d3;
% yd4 = 3*d0*t4.^2 + 2*d1*t4 + d2;
% ydd4 = 6*d0*t4 +  2*d1;
% plot(t4,y4); hold on;
% plot(t4,yd4,'o'); hold on;
% plot(t4,ydd4,'^');
% % end