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

clear all
close all

% load clown
load mandrill

X = (X-min(min(X)))./max(max(X-min(min(X))));


%% fft

freqX = fft(fft(X)')';

testFreqX = zeros(size(freqX));

shiftedFreqX = fftshift(freqX);

centre = size(freqX)/2;
figure

for z = 10:-0.5:1.5
    squarewidth = round(centre(1)/z);
    squareheight = round(centre(2)/z);
    % squarewidth = 10;
    % squareheight = 16;
    testFreqX = zeros(size(freqX));
    
    for x = -squarewidth:squarewidth;
        for y = -squareheight:squareheight;
            testFreqX(centre(1)+x,centre(2)+y) = shiftedFreqX(centre(1)+x,centre(2)+y);
        end
    end
    
    testFreqX = fftshift(testFreqX);
    
    %     testX = abs(ifft2(testFreqX));
    testX = abs(ifft(ifft(testFreqX)')');
    
    imagesc(testX)
    colormap gray
    drawnow
    pause(0.1)
end

%% enchunk

chunksize = 20; % must be even
imagesize = size(X);

% X2 = X;
% X = X(1:480,1:480);

for x = 1:round(imagesize(1)/chunksize)
    for y = 1:round(imagesize(2)/chunksize)
        chunk(x,y).data = X((((x-1)*chunksize)+1):(x*chunksize), (((y-1)*chunksize)+1):(y*chunksize));
    end
end

%% chunks fft

for z = 0.5:(chunksize/2)-0.5
    
    for chunkcount = 1:round(imagesize(1)/chunksize)*round(imagesize(2)/chunksize)
        
        chunk(chunkcount).freqX = fft(fft(chunk(chunkcount).data)')';
        
        chunk(chunkcount).testFreqX = zeros(size(chunk(chunkcount).freqX));
        
        chunk(chunkcount).shiftedFreqX = fftshift(chunk(chunkcount).freqX);
        
        centre = (chunksize/2)+0.5;
        
        for x = centre-z:centre+z;
            for y = centre-z:centre+z;
                chunk(chunkcount).testFreqX(x,y) = chunk(chunkcount).shiftedFreqX(x,y);
            end
        end
        
        chunk(chunkcount).testFreqX = fftshift(chunk(chunkcount).testFreqX);
        
        %     testX = abs(ifft2(testFreqX));
        chunk(chunkcount).testX = abs(ifft(ifft(chunk(chunkcount).testFreqX)')');
        
    end
    
    for x = 1:round(imagesize(1)/chunksize)
        for y = 1:round(imagesize(1)/chunksize)
            ReconTestX((((x-1)*chunksize)+1):(x*chunksize), (((y-1)*chunksize)+1):(y*chunksize)) = chunk(x,y).testX;
        end
    end
    
    figure
    
    imagesc(ReconTestX)
    colormap gray
    drawnow
    pause(0.5)
end

