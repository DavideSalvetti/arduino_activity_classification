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


