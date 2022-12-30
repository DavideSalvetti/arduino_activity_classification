s = serial('COM7','BAU',115200,'terminator','LF');
fopen(s);

arr2 = nan(1000,1);

for i=1:1000
    arr2(i) = fscanf(s,'%d');
    pause(0.015)
end

fclose(s);



%%
figure
d_10 = diff(arr2(:,1));
plot(d_10,'o');
m_10=mean(d_10)
s_10 = std(d_10)
