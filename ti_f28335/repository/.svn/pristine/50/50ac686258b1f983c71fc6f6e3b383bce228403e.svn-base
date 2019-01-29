function filtersteptoC(ctf)
% format filtersteptoC(ctf)  ctf is the continuous transfer this C code will approximate
% this function generates C code that implements a discrete transfer funtion.    
% the C function created must be called every .001 seconds.  
%dtf = c2d(ctf,.001);
dtf = c2d(ctf,.001,'tustin');
b = dtf.num{1};
a = dtf.den{1};
numcoeffs = length(a);
t = 0:.001:4;
[stepy] = step(dtf,t);
figure(1);
plot(t,stepy);
title('qd');
dy = filter([1 -1],[.001 0],stepy);
figure(2);
plot(t,dy);
title('qd_dot');

ddy = filter([1 -1],[.001 0],dy);
figure(3);
plot(t,ddy);
title('qd_ddot');

fprintf(1,'typedef struct steptraj_s {\n');
fprintf(1,'	long double b[%d];\n',numcoeffs);
fprintf(1,'	long double a[%d];\n',numcoeffs);
fprintf(1,'    long double xk[%d];\n',numcoeffs);
fprintf(1,'    long double yk[%d];\n',numcoeffs);
fprintf(1,'    float qd_old;\n');
fprintf(1,'    float qddot_old;\n');
fprintf(1,'    int size;\n')
fprintf(1,'} steptraj_t;\n');
fprintf(1,'\n');
fprintf(1,'steptraj_t trajectory = {');
for i=1:numcoeffs
    fprintf(1,'%10.16eL,',b(i));
end
fprintf(1,'\n\t\t\t\t\t\t');
for i=1:numcoeffs
    fprintf(1,'%10.16eL,',a(i));
end
fprintf(1,'\n\t\t\t\t\t\t');
for i=1:numcoeffs
    fprintf(1,'0,');
end
fprintf(1,'\n\t\t\t\t\t\t');
for i=1:numcoeffs
    fprintf(1,'0,');
end
fprintf(1,'\n\t\t\t\t\t\t');
fprintf(1,'0,\n\t\t\t\t\t\t');
fprintf(1,'0,\n\t\t\t\t\t\t');
fprintf(1,'%d};\n',numcoeffs);
fprintf(1,'\n');
fprintf(1,'// this function must be called every 1ms.\n');
fprintf(1,'void implement_discrete_tf(steptraj_t *traj, float step, float *qd, float *qd_dot, float *qd_ddot) {\n');
fprintf(1,'	int i = 0;\n');
fprintf(1,'\n');
fprintf(1,'	traj->xk[0] = step;\n');
fprintf(1,'	traj->yk[0] = traj->b[0]*traj->xk[0];\n');
fprintf(1,'	for (i = 1;i<traj->size;i++) {\n');
fprintf(1,'		traj->yk[0] = traj->yk[0] + traj->b[i]*traj->xk[i] - traj->a[i]*traj->yk[i];\n');
fprintf(1,'	}\n');
fprintf(1,'\n');
fprintf(1,'	for (i = (traj->size-1);i>0;i--) {\n');
fprintf(1,'		traj->xk[i] = traj->xk[i-1];\n');
fprintf(1,'		traj->yk[i] = traj->yk[i-1];\n');
fprintf(1,'	}\n');
fprintf(1,'\n');
fprintf(1,'	*qd = traj->yk[0];\n');
fprintf(1,'	*qd_dot = (*qd - traj->qd_old)*1000;  //0.001 sample period\n');
fprintf(1,'	*qd_ddot = (*qd_dot - traj->qddot_old)*1000;\n');
fprintf(1,'\n');
fprintf(1,'	traj->qd_old = *qd;\n');
fprintf(1,'	traj->qddot_old = *qd_dot;\n');
fprintf(1,'}\n');
fprintf(1,'\n');
fprintf(1,'// to call this function create a variable that steps to the new positions you want to go to, pass this var to step\n');
fprintf(1,'// pass a reference to your qd variable your qd_dot variable and your qd_double_dot variable\n');
fprintf(1,'// for example\n');
fprintf(1,'//	implement_discrete_tf(&trajectory, mystep, &qd, &dot, &ddot);\n');

