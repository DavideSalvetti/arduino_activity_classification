%exampledata_filtr = exampledata - mean(exampledata);
%exampledata_filtr = movmean(exampledata_filtr,10,2);


exampledata_filtr = lowpass(exampledata,1,1000/15);
%exampledata_filtr = highpass(exampledata_filtr,0.5,1000/15);
exampledata_filtr(:,9) = exampledata(:,9) - exampledata(1,9);

figure 
hold on
plot(exampledata_filtr(:,9),exampledata_filtr(:,1));
plot(exampledata_filtr(:,9),exampledata_filtr(:,2));
plot(exampledata_filtr(:,9),exampledata_filtr(:,3));
hold off
legend('accX', 'accY', 'accZ')
xlim([5000 15000])

figure 
hold on
plot(exampledata_filtr(:,9),exampledata_filtr(:,4));
plot(exampledata_filtr(:,9),exampledata_filtr(:,5));
plot(exampledata_filtr(:,9),exampledata_filtr(:,6));
hold off
legend('gyroX', 'gyroY', 'gyroZ')
xlim([5000 15000])

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

figure
d_1 = diff(exampledata(:,9));
plot(d_1,'o');
m_1=mean(d_1)
s_1 = std(d_1)