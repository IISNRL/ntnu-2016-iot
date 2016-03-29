
void setup() {
  Serial1.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly: 
  Serial1.println( "AT+DTX=5,\"Fejai\"") ;
  delay(10000);
}
