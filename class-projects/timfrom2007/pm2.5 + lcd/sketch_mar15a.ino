#include <Wire.h>
#include "rgb_lcd.h"

long pmcf10=0;
long pmcf25=0;
long pmcf100=0;
long pmat10=0;
long pmat25=0;
long pmat100=0;

char buf[50];

rgb_lcd lcd;

const int colorR = 255;
const int colorG = 0;
const int colorB = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial1.begin(9600);
  lcd.begin(16, 2);
  
}

void loop() {
  // put your main code here, to run repeatedly:
  int count = 0;
  unsigned char c;
  unsigned char high;
  lcd.setCursor(0, 0);
  
  while (Serial1.available()) {
    c = Serial1.read();
    if((count==0 && c!=0x42) || (count==1 && c!=0x4d)){
      lcd.println("check failed");
      break;
    }
      
    if(count > 15){
      lcd.println("complete");
      break;
    }
    else if(count == 4 || count == 6 || count == 8 || count == 10 || count == 12 || count == 14) {
      high = c;
    }
    else if(count == 7){
      pmcf25 = 256*high + c;
      lcd.println("CF=1,");
      lcd.print("PM2.5=");
      lcd.print(pmcf25);
      lcd.print(" ug/m3");
    }
    count++;
  }
  while(Serial1.available()) Serial1.read();
  lcd.println();
  delay(5000);
  lcd.clear();
}
