#define BAUDRATE 9600
/*
  //-----MQTT-----
  #include <PubSubClient.h>
  PubSubClient mqtt_client(client);
  #define MQTT_SERVER ""
*/

//--UltraSonic---
#include <Ultrasonic.h>
#define TRIGGER_PIN  12
#define ECHO_PIN     13

#define TRIGGER_2    10
#define ECHO_2       11

Ultrasonic ultrasonic_h(TRIGGER_PIN, ECHO_PIN);
Ultrasonic ultrasonic_v(TRIGGER_2, ECHO_2);

//--BLE----------
#include "CurieBLE.h"
BLEPeripheral blePeripheral;
BLEService smartCupService("19B10011-E8F2-537E-4F6C-D104768A1214");
BLEUnsignedCharCharacteristic smartCupCharacteristic("19B10011-E8F2-537E-4F6C-D104768A1214", BLERead);//uuid for height 2A8E

//--Accelerometer--
#include "CurieIMU.h"

int axRaw, ayRaw, azRaw;         // raw accelerometer values
float ax, ay, az;

//--Default Info--
float area = 0 ;  // cm^2
float cup_height = 10;  // cm
int waterLine = 0;   // cm
float quantity = 0 ;  // cc:cm^3
bool flipped = false;
boolean drinked = false ;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(BAUDRATE);

  // set advertised local name and service UUID:
  blePeripheral.setLocalName("SmartCup");
  blePeripheral.setAdvertisedServiceUuid(smartCupService.uuid());

  // add service and characteristic:
  blePeripheral.addAttribute(smartCupService);
  blePeripheral.addAttribute(smartCupCharacteristic);

  // set the initial value for the characeristic:
  smartCupCharacteristic.setValue(0);

  blePeripheral.begin();
  Serial.println("Bluetooth device active, waiting for connections...");

  CurieIMU.begin();
  // Set the accelerometer range to 2G
  CurieIMU.setAccelerometerRange(2);
  /*
    mqtt_client.setServer(MQTT_SERVER,1883);
    mqtt_client.connect( "ABCDEFG" ) ;
  */

  while( true ){                        // Waiting for putting on cup
      // read raw accelerometer measurements from device
      CurieIMU.readAccelerometer(axRaw, ayRaw, azRaw);
      float first = convertRawAcceleration(azRaw);
      delay(100) ;
      CurieIMU.readAccelerometer(axRaw, ayRaw, azRaw);
      float second = convertRawAcceleration(azRaw);

      if( abs(first-second) < 0.3 && first > 0.95 ) break ;
  }

  // Measure the waterline first
  waterLine = measureHeight() ;

  // Measure the radius of cup
  area =  measureRadius() ;
  area *= area ;
  area *= PI;

  Serial.print( "Area: " ) ;
  Serial.println( area ) ;
  Serial.print( "Height: " ) ;
  Serial.println( waterLine ) ;
  Serial.println( "SmartCup has already initailized.." ) ;
  Serial.println( "Start measure ..." ) ;
 
}

void loop() {


  // read raw accelerometer measurements from device
  CurieIMU.readAccelerometer(axRaw, ayRaw, azRaw);

  // convert the raw accelerometer data to G's
  ax = convertRawAcceleration(axRaw);
  ay = convertRawAcceleration(ayRaw);
  az = convertRawAcceleration(azRaw);

  float theta = atan( sqrt(ax * ax + ay * ay) / az ) * 180 / PI;

//  Serial.print("Angle :");
//  Serial.println(theta);

  //Serial.print( "Direction" ) ;
  if ( az > 0.95 ) {
    //Serial.println( "Down" ) ;
    flipped = false;
  }
  else if ( az < 0 ) {
    //Serial.println(  "Up"  ) ;
    flipped = true;
  }


   if( flipped ){
      drinked = true ;
      delay(1000) ;
  }
  else
  {
      if( drinked )
      {    
          Serial.println(  "drinked"  ) ;
          delay(800) ;
          float newWaterLine = measureHeight() ;
          //Serial.println( waterLine  ) ;
          if( newWaterLine < waterLine ){         // Fill water
              // do nothing ..
              Serial.println(  "Fill"  ) ;
          }
          else
          {
              quantity += (newWaterLine-waterLine)*area ;
              sendBLE(quantity);
          } 

          waterLine = newWaterLine ;              // Update waterline
          if( waterLine > cup_height ) cup_height = waterLine ;       // Update height of cup
      }
      drinked = false ;
  }

  /*
    char payload[20] ;
    dtostrf(cm, 20, 6, payload);
    mqtt_client.publish( "Test/" , payload ) ;
    mqtt_client.loop();
  */

}

float measureHeight() {
  float total;
  for(int i=0;i<10;i+=1)
  {
    float cm = ultrasonic_v.convert(ultrasonic_v.timing(), Ultrasonic::CM) ;
    total+=cm;
    delay(200);
  }
  return total/10 ;
}

float measureRadius() {
  float total , rad ;
  for(int i=0;i<10;i+=1)
  {
    rad = ultrasonic_h.convert(ultrasonic_h.timing(), Ultrasonic::CM) ;
    total+=rad;
    delay(200);
  }
  return total/10  ;
}

void sendBLE(float &vol) {
  Serial.print(vol);
  Serial.println(" cc");
  smartCupCharacteristic.setValue(vol*100);
}
float convertRawAcceleration(int aRaw) {
  return (aRaw * 2.0) / 32768.0;
}
