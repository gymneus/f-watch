#ifndef __LSM303C_H__
#define __LSM303C_H__

#include <em_device.h>
//#include "LSM303C_ACC_driver.h"
//#include "LSM303C_MAG_driver.h"

#define HFRCO_FREQUENCY         14000000
#define SPI_PERCLK_FREQUENCY    HFRCO_FREQUENCY
#define SPI_BAUDRATE            1000000

#define SPI_LOC USART_ROUTE_LOCATION_LOC2 //2
#define BUFSIZE 80
#define SPI_USART USART0
#define ACC_CS_PIN 8
#define MAG_CS_PIN 10

#define LSM303_SUCCESS 1
#define LSM303_ERROR 0
#define DEV_ACC 0
#define DEV_MAG 1

/* Accelerometer & Magnetometer registers */
//#define 	LSM303_ACC_TEMP_L		 0x0B
//#define 	LSM303_ACC_TEMP_H		 0x0C
#define 	LSM303_ACC_ACT_TSH		 0x1E
#define 	LSM303_ACC_ACT_DUR		 0x1F
#define 	LSM303_WHO_AM_I_REG	 0x0F
#define 	LSM303_CTRL1		 0x20
#define 	LSM303_CTRL2		 0x21
#define 	LSM303_CTRL3		 0x22
#define 	LSM303_CTRL4		 0x23
#define 	LSM303_CTRL5		 0x24
#define 	LSM303_CTRL6		 0x25 /* ACC only */
#define 	LSM303_CTRL7		 0x26 /* ACC only */
#define 	LSM303_STATUS   0x27
#define 	LSM303_OUT_X_L		 0x28
#define 	LSM303_OUT_X_H		 0x29
#define 	LSM303_OUT_Y_L		 0x2A
#define 	LSM303_OUT_Y_H		 0x2B
#define 	LSM303_OUT_Z_L		 0x2C
#define 	LSM303_OUT_Z_H		 0x2D
#define 	LSM303_ACC_FIFO_CTRL	 0x2E /* ACC only */
#define 	LSM303_ACC_FIFO_SRC	 0x2F /* ACC only */
#define 	LSM303_MAG_TEMP_L   0x2E /* MAG only */
#define 	LSM303_MAG_TEMP_H   0x2F /* MAG only */
#define 	LSM303_INT1_CFG		 0x30
#define 	LSM303_INT1_SRC		 0x31
#define 	LSM303_INT1_THS_X	 0x32
#define 	LSM303_INT1_THS_Y	 0x33
#define 	LSM303_INT1_THS_Z	 0x34
#define 	LSM303_INT1_DUR		 0x35
#define 	LSM303_INT2_CFG 0x36 /* ACC only */
#define 	LSM303_INT2_SRC 0x37 /* ACC only */
#define 	LSM303_INT2_THS 0x38 /* ACC only */
#define 	LSM303_INT2_DUR 0x39 /* ACC only */
#define 	LSM303_XREF_L   0x3A /* ACC only */
#define 	LSM303_XREF_H   0x3B /* ACC only */
#define 	LSM303_YREF_L   0x3C /* ACC only */
#define 	LSM303_YREF_H   0x3D /* ACC only */
#define 	LSM303_ZREF_L   0x3E /* ACC only */
#define 	LSM303_ZREF_H   0x3F /* ACC only */

