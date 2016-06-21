#include <Wire.h>                   
#include <IRremote.h>

const int IR_send = 4;
const unsigned int frequency = 38000; // 發射頻率(單位: Hz)
const int led=13;
const int PIRout=2;

void setup() {
  // put your setup code here, to run once:
  Wire.begin();
  Serial.begin(9600);
  
  pinMode(led,OUTPUT);  
  pinMode(PIRout,INPUT);
  
  pinMode(IR_send, OUTPUT); // 把 irLed 接腳設置為 Input
  

}

void loop() {

  int val=digitalRead(PIRout);  //讀取 PIR 輸出
  if (val==HIGH) {   //PIR 有偵測到時 : LED 閃一下
    digitalWrite(led,HIGH);
    tone(IR_send, frequency); // 產生指定頻率的脈波 (Pulses)
    delay(50);
    }
  else {  //PIR 沒有偵測到 : LED 暗
    digitalWrite(led,LOW);
    }
  
  delay(1000); //1000代表一秒
}


