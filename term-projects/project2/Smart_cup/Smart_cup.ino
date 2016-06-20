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
float radius = 0, cm = 0;  // cm
float cup_height = 10;  // cm
int waterline = 0;   // cm*10
float quantity = 0 ;  // cc:cm^3
bool flipped = false;

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
}

void loop() {


  // read raw accelerometer measurements from device
  CurieIMU.readAccelerometer(axRaw, ayRaw, azRaw);

  // convert the raw accelerometer data to G's
  ax = convertRawAcceleration(axRaw);
  ay = convertRawAcceleration(ayRaw);
  az = convertRawAcceleration(azRaw);

  float theta = atan( sqrt(ax * ax + ay * ay) / az ) * 180 / PI;
  Serial.print("Angle :");
  Serial.println(theta);

  Serial.print( "Direction" ) ;

  if ( az > 0 ) {
    Serial.println( "Down" ) ;
  }
  else {
    Serial.println(  "Up"  ) ;
    flipped = true;
  }

  //if (flipped && az > 0) {
  if (true) {
    blePeripheral.poll();

    // listen for BLE peripherals to connect:
    BLECentral central = blePeripheral.central();


    // if a central is connected to peripheral:
    if (central) {
      Serial.print("Connected to central: ");
      // print the central's MAC address:
      Serial.println(central.address());

      // while the central is still connected to peripheral:
      while (central.connected()) {
        meatureRadius();
        meatureHeight();
        updateWaterLine(cm);
        delay(3000);
      }

      // when the central disconnects, print it out:
      Serial.print(F("Disconnected from central: "));
      Serial.println(central.address());
    }
    else {
      Serial.println("BLE fail");
      //updateWaterLine(cm);
    }
    flipped = false;
  }

  /*
    char payload[20] ;
    dtostrf(cm, 20, 6, payload);
    mqtt_client.publish( "Test/" , payload ) ;
    mqtt_client.loop();
  */

  delay( 3000 );
}

void meatureHeight() {
  float total;
  for(int i=0;i<10;i+=1)
  {
    cm = ultrasonic_v.convert(ultrasonic_v.timing(), Ultrasonic::CM) ;
    total+=cm;
    delay(500);
  }
  cm = total/10;
  Serial.print( "vertical dis :");
  Serial.println( cm );
}

void meatureRadius() {
  radius = ultrasonic_h.convert(ultrasonic_h.timing(), Ultrasonic::CM) ;
  Serial.print( "horizontal dis :");
  Serial.println( radius ) ;
  Serial.print( "Area : ");
  Serial.print( radius * radius * PI );
  Serial.println( " cm^2" );
}

void updateWaterLine(float &cm) {
  waterline = cm * 10;
  Serial.println(waterline);
  Serial.println();
  smartCupCharacteristic.setValue(waterline);
}
float convertRawAcceleration(int aRaw) {
  return (aRaw * 2.0) / 32768.0;
}
