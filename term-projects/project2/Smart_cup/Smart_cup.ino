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
Ultrasonic ultrasonic(TRIGGER_PIN, ECHO_PIN);

//--BLE----------
#include "CurieBLE.h"
BLEPeripheral blePeripheral;
BLEService smartCupService("8371");
BLEUnsignedCharCharacteristic smartCupCharacteristic("8371", BLERead | BLEWrite);//uuid for height 2A8E

//--Accelerometer--
#include "CurieIMU.h"

int axRaw, ayRaw, azRaw;         // raw accelerometer values
float ax, ay, az;

//--Default Info--
float cup_height = 10;  // cm
float waterline = 0;   // cm
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

  if (flipped && az > 0) {
  //if (true) {
    blePeripheral.poll();
    float cm = ultrasonic.convert(ultrasonic.timing(), Ultrasonic::CM) ;
    Serial.print( "dis :");
    Serial.println( cm ) ;
    // listen for BLE peripherals to connect:
    BLECentral central = blePeripheral.central();
    

    // if a central is connected to peripheral:
    if (central) {
      Serial.print("Connected to central: ");
      // print the central's MAC address:
      Serial.println(central.address());

      // while the central is still connected to peripheral:
      while (central.connected()) {
        updateWaterLine(cm);
        //delay(1000);
      }

      // when the central disconnects, print it out:
      Serial.print(F("Disconnected from central: "));
      Serial.println(central.address());
    }
    else {
      Serial.println("BLE fail");
      updateWaterLine(cm);
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

void updateWaterLine(float &cm) {
  waterline = cm;
  smartCupCharacteristic.setValue(waterline);
}
float convertRawAcceleration(int aRaw) {
  return (aRaw * 2.0) / 32768.0;
}
