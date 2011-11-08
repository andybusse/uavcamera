#include <SoftwareSerial.h>
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
  /*while (true) {
   char someChar = sSerial.read();
   sSerial.print("(");
   sSerial.print(someChar);
   sSerial.print(")");
   }*/
  
  sSerial.println("Attemting to establish contact.");
  establishContact();  // send a byte to establish contact until receiver responds
  sSerial.println("Contact established, captain"); 
  delay(2000); // 2 second pause
}

void loop()
{
  boolean wellness = takeSnapshot();
  if(wellness){
   sSerial.println("All's well");
   delay(2000);
  }else{
    sSerial.println("Trouble at mill...");
  }
}

void establishContact() {
  sSerial.println("Sending syncs");
  while(1){
    while (Serial.available() <= 0) {
      sSerial.print(".");
      sendSYNC();
      delay(50);
    }
    
    receiveComd();
 
    if(!isACK(RXtest,0x0D,0x00,0x00))
      continue;
    sSerial.println("ACK received");
    receiveComd();
     
   if(!isSYNC(RXtest))
     continue;
   
   sSerial.println("SYNC received");
   
   sendACK(0x0D,0x00,0x00,0x00);
   
   break;
  }
}

// takes a snapshot
boolean takeSnapshot() {
  
  // setup image parameters
  sSerial.println("sending initial");
  sendINITIAL(0x06,0x03,0x01);
  receiveComd();
  if(!isACK(RXtest,0x01,0x00,0x00))
      return false;
  sSerial.println("ACK received");
      
  /*sendSETPACKAGESIZE(0x00,0x02);
  receiveComd();
  if(!isACK(RXtest,0x06,0x00,0x00))
      return false; */
  
  // camera stores a single frame in its buffer  
  sSerial.println("sending snapshot");  
  sendSNAPSHOT(0x01,0x00,0x00);
  receiveComd();
  if(!isACK(RXtest,0x05,0x00,0x00))
      return false;
  sSerial.println("ACK received");
      
  // requests the image from the camera
  sSerial.println("sending getpicture");
  sendGETPICTURE(0x01);
  receiveComd();
  if(!isACK(RXtest,0x04,0x00,0x00))
      return false;
  sSerial.println("ACK received");
  receiveComd();    
  if(!isDATA(RXtest))
      return false;
  sSerial.println("DATA received");
      
      
  long bufferSize = 0;
  bufferSize = RXtest[5] | bufferSize;
  bufferSize = (bufferSize<<8) | RXtest[4];
  bufferSize = (bufferSize<<8) | RXtest[3];
  sSerial.print("Number of bytes: ");
  sSerial.println(bufferSize);
  
  byte dataIn[bufferSize][6];
  
  for(long dataPoint = 0;dataPoint<bufferSize;dataPoint++){
    receiveComd();
    for(int z = 0; z < 6; z++) {
      dataIn[dataPoint][z] = RXtest[z];
    }
    sSerial.print("data point ");
    sSerial.print(dataPoint);
    sSerial.println(" read successfully");
  }
  
  sendACK(0x0A,0x00,0x01,0x00);
  sSerial.println("Final ACK sent");
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
