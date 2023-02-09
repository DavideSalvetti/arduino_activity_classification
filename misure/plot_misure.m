%exampledata_filtr = exampledata - mean(exampledata);
%exampledata_filtr = movmean(exampledata_filtr,10,2);


exampledata_filtr = lowpass(exampledata,20,1000/15);
%exampledata_filtr = highpass(exampledata_filtr,0.5,1000/15);
exampledata_filtr(:,9) = ((exampledata(:,9) - exampledata(1,9))./1000)-5;

figure
sgtitle('IMU data 20Hz LPF')
subplot(2,3,1)
plot(exampledata_filtr(:,9),exampledata_filtr(:,1),'Color','red');
xlim([0 10])
xlabel('Time [s]')
ylabel('Magnitudo [g]')
legend('accX','Location','south')
subplot(2,3,2)
plot(exampledata_filtr(:,9),exampledata_filtr(:,2),'Color','green');
xlim([0 10])
xlabel('Time [s]')
ylabel('Magnitudo [g]')
legend('accY','Location','south')
subplot(2,3,3)
plot(exampledata_filtr(:,9),exampledata_filtr(:,3),'Color','blu');
xlim([0 10])
xlabel('Time [s]')
ylabel('Magnitudo [g]')
legend('accZ','Location','south')

subplot(2,3,4)
plot(exampledata_filtr(:,9),exampledata_filtr(:,4));
xlim([0 10])
xlabel('Time [s]')
ylabel('Magnitudo [dps]')
legend('gyroX','Location','south')
subplot(2,3,5)
plot(exampledata_filtr(:,9),exampledata_filtr(:,5));
xlim([0 10])
xlabel('Time [s]')
ylabel('Magnitudo [dps]')
legend('gyroY','Location','south')
subplot(2,3,6)
plot(exampledata_filtr(:,9),exampledata_filtr(:,6));
xlim([0 10])
xlabel('Time [s]')
ylabel('Magnitudo [dps]')
legend('gyroZ','Location','south')

figure
d_1 = diff(exampledata(:,9));
plot(d_1,'o');
ylim([13 31])
xlim([0 5067])
xlabel('sample')
ylabel('Time [ms]')
title('Acquisition interval between two samples')

m_1=mean(d_1)
s_1 = std(d_1)

figure
d_2 = diff(Untitled(:,9));
plot(d_2,'o');
ylim([13 140])
xlim([0 5067])
xlabel('sample')
ylabel('Time [ms]')
title('Acquisition interval between two samples')

m_2=mean(d_2)
s_2 = std(d_2)


figure 
hold on
plot(exampledata(:,9),exampledata(:,1));
plot(exampledata(:,9),exampledata(:,2));
plot(exampledata(:,9),exampledata(:,3));
hold off
legend('accX', 'accY', 'accZ')
xlim([0 10000])

figure 
hold on
plot(exampledata(:,9),exampledata(:,4));
plot(exampledata(:,9),exampledata(:,5));
plot(exampledata(:,9),exampledata(:,6));
hold off
