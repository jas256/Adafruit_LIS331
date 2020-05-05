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
/*!
 *  STATUS_REG_AUX register
 *   321OR  1, 2 and 3 axis data overrun. Default value: 0
 *          (0: no overrun has occurred; 1: a new set of data has overwritten
 * the previous ones) 3OR    3 axis data overrun. Default value: 0 (0: no
 * overrun has occurred; 1: a new data for the 3-axis has overwritten the
 * previous one) 2OR    2 axis data overrun. Default value: 0 (0: no overrun has
 * occurred; 1: a new data for the 4-axis has overwritten the previous one) 1OR
 * 1 axis data overrun. Default value: 0 (0: no overrun has occurred; 1: a new
 * data for the 1-axis has overwritten the previous one) 321DA  1, 2 and 3 axis
 * new data available. Default value: 0 (0: a new set of data is not yet
 * available; 1: a new set of data is available) 3DA:   3 axis new data
 * available. Default value: 0 (0: a new data for the 3-axis is not yet
 * available; 1: a new data for the 3-axis is available) 2DA:   2 axis new data
 * available. Default value: 0 (0: a new data for the 2-axis is not yet
 * available; 1: a new data for the 2-axis is available) 1DA    1 axis new data
 * available. Default value: 0 (0: a new data for the 1-axis is not yet
 * available; 1: a new data for the 1-axis is available)
 */
#define LIS3X_REG_STATUS1 0x07
#define LIS3X_REG_OUTADC1_L 0x08 /**< 1-axis acceleration data. Low value */
#define LIS3X_REG_OUTADC1_H 0x09 /**< 1-axis acceleration data. High value */
#define LIS3X_REG_OUTADC2_L 0x0A /**< 2-axis acceleration data. Low value */
#define LIS3X_REG_OUTADC2_H 0x0B /**< 2-axis acceleration data. High value */
#define LIS3X_REG_OUTADC3_L 0x0C /**< 3-axis acceleration data. Low value */
#define LIS3X_REG_OUTADC3_H 0x0D /**< 3-axis acceleration data. High value */
#define LIS3X_REG_INTCOUNT                                                     \
  0x0E /**< INT_COUNTER register [IC7, IC6, IC5, IC4, IC3, IC2, IC1, IC0] */
#define LIS3X_REG_WHOAMI                                                       \
  0x0F /**< Device identification register. [0, 0, 1, 1, 0, 0, 1, 1] */
/*!
 *  TEMP_CFG_REG
 *  Temperature configuration register.
 *   ADC_PD   ADC enable. Default value: 0
 *            (0: ADC disabled; 1: ADC enabled)
 *   TEMP_EN  Temperature sensor (T) enable. Default value: 0
 *            (0: T disabled; 1: T enabled)
 */
#define LIS3X_REG_TEMPCFG 0x1F
/*!
 *  CTRL_REG1
 *  [ODR3, ODR2, ODR1, ODR0, LPen, Zen, Yen, Xen]
 *   ODR3-0  Data rate selection. Default value: 00
 *           (0000:50 Hz; Others: Refer to Datasheet Table 26, “Data rate
 * configuration”) LPen    Low power mode enable. Default value: 0 (0: normal
 * mode, 1: low power mode) Zen     Z axis enable. Default value: 1 (0: Z axis
 * disabled; 1: Z axis enabled) Yen     Y axis enable. Default value: 1 (0: Y
 * axis disabled; 1: Y axis enabled) Xen     X axis enable. Default value: 1 (0:
 * X axis disabled; 1: X axis enabled)
 */
#define LIS3X_REG_CTRL1 0x20
/*!
 *  CTRL_REG2
 *  [HPM1, HPM0, HPCF2, HPCF1, FDS, HPCLICK, HPIS2, HPIS1]
 *   HPM1-0  High pass filter mode selection. Default value: 00
 *           Refer to Table 29, "High pass filter mode configuration"
 *   HPCF2-1 High pass filter cut off frequency selection
 *   FDS     Filtered data selection. Default value: 0
 *					 (0: internal filter bypassed; 1: data
 *from internal filter sent to output register and FIFO) HPCLICK High pass
 *filter enabled for CLICK function. (0: filter bypassed; 1: filter enabled)
 *HPIS2   X axis enable. Default value: 1 (0: X axis disabled; 1: X axis
 *enabled) HPIS1 High pass filter enabled for AOI function on interrupt 1, (0:
 *filter bypassed; 1: filter enabled)
 */
#define LIS3X_REG_CTRL2 0x21
#define LIS3X_REG_CTRL3 0x22
#define LIS3X_REG_CTRL4 0x23
#define LIS3X_REG_CTRL5 0x24
#define LIS3X_REG_HP_FILTER_RESET 0x25
#define LIS3X_REG_REFERENCE 0x26
#define LIS3X_REG_STATUS 0x27
#define LIS3X_REG_OUT_X_L 0x28 /**< X-axis acceleration data. Low value */
#define LIS3X_REG_OUT_X_H 0x29 /**< X-axis acceleration data. High value */
#define LIS3X_REG_OUT_Y_L 0x2A /**< Y-axis acceleration data. Low value */
#define LIS3X_REG_OUT_Y_H 0x2B /**< Y-axis acceleration data. High value */
#define LIS3X_REG_OUT_Z_L 0x2C /**< Z-axis acceleration data. Low value */
#define LIS3X_REG_OUT_Z_H 0x2D /**< Z-axis acceleration data. High value */
#define LIS3X_REG_INT1CFG 0x30
#define LIS3X_REG_INT1SRC 0x31
#define LIS3X_REG_INT1THS 0x32
#define LIS3X_REG_INT1DUR 0x33
#define LIS3X_REG_INT2CFG 0x34
#define LIS3X_REG_INT2SRC 0x35
#define LIS3X_REG_INT2THS 0x36
#define LIS3X_REG_INT2DUR 0x37

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
  bool configIntDataReady(uint8_t irqnum=1, bool activelow=true, bool opendrain=true);

  void read(void);

  bool getEvent(sensors_event_t *event);
  void getSensor(sensor_t *sensor);

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
