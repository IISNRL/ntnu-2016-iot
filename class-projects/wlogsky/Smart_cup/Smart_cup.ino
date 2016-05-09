#include <Wire.h>
#include <SPI.h>
#define BAUDRATE 9600

//-----WIFI-----
#include "YunClient.h"
YunClient client;

//-----MQTT-----
#include <PubSubClient.h>
PubSubClient mqtt_client(client);
#define MQTT_SERVER "61.230.197.66"

//--UltraSonic---
#include <Ultrasonic.h>
#define TRIGGER_PIN  12
#define ECHO_PIN     13
Ultrasonic ultrasonic(TRIGGER_PIN, ECHO_PIN);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(BAUDRATE);
  Bridge.begin();

  mqtt_client.setServer(MQTT_SERVER,1883);
  while(!mqtt_client.connect( "WlogskyTest" )){
      Serial.println("MQTT connect fail");  
  }
  if( mqtt_client.connected() ){
    Serial.println("MQTT connect ok");    
  }

}

void loop() {
  // put your main code here, to run repeatedly:

  float cm = ultrasonic.convert(ultrasonic.timing(), Ultrasonic::CM) ;
  Serial.println( cm ) ;
  char payload[20] ;
  dtostrf(cm, 20, 6, payload);

  while( !mqtt_client.connected() ){        // Reconnect
      mqtt_client.connect( "WlogskyTest" );
  }

  mqtt_client.publish( "Wlogsky/" , payload ) ;
  mqtt_client.loop();
  delay( 10000 );
}
