
// Basic demo for accelerometer readings from Adafruit H3LIS331

#include <Wire.h>
#include <SPI.h>
#include <Adafruit_H3LIS331.h>
#include <Adafruit_LIS331HH.h>
#include <Adafruit_Sensor.h>

// Used for software SPI
#define LIS331_SCK 13
#define LIS331_MISO 12
#define LIS331_MOSI 11
// Used for hardware & software SPI
#define LIS331_CS 10
unsigned long time;
unsigned long last_time;

//Adafruit_H3LIS331 lis = Adafruit_H3LIS331();
Adafruit_LIS331HH lis = Adafruit_LIS331HH();

void setup(void) {
  Serial.begin(115200);
  while (!Serial) delay(10);     // will pause Zero, Leonardo, etc until serial console opens

//  Serial.println("H3LIS331 test!");

//  if (!lis.begin_SPI(LIS331_CS)) {
//  if (!lis.begin_SPI(LIS331_CS, LIS331_SCK, LIS331_MISO, LIS331_MOSI)) {
 if (! lis.begin_I2C()) {   // change this to 0x19 for alternative i2c address
    Serial.println("Couldnt start");
    while (1) yield();
  }
  lis.setRange(LIS331HH_RANGE_6_G);
//  lis.setLPFCutoff(LIS3X_LPF_37_HZ); // requires that DataRate is a LOW_POWER rate
  lis.setDataRate(LIS331_DATARATE_50_HZ);
//  lis.setHPFReference(127);
//  ,-48.67
//  ,-48.82
//  ,-48.82
//This register sets the acceleration value taken as a reference for the high-pass filter output.
//When filter is turned onand HPM bits are set to “01”, filter out is generated taking this value as a reference
//  lis.setHPFReference(255);
//  ,10.17
//  ,9.96
//  ,10.02
//  ,9.96
//  ,10.02
//  ,10.11
//  HPFReset();
  lis.setHPFReference(0);


  lis.enableHighPassFilter(true, LIS3X_HPF_0_0025_ODR, true); //0.125Hz Cutoff
//  Serial.print("HPF reference: ");Serial.println(lis.getHPFReference());

  
  time = micros();
  last_time = time;
//    1000 / 100 => 10.000000
}

void loop() {
  /* Get a new sensor event, normalized */
  sensors_event_t event;
  lis.getEvent(&event);
  time = micros();

  /* Display the results (acceleration is measured in m/s^2) */
//  Serial.print(event.acceleration.x);
//  Serial.print(","); Serial.print(event.acceleration.y);
  Serial.print(","); Serial.print(event.acceleration.z);
//  Serial.print(","); Serial.print((time-last_time)/1000);
  Serial.println();
  last_time=time;
  delayMicroseconds(1000);
}
