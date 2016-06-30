/*
  ===============================================
  Example sketch for CurieIMU library for Intel(R) Curie(TM) devices.
  Copyright (c) 2015 Intel Corporation.  All rights reserved.
 
  Based on I2C device class (I2Cdev) demonstration Arduino sketch for MPU6050
  class by Jeff Rowberg: [url=https://github.com/jrowberg/i2cdevlib]https://github.com/jrowberg/i2cdevlib[/url]
 
  ===============================================
  I2Cdev device library code is placed under the MIT license
  Copyright (c) 2011 Jeff Rowberg
 
  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:
 
  The above copyright notice and this permission notice shall be included in
  all copies or substantial portions of the Software.
 
  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
  THE SOFTWARE.
  ===============================================
 
  Genuino 101 CurieIMU Orientation Visualiser
  Hardware Required:
  * Arduino/Genuino 101
 
  Modified Nov 2015
  by Helena Bisby <[url=mailto:support@arduino.cc]support@arduino.cc[/url]>
  This example code is in the public domain
  [url=http://arduino.cc/en/Tutorial/Genuino101CurieIMUOrientationVisualiser]http://arduino.cc/en/Tutorial/Ge ... ientationVisualiser[/url]
*/
 
#include <CurieIMU.h>
#include <MadgwickAHRS.h>

 
Madgwick filter; // initialise Madgwick object
int ax, ay, az;
int gx, gy, gz;
int count,flag,flag2;
float yaw;
float pitch;
float roll;
float angle;
int factor = 11000; // variable by which to divide gyroscope values, used to control sensitivity
// note that an increased baud rate requires an increase in value of factor
 
int calibrateOffsets = 1; // int to determine whether calibration takes place or not

int lastStepCount = 0; //計步器
 
void setup() {
  
  // initialize Serial communication
  Serial.begin(9600);
  // initialize device
  CurieIMU.begin();
  delay(5000);
  CurieIMU.setStepDetectionMode(BMI160_STEP_MODE_NORMAL); // set step detection mode to normal
  CurieIMU.setStepCountEnabled(true); // enable step count
 
  Serial.println("IMU initialisation complete, waiting for events...");
  
  if (calibrateOffsets == 1) {
    // use the code below to calibrate accel/gyro offset values
    Serial.println("Internal sensor offsets BEFORE calibration...");
    Serial.print(CurieIMU.getAccelerometerOffset(X_AXIS)); Serial.print("\t");
    Serial.print(CurieIMU.getAccelerometerOffset(Y_AXIS)); Serial.print("\t");
    Serial.print(CurieIMU.getAccelerometerOffset(Z_AXIS)); Serial.print("\t");
    Serial.print(CurieIMU.getGyroOffset(X_AXIS)); Serial.print("\t");
    Serial.print(CurieIMU.getGyroOffset(Y_AXIS)); Serial.print("\t");
    Serial.print(CurieIMU.getGyroOffset(Z_AXIS)); Serial.print("\t");
    Serial.println("");
 
    // To manually configure offset compensation values, use the following methods instead of the autoCalibrate...() methods below
    //    CurieIMU.setGyroOffset(X_AXIS, 220);
    //    CurieIMU.setGyroOffset(Y_AXIS, 76);
    //    CurieIMU.setGyroOffset(Z_AXIS, -85);
    //    CurieIMU.setAccelerometerOffset(X_AXIS, -76);
    //    CurieIMU.setAccelerometerOffset(Y_AXIS, -235);
    //    CurieIMU.setAccelerometerOffset(Z_AXIS, 168);
 
    //IMU device must be resting in a horizontal position for the following calibration procedure to work correctly!
 
    Serial.print("Starting Gyroscope calibration...");
    CurieIMU.autoCalibrateGyroOffset();
    Serial.println(" Done");
    Serial.print("Starting Acceleration calibration...");
    CurieIMU.autoCalibrateAccelerometerOffset(X_AXIS, 0);
    CurieIMU.autoCalibrateAccelerometerOffset(Y_AXIS, 0);
    CurieIMU.autoCalibrateAccelerometerOffset(Z_AXIS, 1);
    Serial.println(" Done");
 
    Serial.println("Internal sensor offsets AFTER calibration...");
    Serial.print(CurieIMU.getAccelerometerOffset(X_AXIS)); Serial.print("\t");
    Serial.print(CurieIMU.getAccelerometerOffset(Y_AXIS)); Serial.print("\t");
    Serial.print(CurieIMU.getAccelerometerOffset(Z_AXIS)); Serial.print("\t");
    Serial.print(CurieIMU.getAccelerometerOffset(X_AXIS)); Serial.print("\t");
    Serial.print(CurieIMU.getAccelerometerOffset(Y_AXIS)); Serial.print("\t");
    Serial.print(CurieIMU.getAccelerometerOffset(Z_AXIS)); Serial.print("\t");
    Serial.println("");
  }
  count=0;
  angle=0;
  flag=0;
  flag2=0;
}
 
void loop() {
  // read raw accel/gyro measurements from device
  CurieIMU.readMotionSensor(ax, ay, az, gx, gy, gz); 
 
  // use function from MagdwickAHRS.h to return quaternions
  filter.updateIMU(gx / factor, gy / factor, gz / factor, ax, ay, az);
 
  // functions to find yaw roll and pitch from quaternions
  yaw = filter.getYaw();
  roll = filter.getRoll();
  pitch = filter.getPitch();
  angle=pitch*roll;
  // print gyro and accel values for debugging only, comment out when running Processing
  /*
  Serial.print(ax); Serial.print("\t");
  Serial.print(ay); Serial.print("\t");
  Serial.print(az); Serial.print("\t");
  Serial.print(gx); Serial.print("\t");
  Serial.print(gy); Serial.print("\t");
  Serial.print(gz); Serial.print("\t");
  Serial.println("");
  */
 
  /*if (Serial.available() > 0) {
    int val = Serial.read();
    if (val == 's') { // if incoming serial is "s"
      Serial.print(yaw);
      Serial.print(","); // print comma so values can be parsed
      Serial.print(pitch);
      Serial.print(","); // print comma so values can be parsed
      Serial.println(roll);
      
      //Serial.print(",");
      //updateStepCount();
     
    }*/

    if(angle>0.045&&flag==0)
    {
    count+=1;
    flag=1;
    Serial.print("You have do ");
    Serial.print(count);
    Serial.println(" steps");
     }
     if(angle<0.01&&flag==1)
        {
          flag=0;
        }
  //}
 /* Serial.print(roll);
  Serial.print("  ");
  Serial.println(pitch);*/
   if((angle>0.9||-angle>0.9)&&flag2==0)
   {
    flag2=1;
    Serial.println("You falled!");
   
   }
    if((angle<0.7||-angle<0.7)&&flag2==1)
    {flag2=0;}
      
  

    
   
}

void updateStepCount()
{
  int stepCount = CurieIMU.getStepCount(); // set stepCount to read stepCount from function
  if (stepCount != lastStepCount) { // if stepCount has changed
    Serial.println(stepCount);
    lastStepCount = stepCount;
  }
  else
  {
    Serial.println(stepCount);
  }
}
