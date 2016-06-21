#include <Wire.h>                   //Light Sensor
#include <Digital_Light_TSL2561.h>  //Light Sensor
#include <IRremote.h>

int RECV_PIN = 5;
IRrecv irrecv(RECV_PIN);
decode_results results;
int i,j; //用來測量光感變化量
int level=0; //警戒狀態

void setup() {
  
  // put your setup code here, to run once:
  Wire.begin();
  Serial.begin(9600);
  irrecv.enableIRIn(); // Start the receiver
  TSL2561.init();  //感光片初始化
}

void loop() {
  
  i = TSL2561.readVisibleLux(); //第一次數據
  delay(2500); //1000代表一秒
  j = TSL2561.readVisibleLux(); //第二次數據

  if(abs(j-i)>40){  //代表有東西覆蓋

    if (irrecv.decode(&results)) {
    //Serial.println(results.value, HEX);
    Serial.println("有接收");
    irrecv.resume(); // Receive the next value
    String resultString = String(results.value);
    if( !(resultString).equals("") ){

    }
    
  }
    
  }

  Serial.println(TSL2561.readVisibleLux());
  delay(200); //1000代表一秒
  
}


