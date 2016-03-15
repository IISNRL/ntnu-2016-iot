#include <Arduino.h>

//0:A4 1:G3
#define PM_sensor 1

//========================================

void get_PM(int *PM1 = 0,int *PM25 = 0,int *PM10 = 0)
{
  #if PM_sensor == 0
  unsigned long timeout = millis();
  int count = 0;
  byte incomeByte[32];
  boolean startcount = false;
  byte data;
  Serial1.begin(9600);
  while (1) {
    if ((millis() - timeout) > 1500) {
      Serial.println("[A4-ERROR-TIMEOUT]");
      //#TODO:make device fail alarm message here
      break;
    }
    if (Serial1.available()) {
      data = Serial1.read();
      if (data == 0x32 && !startcount) {
        startcount = true;
        count++;
        incomeByte[0] = data;
      } else if (startcount) {
        count++;
        incomeByte[count - 1] = data;
        if (count >= 32) {
          break;
        }
      }
    }
  }
  Serial1.end();
  Serial1.flush();
  unsigned int calcsum = 0; // BM
  unsigned int exptsum;
  for (int i = 0; i < 29; i++) {
    calcsum += (unsigned int)incomeByte[i];
  }

  exptsum = ((unsigned int)incomeByte[30] << 8) + (unsigned int)incomeByte[31];
  if (calcsum == exptsum) {
    *PM1 = ((unsigned int)incomeByte[4] << 8) + (unsigned int)incomeByte[5];
    *PM25 = ((unsigned int)incomeByte[6] << 8) + (unsigned int)incomeByte[7];
    *PM10 = ((unsigned int)incomeByte[8] << 8) + (unsigned int)incomeByte[9];
  } else {
    Serial.println("#[exception] PM2.5 Sensor CHECKSUM ERROR!");
    *PM1 = -1;
    *PM25 = -1;
    *PM10 = -1;
  } 
  
  #elif PM_sensor == 1
    unsigned long timeout = millis();
      int count=0;
      byte incomeByte[24];
      boolean startcount=false;
      byte data;
      Serial1.begin(9600);
      while (1){
        if((millis() - timeout) > 3000) {    
          Serial.println("[G3-ERROR-TIMEOUT]");
          break;
        }
        if(Serial1.available()){
          data=Serial1.read();
          if(data==0x42 && !startcount){
            startcount = true;
            count++;
            incomeByte[0]=data;
          } else if (startcount){
            count++;
            incomeByte[count-1]=data;
            if(count>=24) {break;}
          }
        }
      }
      Serial1.end();
      Serial1.flush();
      unsigned int calcsum = 0; // BM
      unsigned int exptsum;
      for(int i = 0; i < 22; i++) {
        calcsum += (unsigned int)incomeByte[i];
      }
    
      exptsum = ((unsigned int)incomeByte[22] << 8) + (unsigned int)incomeByte[23];
      if(calcsum == exptsum) {
        *PM1 = ((unsigned int)incomeByte[10] << 8) + (unsigned int)incomeByte[11];
        *PM25 = ((unsigned int)incomeByte[12] << 8) + (unsigned int)incomeByte[13];
        *PM10 = ((unsigned int)incomeByte[14] << 8) + (unsigned int)incomeByte[15];
      } else {
        Serial.println("#[exception] PM2.5 Sensor CHECKSUM ERROR!");
        *PM1 = -1;
        *PM25 = -1;
        *PM10 = -1;
      }
  #endif
}


void setup() {
  Serial.begin(9600);
}

void loop() {
  int PM1=0, PM25=0, PM10=0;

  get_PM(&PM1,&PM25,&PM10);

  Serial.print("PM1 = ");
  Serial.println(PM1,DEC);
  Serial.print("PM2.5 = ");
  Serial.println(PM25,DEC);
  Serial.print("PM10 = ");
  Serial.println(PM10,DEC);

}
