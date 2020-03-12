a = 0:0.1:2*pi;
i=0;

while(1)
pause(0.01);
if i>(2*pi/0.1)
    i=0;
end
i=i+1
z = 10*sin(a(i))+20
y = 10*sin(2*a(i))
% pause;
GE420_serialwrite('x_target',20);
GE420_serialwrite('y_target',y);
GE420_serialwrite('z_target',z);
end
