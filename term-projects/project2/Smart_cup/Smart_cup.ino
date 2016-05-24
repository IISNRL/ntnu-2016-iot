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
#include <CurieBLE.h>
BLEPeripheral blePeripheral;
BLEService smartCupService("8371");
// BLE LED Switch Characteristic - custom 128-bit UUID, read and writable by central
BLEUnsignedCharCharacteristic smartCupCharacteristic("8371", BLERead | BLEWrite);


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
/*
  mqtt_client.setServer(MQTT_SERVER,1883);
  mqtt_client.connect( "ABCDEFG" ) ;
                                            */
}

void loop() {
  blePeripheral.poll();

  float cm = ultrasonic.convert(ultrasonic.timing(), Ultrasonic::CM) ;
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
      // if the remote device wrote to the characteristic,
      // use the value to control the LED:
      if (smartCupCharacteristic.written()) {
        if (smartCupCharacteristic.value()) {   // any value other than 0
          Serial.println("LED on");
        } else {                              // a 0 value
          Serial.println(F("LED off"));
        }
      }
    }

    // when the central disconnects, print it out:
    Serial.print(F("Disconnected from central: "));
    Serial.println(central.address());
  }
  else
    Serial.println("BLE fail");

/*  
  char payload[20] ;
  dtostrf(cm, 20, 6, payload);
  mqtt_client.publish( "Test/" , payload ) ;
  mqtt_client.loop();
*/  
  
  delay( 3000 );
}
