/*!
 *  @file Adafruit_LIS3X.h
 *
 *  @mainpage Adafruit LIS3X breakout board
 *
 *  @section intro_sec Introduction
 * *  This is a library for the Adafruit LIS3XX Family of Accelerometer breakout
 boards
 *
 *  Designed specifically to work with:
 *  * [Adafruit LIS331HH Triple-Axis Accelerometer
 (+-6g/12g/24g)](https://www.adafruit.com/product/4XXX)
 *  * [Adafruit H3LIS331 High-G Triple-Axis Accelerometer
 (+-100g/200g/400g)](https://www.adafruit.com/product/4XXX)

 *  Pick one up today in the adafruit shop!
 *
 *	These sensors communicate over I2C or SPI (our library code supports
 *both) so you can share it with a bunch of other sensors on the same I2C bus.
 *  There's an address selection pin so you can have two accelerometers share an
 *I2C bus.
 *
 *  Adafruit invests time and resources providing this open source code,
 *  please support Adafruit andopen-source hardware by purchasing products
 *  from Adafruit!
 *
 *  @section author Author
 *
 *  Bryan Siepert / K. Townsend / Limor Fried (Adafruit Industries)
 *
 *  @section license License
 *
 *  BSD license, all text above must be included in any redistribution
 */

#ifndef ADAFRUIT_LIS3X_H
#define ADAFRUIT_LIS3X_H

#include "Arduino.h"

#include <SPI.h>
#include <Wire.h>

#include <Adafruit_BusIO_Register.h>
#include <Adafruit_I2CDevice.h>
#include <Adafruit_SPIDevice.h>
#include <Adafruit_Sensor.h>
/** Default I2C ADDRESS. If SDO/SA0 is 3V, its 0x19**/
#define LIS3X_DEFAULT_ADDRESS (0x18)
#define LIS331_CHIP_ID                                                         \
  0x32 ///< The default response to WHO_AM_I for the H3LIS331 and LIS331HH

#define LIS3X_REG_WHOAMI                                                       \
  0x0F /**< Device identification register. [0, 0, 1, 1, 0, 0, 1, 1] */
#define LIS3X_REG_CTRL1 0x20 ///< Power mode, data rate, axis enable
#define LIS3X_REG_CTRL2 0x21 ///< Memory reboot, HPF config
#define LIS3X_REG_CTRL3                                                        \
  0x22 ///< Interrupt config, poarity, pin mode, latching, pin enable
#define LIS3X_REG_CTRL4 0x23           ///< BDU, Endianness, Range, SPI mode
#define LIS3X_REG_CTRL5 0x24           ///< Sleep to wake enable
#define LIS3X_REG_HP_FILTER_RESET 0x25 ///< Dummy register to reset filter
#define LIS3X_REG_REFERENCE 0x26       ///< HPF reference value
#define LIS3X_REG_STATUS 0x27  ///< Data overrun status, Data available status
#define LIS3X_REG_OUT_X_L 0x28 /**< X-axis acceleration data. Low value */
#define LIS3X_REG_OUT_X_H 0x29 /**< X-axis acceleration data. High value */
#define LIS3X_REG_OUT_Y_L 0x2A /**< Y-axis acceleration data. Low value */
#define LIS3X_REG_OUT_Y_H 0x2B /**< Y-axis acceleration data. High value */
#define LIS3X_REG_OUT_Z_L 0x2C /**< Z-axis acceleration data. Low value */
#define LIS3X_REG_OUT_Z_H 0x2D /**< Z-axis acceleration data. High value */
#define LIS3X_REG_INT1CFG 0x30 ///< INT1 config. Enable on hi/low for each axis
#define LIS3X_REG_INT1SRC 0x31 ///< INT1 source info
#define LIS3X_REG_INT1THS 0x32 ///< INT1 acceleration threshold
#define LIS3X_REG_INT1DUR 0x33 ///< INT1 duration threshold
#define LIS3X_REG_INT2CFG 0x34 ///< INT2 config. Enable on hi/low for each axis
#define LIS3X_REG_INT2SRC 0x35 ///< INT2 source info
#define LIS3X_REG_INT2THS 0x36 ///< INT2 acceleration threshold
#define LIS3X_REG_INT2DUR 0x37 ///< INT3 duration threshold

/** The high pass filter cutoff frequency */
typedef enum hpf_cutoff {
  LIS3X_HPF_0_02_ODR, ///< ODR/50
  LIS3X_HPF_0_01_ODR, ///< ODR/100
  LIS3X_HPF_0_005_ODR, ///< ODR/200
  LIS3X_HPF_0_0025_ODR, ///< ODR/400
} lis3x_hpf_cutoff_t;

/** The low pass filter cutoff frequency **/
typedef enum {
  LIS3X_LPF_37_HZ,
  LIS3X_LPF_74_HZ,
  LIS3X_LPF_292_HZ,
  LIS3X_LPF_780_HZ,

} lis3x_lpf_cutoff_t;
/** A structure to represent axes **/
typedef enum {
  LIS3X_AXIS_X = 0x0,
  LIS3X_AXIS_Y = 0x1,
  LIS3X_AXIS_Z = 0x2,
} lis3x_axis_t;

/*!
 *  @brief  Class that stores state and functions for interacting with
 *          Adafruit_LIS3X
 */
class Adafruit_LIS3X : public Adafruit_Sensor {
public:
  Adafruit_LIS3X(TwoWire *Wi = &Wire);
  Adafruit_LIS3X(int8_t cspin, SPIClass *theSPI = &SPI);
  Adafruit_LIS3X(int8_t cspin, int8_t mosipin, int8_t misopin, int8_t sckpin);

  uint8_t getDeviceID(void);
  bool configIntDataReady(uint8_t irqnum = 1, bool activelow = true,
                          bool opendrain = true);

  void read(void);

  bool getEvent(sensors_event_t *event);
  void getSensor(sensor_t *sensor);
  void highPassFilter(bool filter_enabled, lis3x_hpf_cutoff_t cutoff);
  void setLPFCutoff(lis3x_lpf_cutoff_t cutoff);
  virtual void _scaleValues(void);
  int16_t x; /**< x axis value */
  int16_t y; /**< y axis value */
  int16_t z; /**< z axis value */

protected:
  float x_g; /**< x_g axis value (calculated by selected range) */
  float y_g; /**< y_g axis value (calculated by selected range) */
  float z_g; /**< z_g axis value (calculated by selected scale) */

  Adafruit_I2CDevice *i2c_dev = NULL; ///< Pointer to I2C bus interface
  Adafruit_SPIDevice *spi_dev = NULL; ///< Pointer to I2C bus interface

  void writeDataRate(uint8_t dataRate);
  uint8_t readDataRate(void);

  void writeRange(uint8_t range);
  uint8_t readRange(void);

private:
  int32_t _sensorID;
};

#endif
