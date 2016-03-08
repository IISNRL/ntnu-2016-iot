#include <Wire.h>
#include "SparkFunHTU21D.h"
#include <SFE_BMP180.h>
#include <BH1750FVI.h>

HTU21D myHumidity;
SFE_BMP180 pressure;
BH1750FVI myBH1750;

void setup()
{
  Serial.begin(9600);
  Serial.println("HTU21D Example!");

  myHumidity.begin();
  pressure.begin();
  myBH1750.begin();
}

void loop()
{
  float humd = myHumidity.readHumidity();
  float temp = myHumidity.readTemperature();

  Serial.print("Temperature:");
  Serial.print(temp, 1);
  Serial.print(" C");
  Serial.print(" Humidity:");
  Serial.print(humd, 1);
  Serial.println(" %");


  Serial.print( "Pressure:");
  Serial.print( getPressure() );
  Serial.println( " hPa");
  
  myBH1750.setSensitivity(2);
  Serial.print(F("Light level: "));
  Serial.print(myBH1750.readLightLevel());
  Serial.println(F(" lx"));
  Serial.println();
  Serial.println();
  
  delay(2000);
}


double getPressure()
{
  char status;
  double T,P,p0,a;

  // You must first get a temperature measurement to perform a pressure reading.
  
  // Start a temperature measurement:
  // If request is successful, the number of ms to wait is returned.
  // If request is unsuccessful, 0 is returned.

  status = pressure.startTemperature();
  if (status != 0)
  {
    // Wait for the measurement to complete:

    delay(status);

    // Retrieve the completed temperature measurement:
    // Note that the measurement is stored in the variable T.
    // Use '&T' to provide the address of T to the function.
    // Function returns 1 if successful, 0 if failure.

    status = pressure.getTemperature(T);
    if (status != 0)
    {
      // Start a pressure measurement:
      // The parameter is the oversampling setting, from 0 to 3 (highest res, longest wait).
      // If request is successful, the number of ms to wait is returned.
      // If request is unsuccessful, 0 is returned.

      status = pressure.startPressure(3);
      if (status != 0)
      {
        // Wait for the measurement to complete:
        delay(status);

        // Retrieve the completed pressure measurement:
        // Note that the measurement is stored in the variable P.
        // Use '&P' to provide the address of P.
        // Note also that the function requires the previous temperature measurement (T).
        // (If temperature is stable, you can do one temperature measurement for a number of pressure measurements.)
        // Function returns 1 if successful, 0 if failure.

        status = pressure.getPressure(P,T);
        if (status != 0)
        {
          return(P);
        }
        else Serial.println("error retrieving pressure measurement\n");
      }
      else Serial.println("error starting pressure measurement\n");
    }
    else Serial.println("error retrieving temperature measurement\n");
  }
  else Serial.println("error starting temperature measurement\n");
}
