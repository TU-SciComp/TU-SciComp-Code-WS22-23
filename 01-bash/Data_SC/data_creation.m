d=[20220201:20220228];
a = -100;
b = 100;
r = (b-a).*rand(1000,1) + a;
for i=1:28
    P=[];
    for j=1:10
    P=[P (b-a).*rand(100,1) + a;];
    filename=[num2str(d(i)) '.csv'];
    T=table(P);
    writetable(T,filename);
    end
end

