/*
 * TODO : license
 */


#include "../../api/api_mdriver_sd.h"
#include "config_mdriver_sd.h"
#include "../../psp/include/psp_string.h"
#include "em_gpio.h"
#include "em_cmu.h"

#if VER_MDRIVER_SD_MAJOR != 1 || VER_MDRIVER_SD_MINOR != 0
 #error Incompatible MDRIVER_SD version number!
#endif

//TODO: use those CMD to send it through SPI interface
/* MMC/SD command (in SPI) */
#define MICROSD_CMD0	(0x40+0)	/* GO_IDLE_STATE */
#define MICROSD_CMD1	(0x40+1)	/* SEND_OP_COND (MMC) */
#define	MICROSD_ACMD41	(0xC0+41)	/* SEND_OP_COND (SDC) */
#define MICROSD_CMD8	(0x40+8)	/* SEND_IF_COND */
#define MICROSD_CMD9	(0x40+9)	/* SEND_CSD */
#define MICROSD_CMD10	(0x40+10)	/* SEND_CID */
#define MICROSD_CMD12	(0x40+12)	/* STOP_TRANSMISSION */
#define MICROSD_ACMD13	(0xC0+13)	/* SD_STATUS (SDC) */
#define MICROSD_CMD16	(0x40+16)	/* SET_BLOCKLEN */
#define MICROSD_CMD17	(0x40+17)	/* READ_SINGLE_BLOCK */
#define MICROSD_CMD18	(0x40+18)	/* READ_MULTIPLE_BLOCK */
#define MICROSD_CMD23	(0x40+23)	/* SET_BLOCK_COUNT (MMC) */
#define	MICROSD_ACMD23	(0xC0+23)	/* SET_WR_BLK_ERASE_COUNT (SDC) */
#define MICROSD_CMD24	(0x40+24)	/* WRITE_BLOCK */
#define MICROSD_CMD25	(0x40+25)	/* WRITE_MULTIPLE_BLOCK */
#define MICROSD_CMD55	(0x40+55)	/* APP_CMD */
#define MICROSD_CMD58	(0x40+58)	/* READ_OCR */


#include "../../version/ver_mdriver_ram.h"
#if VER_MDRIVER_RAM_MAJOR != 1 || VER_MDRIVER_RAM_MINOR != 2
 #error Incompatible MDRIVER_RAM version number!
#endif


static int sd_card_present(F_DRIVER * driver);


typedef struct {
  //TODO: we can improve and add the USART number here
  unsigned long  maxsector;
  int            use;
  F_DRIVER     * driver;
} t_SdDrv;

static F_DRIVER  t_drivers[1];

static t_SdDrv  SdDrv[1] =
{
  { ( MDRIVER_SD_VOLUME0_SIZE / MDRIVER_SD_SECTOR_SIZE ), 0, &t_drivers[0] }
};

static int sd_readsector ( F_DRIVER * driver, void * data, unsigned long sector ) {
	//TODO:implement sd_readsector
	return -1;
}
static int sd_writesector ( F_DRIVER * driver, void * data, unsigned long sector ) {
	//TODO:implement sd_writesector
	return -1;
}
static int sd_getphy ( F_DRIVER * driver, F_PHY * phy ) {
  t_SdDrv * p = (t_SdDrv *)( driver->user_ptr );

  if (sd_card_present(driver) != 0) {
    return MDRIVER_SD_ERR_NOTAVAILABLE;
  }

  phy->number_of_sectors = p->maxsector;
  phy->bytes_per_sector = MDRIVER_SD_SECTOR_SIZE;

  return MDRIVER_SD_NO_ERROR;
}
static void sd_release ( F_DRIVER * driver ) {
  t_SdDrv * p = (t_SdDrv *)( driver->user_ptr );

  if ( p == SdDrv ) {
    //TODO: check if SD protocol and disconnect properly the SD through SPI + SD protocol
    p->use = 0;
  }
}
static int sd_card_present(F_DRIVER * driver) {
	return (GPIO_PortInGet(gpioPortC) & 0x8);//pin = PC3
}
F_DRIVER * sd_initfunc ( unsigned long driver_param ) {

  t_SdDrv    * p;
  p = SdDrv + driver_param;

  if ( p != SdDrv ) {
    return 0;
  }
  if ( p->use ) {
    return 0;
  }
  {
    GPIO_Mode_TypeDef gpioModeSD_EN = gpioModeInput;
    GPIO_PinModeSet(gpioPortC, 3, gpioModeSD_EN, 0); /* init pin SD_EN to check if SD is plugged */
    /* Enabling clock to USART 1*/
    CMU_ClockEnable(cmuClock_USART1, true);
    SPI_setup(1, 1, 1);//init SD SPI
    
    //TODO: check specif of SD protocol and init SD through SPI + SD protocol

    (void)psp_memset( p->driver, 0, sizeof( F_DRIVER ) );

    p->driver->readsector = sd_readsector;
    p->driver->writesector = sd_writesector;
    p->driver->getphy = sd_getphy;
    p->driver->release = sd_release;
    p->driver->user_ptr = p;

    p->use = 1;

    return p->driver;
  }
} /* ram_initfunc */