/* Accelerometer config */
#define LSM303_ACC_ST_MASK 0x0C
#define LSM303_ACC_ST_POS  0x04
#define LSM303_ACC_ST_NEG  0x08
#define LSM303_ACC_REBOOT	0x80
#define LSM303_FIFO_EN     0x80
typedef enum { 
	LSM303_ACC_ODR_PDOWN   = 0x00,
	LSM303_ACC_ODR_10_Hz 	= 0x10,
	LSM303_ACC_ODR_50_Hz 	= 0x20,
	LSM303_ACC_ODR_100_Hz 	= 0x30,
	LSM303_ACC_ODR_200_Hz 	= 0x40,
	LSM303_ACC_ODR_400_Hz 	= 0x50,
	LSM303_ACC_ODR_800_Hz 	= 0x60,
	LSM303_ACC_ODR_MASK 		= 0x60,
	LSM303_MAG_ODR_0_625_Hz = 0x00,
	LSM303_MAG_ODR_1_25_Hz  = 0x04,
	LSM303_MAG_ODR_2_5_Hz 	 = 0x08,
	LSM303_MAG_ODR_5_Hz     = 0x0C,
	LSM303_MAG_ODR_10_Hz    = 0x10,
	LSM303_MAG_ODR_20_Hz    = 0x14,
	LSM303_MAG_ODR_40_Hz    = 0x18,
	LSM303_MAG_ODR_80_Hz    = 0x1C,
  LSM303_MAG_ODR_MASK     = 0x1C
} LSM303_ODR_t;

typedef enum {
	LSM303_ACC_OPM_NORM =	0x00,
	LSM303_ACC_OPM_HIGH = 0x80,
	LSM303_ACC_OPM_MASK = 0x80,
	LSM303_MAG_OPM_LOW	 = 0x00,
	LSM303_MAG_OPM_MED	 = 0x20,
	LSM303_MAG_OPM_HIGH = 0x40,
	LSM303_MAG_OPM_UHIGH= 0x60,
	LSM303_MAG_OPM_MASK = 0x60
} LSM303_OPM_t;

#define LSM303_ACC_SRST 0x40
#define LSM303_MAG_SRST 0x04

typedef enum {
	LSM303_2G = 0x00,
	LSM303_4G = 0x20,
	LSM303_8G = 0x30,
	LSM303_ACC_FS_MASK = 0x30,
	LSM303_4Ga = 0x00,
	LSM303_8Ga = 0x20,
	LSM303_12Ga= 0x40,
	LSM303_16Ga= 0x60,
	LSM303_MAG_FS_MASK = 0x60
} LSM303_FS_t;

typedef enum {
	LSM303_ACC_4WIRE = 0x00,
	LSM303_MAG_4WIRE = 0x00,
	LSM303_ACC_3WIRE = 0x01,
	LSM303_MAG_3WIRE = 0x04
} LSM303_SMODE_t;

typedef enum {
	LEM303_ACC_XEN = 0x01,
	LSM303_ACC_YEN = 0x02,
	LSM303_ACC_ZEN = 0x04,
	LSM303_ACC_AXIS_MASK = 0x07
} LSM303_ENAXIS_t;

typedef enum {
	LSM303_FMODE_BYPASS         = 0x00,
	LSM303_FMODE_STOP_WHEN_FULL = 0x20,
	LSM303_FMODE_STREAM         = 0x40,
	LSM303_FMODE_STREAM_TO_FIFO = 0x60,
	LSM303_FMODE_BYPASS_TO_STREAM = 0x80,
	LSM303_FMODE_BYPASS_TO_FIFO = 0xE0,
	LSM303_FMODE_MASK = 0xE0
} LSM303_FMODE_t;

/* Magnetometer config */
#define LSM303_MAG_ST_MASK 0x01
#define LSM303_MAG_ST_EN   0x01
#define LSM303_MAG_TEMP_EN	0x80
#define LSM303_MAG_REBOOT	0x08


int lsm303_init();
int lsm303_selftest(int dev, int en, int dir);
int lsm303_odr(int dev, LSM303_ODR_t odr);
int lsm303_opmode(int dev, LSM303_OPM_t opm);
int lsm303_rst(int dev, int hard);
int lsm303_temp(int16_t *temp);
int lsm303_fullscale(int dev, LSM303_FS_t fs);
int lsm303_serialmode(int dev, LSM303_SMODE_t mode);
int lsm303_enableaxis(int dev, int mask);
int lsm303_fifo_mode(int dev, LSM303_FMODE_t mode, int en);
int lsm303_get_sample(int dev, int16_t *x, int16_t *y, int16_t *z);

int spi_read(uint8_t dev, uint8_t adr, uint8_t *dat);


#endif
