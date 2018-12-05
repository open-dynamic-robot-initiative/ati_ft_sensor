import os,sys
from pylab import *

data = loadtxt('log_file.dat')

t = data[:,0]/1000.0

figure()
for i in range(3):
    subplot(3,1,i+1)
    plot(t,data[:,4+i])
title('Force [N]')
figure()
for i in range(3):
    subplot(3,1,i+1)
    plot(t,data[:,7+i])
title('torque [Nm]')
figure()
subplot(4,1,1)
plot(data[:,0])
xlabel('sequence')
subplot(4,1,2)
plot(diff(data[:,0]))
xlabel('diff sequence')
subplot(4,1,3)
plot(data[:,2])
xlabel('status')
subplot(4,1,4)
plot(data[:,3])
xlabel('dt [s]')


show()