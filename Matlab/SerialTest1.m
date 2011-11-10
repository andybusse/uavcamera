
ardio1 = serial('COM6');

%%
fopen(ardio1);

%%

fprintf(ardio1,'g');
numPackages = str2num(fgetl(ardio1));

dataIn = zeros(numPackages,64);

for x = 1:numPackages
    for y = 1:64
        dataIn(x,y) = fread(ardio1,1,'uint8');
    end
end

imagesc(dataIn.');

%%

fclose(ardio1);

%%

fid1 = fopen('testImage1.jpg','w');

fwrite(fid1,dataIn);

fclose(fid1);