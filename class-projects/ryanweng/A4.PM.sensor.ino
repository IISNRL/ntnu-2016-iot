// Sense_TH: whether temperature/humidity sensor is used
// 0: none
// 1: Grove - Temperature and Humidity Sensor Pro (Digital, D5)
// 2: SHT31
// 3: Bosch BME280 (I2C)
#define Sense_TH        0
#define DHTPIN_D 5     // D5
#define DHTTYPE_D DHT22   // DHT 22  (AM2302)

// Sense_Light: whether light sensor is used
// 0: none
// 1: Grove - Digital Light Sensor (I2C)
// 2: SI1145 (I2C)
#define Sense_Light     0

// Sense_Barometer: whether barometer sensor is used
// 0: none
// 1: Grove - Grove - Barometer (High-Accuracy) (I2C)
// 2: Grove - Barometer Sensor (BMP180) (I2C)
// 3: Boshi BME280 (I2C)
#define Sense_Barometer 0

// Sense_PM: whether particulate matter sensor is used
// 0: none
// 1: PMS3003 (G3 sensor) (UART)
// 2: PMS3003 (G3 sensor) (SoftwareSerial; rx:10, tx:11)
// 3: CP-15-A4 (A4 sensor) (UART)
// 4: CP-15-A4 (A4 sensor) (SoftwareSerial; rx:10, tx:11)
// 5: PMS5003 (G5 sensor) (UART)
// 6: PMS5003 (G5 sensor) (SoftwareSerial; rx:10, tx:11)
#define Sense_PM        4

// Output_LCD: whether LCD is used
// 0: none
// 1: Grove - LCD RGB Backlight (I2C)
#define Output_LCD      0     // Sense_TH: whether temperature/humidity sensor is used

#include <Wire.h>
#include <Arduino.h>


  #if Sense_PM==1
    // Serial1.begin(9600);
  #elif Sense_PM==2
    #include <SoftwareSerial.h>
    SoftwareSerial mySerial(10, 11); 
  #elif Sense_PM==3
    // Serial1.begin(9600);
  #elif Sense_PM==4
    #include <SoftwareSerial.h>
    SoftwareSerial mySerial(0, 1);
  #elif Sense_PM==5
    // Serial1.begin(9600);
  #elif Sense_PM==6
    #include <SoftwareSerial.h>
    SoftwareSerial mySerial(10, 11);
  #endif


  void init_Sensor_PM(){
    #if Sense_PM==1
      // Serial1.begin(9600);
    #elif Sense_PM==2
      // mySerial.begin(9600);
    #elif Sense_PM==3
      // Serial1.begin(9600);
    #elif Sense_PM==4
      // mySerial.begin(9600);
    #elif Sense_PM==5
      // Serial1.begin(9600);
    #elif Sense_PM==6
      // mySerial.begin(9600);
    #endif
    return;
  }

   void get_Sensor_PM(int *PM1 = 0, int *PM25 = 0, int *PM10 = 0){


      unsigned long timeout = millis();
      int count=0;
      byte incomeByte[32];
      boolean startcount=false;
      byte data;
      mySerial.begin(9600);
      while (1){
        if((millis() -timeout) > 1500) {    
          Serial.println("[A4-ERROR-TIMEOUT]");
          //#TODO:make device fail alarm message here
          break;
        }
        if(mySerial.available()){
          data=mySerial.read();
          if(data==0x32 && !startcount){
            startcount = true;
            count++;
            incomeByte[0]=data;
          }else if(startcount){
            count++;
            incomeByte[count-1]=data;
            if(count>=32) {break;}
          }
        }
      }
      mySerial.end();
      mySerial.flush();
      unsigned int calcsum = 0; // BM
      unsigned int exptsum;
      for(int i = 0; i < 29; i++) {
        calcsum += (unsigned int)incomeByte[i];
      }
      
      exptsum = ((unsigned int)incomeByte[30] << 8) + (unsigned int)incomeByte[31];
      if(calcsum == exptsum) {
        *PM1 = ((unsigned int)incomeByte[4] << 8) + (unsigned int)incomeByte[5];
        *PM25 = ((unsigned int)incomeByte[6] << 8) + (unsigned int)incomeByte[7];
        *PM10 = ((unsigned int)incomeByte[8] << 8) + (unsigned int)incomeByte[9];
      } else {
        Serial.println("#[exception] PM2.5 Sensor CHECKSUM ERROR!");
        *PM1 = -1;
        *PM25 = -1;
        *PM10 = -1;
      }
   }

   


void setup() {
  Serial.begin(9600);
  init_Sensor_PM();
}
  

void loop() {
  int value_PM1, value_PM25, value_PM10;
  #if Sense_PM>0
    get_Sensor_PM(&value_PM1, &value_PM25, &value_PM10);
    Serial.print("[Sensor-PM] PM1 = ");
    Serial.println(value_PM1,DEC);    
    Serial.print("[Sensor-PM] PM25 = ");
    Serial.println(value_PM25,DEC);    
    Serial.print("[Sensor-PM] PM10 = ");
    Serial.println(value_PM10,DEC);    
  #endif

}
