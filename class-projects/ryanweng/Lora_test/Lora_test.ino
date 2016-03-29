#include <Arduino.h>
//#include <Barometer.h>
//Barometer myBarometer;
//myBarometer.init();

void setup() {
  Serial1.begin(9600);
}

void loop() {
  Serial1.println("AT+DTX=3,\"   \"");
  delay(100);

}
