/*
  Blink
  Turns on an LED on for one second, then off for one second, repeatedly.
 
  This example code is in the public domain.
 */
int spiClkPin = 12;
int spiMISOPin = 13;
int spiSSPin = 11;
void setup() {                
  // initialize the digital pin as an output.
  // Pin 13 has an LED connected on most Arduino boards:
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
