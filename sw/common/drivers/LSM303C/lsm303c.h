#ifndef __LSM303C_H__
#define __LSM303C_H__

#include <em_device.h>
#include "LSM303C_ACC_driver.h"
#include "LSM303C_MAG_driver.h"

#define HFRCO_FREQUENCY         14000000
#define SPI_PERCLK_FREQUENCY    HFRCO_FREQUENCY
#define SPI_BAUDRATE            1000000

#define SPI_LOC USART_ROUTE_LOCATION_LOC2 //2
#define BUFSIZE 80
#define SPI_USART USART0
#define ACC_CS_PIN 8
#define MAG_CS_PIN 10

#define LSM303_SUCCESS 0
#define LSM303_ERROR -1
#define DEV_ACC 0
#define DEV_MAG 1

int lsm303_init();
uint8_t spi_read(uint8_t dev, uint8_t adr);
void spi_send(uint8_t dev, uint8_t adr, uint8_t dat);

/* Accelerometer & Magnetometer registers */
//#define 	LSM303C_ACC_TEMP_L		 0x0B
//#define 	LSM303C_ACC_TEMP_H		 0x0C
//#define 	LSM303C_ACC_ACT_TSH		 0x1E
//#define 	LSM303C_ACC_ACT_DUR		 0x1F
//#define 	LSM303C_WHO_AM_I_REG	 0x0F
//#define 	LSM303C_CTRL1		 0x20
//#define 	LSM303C_CTRL2		 0x21
//#define 	LSM303C_CTRL3		 0x22
//#define 	LSM303C_CTRL4		 0x23
//#define 	LSM303C_CTRL5		 0x24
//#define 	LSM303C_CTRL6		 0x25 /* ACC only */
//#define 	LSM303C_CTRL7		 0x26 /* ACC only */
//#define 	LSM303C_STATUS   0x27
//#define 	LSM303C_OUT_X_L		 0x28
//#define 	LSM303C_OUT_X_H		 0x29
//#define 	LSM303C_OUT_Y_L		 0x2A
//#define 	LSM303C_OUT_Y_H		 0x2B
//#define 	LSM303C_OUT_Z_L		 0x2C
//#define 	LSM303C_OUT_Z_H		 0x2D
//#define 	LSM303C_ACC_FIFO_CTRL	 0x2E
//#define 	LSM303C_ACC_FIFO_SRC		 0x2F
//#define 	LSM303C_ACC_IG_CFG1		 0x30
//#define 	LSM303C_ACC_IG_SRC1		 0x31
//#define 	LSM303C_ACC_IG_THS_X1	 0x32
//#define 	LSM303C_ACC_IG_THS_Y1	 0x33
//#define 	LSM303C_ACC_IG_THS_Z1	 0x34
//#define 	LSM303C_ACC_IG_DUR1		 0x35
//#define 	LSM303C_ACC_IG_CFG2		 0x36
//#define 	LSM303C_ACC_IG_SRC2		 0x37
//#define 	LSM303C_ACC_IG_THS2		 0x38
//#define 	LSM303C_ACC_IG_DUR2		 0x39
//#define 	LSM303C_ACC_XL_REFERENCE		 0x3A
//#define 	LSM303C_ACC_XH_REFERENCE		 0x3B
//#define 	LSM303C_ACC_YL_REFERENCE		 0x3C
//#define 	LSM303C_ACC_YH_REFERENCE		 0x3D
//#define 	LSM303C_ACC_ZL_REFERENCE		 0x3E
//#define 	LSM303C_ACC_ZH_REFERENCE		 0x3F

#endif
