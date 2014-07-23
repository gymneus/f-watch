/*
 * TODO : license
 */

#ifndef _CONFIG_MDRIVER_SD_H_
#define _CONFIG_MDRIVER_SD_H_

#include "../../version/ver_mdriver_sd.h"
#if VER_MDRIVER_SD_MAJOR != 1 || VER_MDRIVER_SD_MINOR != 0
 #error Incompatible MDRIVER_SD version number!
#endif

#define PIN_UC_PC3    PRS_GPIO_PIN12
#define PIN_UC_PD0    28
#define PIN_UC_PD1    29
#define PIN_UC_PD2    30
#define PIN_UC_PD3    31

//SPI protocole
#define PIN_UC_SD_MOSI    PIN_UC_PD0
#define PIN_UC_SD_MISO    PIN_UC_PD1
#define PIN_UC_SD_CLK     PIN_UC_PD2
#define PIN_UC_SD_CS_N    PIN_UC_PD3
//checking if SD card is present
#define PIN_UC_SD_EN      PIN_UC_PC3

/* usage : add those pins values to GPIO_BASE */


#define MDRIVER_SD_SECTOR_SIZE   512       /* Sector size */

#define MDRIVER_SD_VOLUME0_SIZE  (32 * 1024) /* definition for size of sddrive0 */

#endif /* _CONFIG_MDRIVER_SD_H_ */
