void setup() {
  // put your setup code here, to run once:
Serial1.begin(9600);
}

void loop() {
  Serial1.println("AT+DTX=3,\"123\"");
  delay(3000);
}
