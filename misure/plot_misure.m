camminata_1 = camminata - mean(camminata,1); 
camminata_1 = movmean(camminata_1,10);

figure 
hold on
%plot(camminata(:,9),camminata_1(:,1));
plot(camminata(:,9),camminata_1(:,2));
%plot(camminata(:,9),camminata_1(:,3));
hold off

figure 
hold on
plot(camminata(:,9),camminata_1(:,4));
%plot(camminata(:,9),camminata_1(:,5));
%plot(camminata(:,9),camminata_1(:,6));
hold off

%%

figure
d_1 = diff(Corsa1(:,9));
plot(d_1,'o');
m_1=mean(d_1)
s_1 = std(d_1)

figure
d_2 = diff(Corsa2(:,9));
plot(d_2,'o');
m_2=mean(d_2)
s_2 = std(d_2)

figure
d_3 = diff(Camminatarotazionesx1(:,9));
plot(d_3,'o');
m_3=mean(d_3)
s_3 = std(d_3)

figure
d_4 = diff(Camminatarotazionesx2(:,9));
plot(d_4,'o');
m_4=mean(d_4)
s_4 = std(d_4)

figure
d_5 = diff(Camminatarotazionesx3(:,9));
plot(d_5,'o');
m_5=mean(d_3)
s_5 = std(d_5)

figure
d_6 = diff(Camminatarotazionedx1(:,9));
plot(d_6,'o');
m_6=mean(d_6)
s_6 = std(d_6)

figure
d_7 = diff(Camminatarotazionedx2(:,9));
plot(d_7,'o');
m_7=mean(d_7)
s_7 = std(d_7)

figure
d_7 = diff(Camminatarotazionedx3(:,9));
plot(d_7,'o');
m_7=mean(d_7)
s_7 = std(d_7)




%% test 2m
figure
d_8 = diff(test1(:,9));
plot(d_8,'o');
m_8=mean(d_8)
s_8 = std(d_8)

figure
d_9 = diff(test2(:,9));
plot(d_9,'o');
m_9=mean(d_9)
s_9 = std(d_9)


%% test 10cm

figure
d_10 = diff(test3(:,9));
plot(d_10,'o');
m_10=mean(d_10)
s_10 = std(d_10)


%% test 20ms

figure
d_11 = diff(test4(:,9));
plot(d_11,'o');
m_11=mean(d_11)
s_11 = std(d_11)
%% circular buffer

figure
d_100 = diff(test10(:,9));
plot(d_100,'o');
m_100=mean(d_100)
s_100 = std(d_100)

figure
d_150 = diff(test15(:,9));
plot(d_150,'o');
m_150=mean(d_150)
s_150 = std(d_150)

figure
d_test = diff(test(:,9));
plot(d_test,'o');
m_test=mean(d_test)
s_test = std(d_test)

test_1 = test - mean(test,1); 
test_1 = movmean(test,100);


