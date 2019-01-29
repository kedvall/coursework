w = 250;
sc = 3;
hold on;
idx = 1;
for x = -0.0115:0.0002*sc:0.0115
    b(idx) = x;
    y(idx) = ((w*x+1)^4-4*(w*x+1)^3-2*(w*x+1)^2+12*(w*x+1)-100)/4000;
    scatter(b(idx),y(idx));
    idx = idx+1;
end
cut = idx
for x = -0.0115:0.0002*sc:0.0115
    c = sqrt(0.0124^2-x^2)-0.018;
    if c > -0.02023
        b(idx) = x;
        y(idx) = real(sqrt(0.0115^2-x^2)-0.023);
        scatter(b(idx),y(idx));
        idx = idx+1;
    end
end
q = b(cut:78);
p = fliplr(q);
b(cut:78) = p;

plot(b,y)

xstr = num2str(x(1));
for i=2:1:78
    xstr = strcat(xstr,', ',num2str(b(i)));
end
ystr = num2str(y(1));
for i=2:1:78
    ystr = strcat(ystr,', ',num2str(y(i)));
end
disp(xstr);