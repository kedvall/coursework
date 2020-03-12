
for t = linspace(0,10,100);

x = cos(t);
y = sin(2*t);
z = 6;

inv(i) = inversekinematics(x,y,z)
end