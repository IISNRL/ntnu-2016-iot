#include <Bridge.h>
#include <Temboo.h>
#include "TembooAccount.h" // contains Temboo account information, as described below

int numRuns = 1;   // Execution count, so this doesn't run forever
int maxRuns = 10;   // Maximum number of times the Choreo should be executed

void setup() {
  Serial.begin(9600);
  
  // For debugging, wait until the serial console is connected
  delay(4000);
  while(!Serial);
  Bridge.begin();
}

void loop() {
  if (numRuns <= maxRuns) {
    Serial.println("Running SendEmail - Run #" + String(numRuns++));
    
    TembooChoreo SendEmailChoreo;

    // Invoke the Temboo client
    SendEmailChoreo.begin();

    // Set Temboo account credentials
    SendEmailChoreo.setAccountName(TEMBOO_ACCOUNT);
    SendEmailChoreo.setAppKeyName(TEMBOO_APP_KEY_NAME);
    SendEmailChoreo.setAppKey(TEMBOO_APP_KEY);
    
    // Set Choreo inputs
    SendEmailChoreo.addInput("FromAddress", "timfrom2007@gmail.com");
    SendEmailChoreo.addInput("Username", "timfrom2007@gmail.com");
    SendEmailChoreo.addInput("Subject", "Watch!");
    SendEmailChoreo.addInput("ToAddress", "timfrom2007@gmail.com");
    SendEmailChoreo.addInput("MessageBody", "Check Your MSC Monitor!");
    SendEmailChoreo.addInput("Password", "qnjgutfeaouxisuh");
    
    // Identify the Choreo to run
    SendEmailChoreo.setChoreo("/Library/Google/Gmail/SendEmail");
    
    // Run the Choreo; when results are available, print them to serial
    SendEmailChoreo.run();
    
    while(SendEmailChoreo.available()) {
      char c = SendEmailChoreo.read();
      Serial.print(c);
    }
    SendEmailChoreo.close();
  }

  Serial.println("Waiting...");
  delay(30000); // wait 30 seconds between SendEmail calls
}#include <Bridge.h>
#include <Temboo.h>
#include "TembooAccount.h" // contains Temboo account information, as described below

int numRuns = 1;   // Execution count, so this doesn't run forever
int maxRuns = 10;   // Maximum number of times the Choreo should be executed

void setup() {
  Serial.begin(9600);
  
  // For debugging, wait until the serial console is connected
  delay(4000);
  while(!Serial);
  Bridge.begin();
}

void loop() {
  if (numRuns <= maxRuns) {
    Serial.println("Running SendEmail - Run #" + String(numRuns++));
    
    TembooChoreo SendEmailChoreo;

    // Invoke the Temboo client
    SendEmailChoreo.begin();

    // Set Temboo account credentials
    SendEmailChoreo.setAccountName(TEMBOO_ACCOUNT);
    SendEmailChoreo.setAppKeyName(TEMBOO_APP_KEY_NAME);
    SendEmailChoreo.setAppKey(TEMBOO_APP_KEY);
    
    // Set Choreo inputs
    SendEmailChoreo.addInput("FromAddress", "timfrom2007@gmail.com");
    SendEmailChoreo.addInput("Username", "timfrom2007@gmail.com");
    SendEmailChoreo.addInput("Subject", "Watch!");
    SendEmailChoreo.addInput("ToAddress", "timfrom2007@gmail.com");
    SendEmailChoreo.addInput("MessageBody", "Check Your MSC Monitor!");
    SendEmailChoreo.addInput("Password", "qnjgutfeaouxisuh");
    
    // Identify the Choreo to run
    SendEmailChoreo.setChoreo("/Library/Google/Gmail/SendEmail");
    
    // Run the Choreo; when results are available, print them to serial
    SendEmailChoreo.run();
    
    while(SendEmailChoreo.available()) {
      char c = SendEmailChoreo.read();
      Serial.print(c);
    }
    SendEmailChoreo.close();
  }

  Serial.println("Waiting...");
  delay(30000); // wait 30 seconds between SendEmail calls
}
