/* Copyright 2011 Michael Hodgson, Piyabhum Sornpaisarn, Andrew Busse, John Charlesworth, Paramithi Svastisinha

    This file is part of uavcamera.

    uavcamera is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    uavcamera is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with uavcamera.  If not, see <http://www.gnu.org/licenses/>.

*/

int spiClkPin = 12;
int spiMISOPin = 13;
int spiSSPin = 11;
void setup() {
  pinMode(spiClkPin, INPUT);     
  pinMode(spiMISOPin, INPUT);
  pinMode(spiSSPin, INPUT);
  attachInterrupt(0, spiClkInt, FALLING);
  attachInterrupt(1, spiSSInt, FALLING);

  Serial.begin(9600);
  Serial.println("Debugger starting...");
  interrupts();
}

int bitNum = 0;
byte currentChar = 0;

void spiClkInt() {
  if(!digitalRead(spiSSPin)) {
   // we are selected 
   if(!digitalRead(spiMISOPin))
     bitSet(currentChar, bitNum);
   bitNum++;
   
   if(bitNum >= 8) {
     Serial.write(currentChar);  
     bitNum = 0;
     currentChar = 0x00;
    }
  } 
}

void spiSSInt() {
  // we have just been selected
  bitNum = 0;
  currentChar = 0x00;
}

void loop() {}
