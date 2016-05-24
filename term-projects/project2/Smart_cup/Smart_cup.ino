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

void setup() {
  // put your setup code here, to run once:
  Serial.begin(BAUDRATE);

/*
  mqtt_client.setServer(MQTT_SERVER,1883);
  mqtt_client.connect( "ABCDEFG" ) ;
                                            */
}

void loop() {
  // put your main code here, to run repeatedly:

  float cm = ultrasonic.convert(ultrasonic.timing(), Ultrasonic::CM) ;
  Serial.println( cm ) ;

/*  
  char payload[20] ;
  dtostrf(cm, 20, 6, payload);
  mqtt_client.publish( "Test/" , payload ) ;
  mqtt_client.loop();
*/  
  
  delay( 3000 );
}
