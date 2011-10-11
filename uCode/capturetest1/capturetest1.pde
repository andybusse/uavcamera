
int inByte = 0;         // incoming serial byte
byte SYNC[] = {0xAA,0x0D,0x00,0x00,0x00,0x00};
byte ACK[] = {0xAA,0x0E,0x0D,0x00,0x00,0x00};
enum commands{sync, ack};
byte RXtest[6];

void setup()
{
  // start serial port at 9600 bps:
  Serial.begin(115200);
  establishContact();  // send a byte to establish contact until receiver responds 
  while(1) {
   Serial.println("All's well");
   delay(200);
  }
}

void loop()
{
  // if we get a valid byte, read analog ins:
  if (Serial.available() > 0) {
    // get incoming byte:
    inByte = Serial.read();            
  }
}

void establishContact() {
  while(1){
    while (Serial.available() <= 0) {
      sendSYNC();
      delay(50);
    }

    for(int z = 0;z<6;z++){
      while (Serial.available() <= 0) {}
      RXtest[z] = Serial.read();
    }
 
    if(!isACK(RXtest))
      continue;
     
    for(int z = 0;z<6;z++){
      while (Serial.available() <= 0) {}
      RXtest[z] = Serial.read();
    }
     
   if(!isSYNC(RXtest))
     continue;
   
   sendACK();
   
   break;
  }
}

void sendSYNC() {
  for(int z = 0;z<6;z++){
    Serial.print(SYNC[z], BYTE);
  }
}

void sendACK() {
  for(int z = 0;z<6;z++){
    Serial.print(ACK[z], BYTE);
  }
}

boolean isACK(byte byteundertest[]){
  for(int z = 0;z<6;z++){
    if((z != 3) && (byteundertest[z] != ACK[z]))
      return false;
  }
  return true;
}

boolean isSYNC(byte byteundertest[]){
  for(int z = 0;z<6;z++){
    if(byteundertest[z] != SYNC[z])
      return false;
  }
  return true;
}
