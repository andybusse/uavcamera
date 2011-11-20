/*
 * main.cpp
 *
 *  Created on: Mar 5, 2010
 *      Author: ssmethurst
 */
#include <WProgram.h>


int ledPin =  13;    // LED connected to digital pin 13

// The setup() method runs once, when the sketch starts
void setup()   {
  // initialize the digital pin as an output:
  pinMode(ledPin, OUTPUT);
  //Serial.begin(9600);
}

// the loop() method runs over and over again,
// as long as the Arduino has power

void loop()
{
  digitalWrite(ledPin, HIGH);   // set the LED on
  delay(1000);                  // wait for a second
  digitalWrite(ledPin, LOW);    // set the LED off
  delay(1000);                  // wait for a second
 // Serial.println("Blaaaaaaaaaaah!");
}


int main(void) {

  /* Must call init for arduino to work properly */
  init();
  setup();

  for (;;) {
	  loop();
  } // end for
} // end main


