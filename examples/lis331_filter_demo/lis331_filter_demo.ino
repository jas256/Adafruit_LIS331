
// Basic demo for accelerometer readings from Adafruit H3LIS331

#include <Wire.h>
#include <SPI.h>
#include <Adafruit_H3LIS331.h>
#include <Adafruit_Sensor.h>

// Used for software SPI
#define H3LIS331_SCK 13
#define H3LIS331_MISO 12
#define H3LIS331_MOSI 11
// Used for hardware & software SPI
#define H3LIS331_CS 10


Adafruit_H3LIS331 lis = Adafruit_H3LIS331();

void setup(void) {
  Serial.begin(115200);
  while (!Serial) delay(10);     // will pause Zero, Leonardo, etc until serial console opens

//  Serial.println("H3LIS331 test!");

//  if (!lis.begin_SPI(H3LIS331_CS)) {
//  if (!lis.begin_SPI(H3LIS331_CS, H3LIS331_SCK, H3LIS331_MISO, H3LIS331_MOSI)) {
 if (! lis.begin_I2C()) {   // change this to 0x19 for alternative i2c address
    Serial.println("Couldnt start");
    while (1) yield();
  }

   lis.setRange(H3LIS331_RANGE_400_G);   // 100, 200, or 400 G!
   lis.setDataRate(H3LIS331_DATARATE_LOWPOWER_10_HZ);
//  Serial.print("Data rate set to: ");
//  switch (lis.getDataRate()) {
//
//    case H3LIS331_DATARATE_POWERDOWN: Serial.println("Powered Down"); break;
//    case H3LIS331_DATARATE_50_HZ: Serial.println("50 Hz"); break;
//    case H3LIS331_DATARATE_100_HZ: Serial.println("100 Hz"); break;
//    case H3LIS331_DATARATE_400_HZ: Serial.println("400 Hz"); break;
//    case H3LIS331_DATARATE_1000_HZ: Serial.println("1000 Hz"); break;
//    case H3LIS331_DATARATE_LOWPOWER_0_5_HZ: Serial.println("0.5 Hz Low Power"); break;
//    case H3LIS331_DATARATE_LOWPOWER_1_HZ: Serial.println("1 Hz Low Power"); break;
//    case H3LIS331_DATARATE_LOWPOWER_2_HZ: Serial.println("2 Hz Low Power"); break;
//    case H3LIS331_DATARATE_LOWPOWER_5_HZ: Serial.println("5 Hz Low Power"); break;
//    case H3LIS331_DATARATE_LOWPOWER_10_HZ: Serial.println("10 Hz Low Power"); break;
//
////  }
  lis.setLPFCutoff(LIS3X_LPF_37_HZ);
  lis.highPassFilter(true, LIS3X_HPF_0_0025_ODR);
}

void loop() {
  /* Get a new sensor event, normalized */
  sensors_event_t event;
  lis.getEvent(&event);

  /* Display the results (acceleration is measured in m/s^2) */
  Serial.print(event.acceleration.x);
  Serial.print(","); Serial.print(event.acceleration.y);
  Serial.print(","); Serial.print(event.acceleration.z);
  Serial.println();

  delay(10);
}
