% Copyright 2011 Michael Hodgson, Piyabhum Sornpaisarn, Andrew Busse, John Charlesworth, Paramithi Svastisinha
%
%   This file is part of uavcamera.
%
%   uavcamera is free software: you can redistribute it and/or modify
%   it under the terms of the GNU General Public License as published by
%   the Free Software Foundation, either version 3 of the License, or
%   (at your option) any later version.
%
%   uavcamera is distributed in the hope that it will be useful,
%   but WITHOUT ANY WARRANTY; without even the implied warranty of
%   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
%   GNU General Public License for more details.
%
%   You should have received a copy of the GNU General Public License
%   along with uavcamera.  If not, see <http://www.gnu.org/licenses/>.

%%

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
