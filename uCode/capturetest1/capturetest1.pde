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

// Jpeg image capture

#include <SoftwareSerial.h>
#define DLOG(...)  sSerial.print(__VA_ARGS__)
//#define DLOG(...)
byte SYNC[] = {0xAA,0x0D,0x00,0x00,0x00,0x00};
byte RXtest[6];

#define sRxPin 2
#define sTxPin 3

SoftwareSerial sSerial = SoftwareSerial(sRxPin, sTxPin);

void setup()
{
  // start serial port at 115200 bps
  Serial.begin(115200);
  
  // start software serial library for debugging
  pinMode(sRxPin, INPUT);
  pinMode(sTxPin, OUTPUT);
  sSerial.begin(9600);
  
  //DLOG("Attemting to establish contact.\n\r");
  establishContact();  // send a byte to establish contact until receiver responds
  //DLOG("Contact established, captain\n\r"); 
  delay(2000); // 2 second pause
}

void loop()
{
  sSerial.read();
  boolean wellness = takeSnapshot();
  if(wellness){
   DLOG("All's well\n\r");
   delay(2000);
  }else{
    DLOG("Trouble at mill...\n\r");
  }
}

// synchronise with camera
void establishContact() {
  //DLOG("Sending syncs\n\r\r");
  while(1){
    while (Serial.available() <= 0) {
      //DLOG(".");
      sendSYNC();
      delay(50);
    }
    
    receiveComd();
 
    if(!isACK(RXtest,0x0D,0x00,0x00))
      continue;
    //DLOG("ACK received\n\r");
    receiveComd();
     
   if(!isSYNC(RXtest))
     continue;
   
   //DLOG("SYNC received\n\r");
   
   sendACK(0x0D,0x00,0x00,0x00);
   
   break;
  }
}

// takes a snapshot
boolean takeSnapshot() {
  
  // setup image parameters
  //DLOG("sending initial\n\r");
  delay(50);
  sendINITIAL(0x07,0x07,0x07);
  receiveComd();
  if(!isACK(RXtest,0x01,0x00,0x00))
      return false;
  //DLOG("ACK received\n\r");
  
  unsigned int packageSize = 64;
  
  // sets the size of the packets to be sent from the camera
  //DLOG("sending setpackagesize\n\r");
  delay(50);
  sendSETPACKAGESIZE((byte)packageSize,(byte)(packageSize>>8));
  receiveComd();
  if(!isACK(RXtest,0x06,0x00,0x00))
      return false;
  //DLOG("ACK received\r");
  
  // camera stores a single frame in its buffer  
  //DLOG("sending snapshot\n\r");
  delay(50);
  sendSNAPSHOT(0x00,0x00,0x00);
  receiveComd();
  if(!isACK(RXtest,0x05,0x00,0x00))
      return false;
  //DLOG("ACK received\n\r");
      
  // requests the image from the camera
  //DLOG("sending getpicture\n\r");
  delay(50);
  sendGETPICTURE(0x01);
  receiveComd();
  if(!isACK(RXtest,0x04,0x00,0x00))
      return false;
  //DLOG("ACK received\n\r");
  receiveComd();    
  if(!isDATA(RXtest))
      return false;
  //DLOG("DATA received\n\r");
      
  unsigned long bufferSize = 0;
  bufferSize = RXtest[5] | bufferSize;
  bufferSize = (bufferSize<<8) | RXtest[4];
  bufferSize = (bufferSize<<8) | RXtest[3];
  
  unsigned int numPackages = bufferSize/(packageSize-6);
  
  //DLOG("Number of packages: ");
  DLOG(numPackages,DEC);
  sSerial.println();
  
  byte dataIn[packageSize]; // 512 is from the setpackagesize command
  
  for(unsigned int package = 0;package<numPackages;package++){
    
    //DLOG("Sending ACK for package ");
    //DLOG(package,DEC);
    //DLOG("\n\r");
    sendACK(0x00,0x00,(byte)package,(byte)(package>>8));
    
    // receive package
    for(int dataPoint = 0; dataPoint<packageSize;dataPoint++){
      while (Serial.available() <= 0) {} // wait for data to be available n.b. will wait forever...
      dataIn[dataPoint] = Serial.read();
      if(dataPoint > 3 && dataPoint < (packageSize - 2)){
      sSerial.print(dataIn[dataPoint],BYTE);
      }
      //DLOG(dataPoint);
      //DLOG("\n\r");
    }
    //DLOG("Package ");
    //DLOG(package);
    //DLOG(" read successfully\n\r");
  }
  
  sendACK(0x0A,0x00,0xF0,0xF0);
  DLOG("Final ACK sent\n\r");
  return true;
}

void sendSYNC() {
  sendCommand(0xAA,0x0D,0x00,0x00,0x00,0x00);
}

void sendACK(byte comID, byte ACKcounter, byte pakID1, byte pakID2) {
 sendCommand(0xAA,0x0E,comID,ACKcounter,pakID1,pakID2);
}

void sendINITIAL(byte colourType, byte rawRes, byte jpegRes) {
 sendCommand(0xAA,0x01,0x00,colourType,rawRes,jpegRes);
}

void sendSETPACKAGESIZE(byte paklb, byte pakhb) {
 sendCommand(0xAA,0x06,0x08,paklb,pakhb,0x00);
}

void sendSNAPSHOT(byte snapType, byte skip1, byte skip2) {
 sendCommand(0xAA,0x05,snapType,skip1,skip2,0x00);
}

void sendGETPICTURE(byte picType) {
 sendCommand(0xAA,0x04,picType,0x00,0x00,0x00);
}

// sends commands to the camera
void sendCommand(byte ID1, byte ID2, byte par1, byte par2, byte par3, byte par4) {
  byte Command[] = {ID1,ID2,par1,par2,par3,par4};
  Serial.write(Command, 6);
}

// receives messages from the camera
void receiveComd() {
  for(int z = 0;z<6;z++){
      while (Serial.available() <= 0) {}
      RXtest[z] = Serial.read();
  }
}

// verifies ACK
boolean isACK(byte byteundertest[],byte comID, byte pakID1, byte pakID2){
  byte testACK[] = {0xAA,0x0E,comID,0x00,pakID1,pakID2};
  for(int z = 0;z<6;z++){
    if((z != 3) && (byteundertest[z] != testACK[z]))
      return false;
  }
  return true;
}

// verifies SYNC
boolean isSYNC(byte byteundertest[]){
  for(int z = 0;z<6;z++){
    if(byteundertest[z] != SYNC[z])
      return false;
  }
  return true;
}

// verifies DATA
boolean isDATA(byte byteundertest[]){
    if((byteundertest[0] != 0xAA) || (byteundertest[1] != 0x0A)){
      return false; }
  return true;
}
