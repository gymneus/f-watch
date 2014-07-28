/*
 * TODO : license
 */


#include "../../api/api_mdriver_sd.h"
#include "config_mdriver_sd.h"
#include "../../psp/include/psp_string.h"
#include "spi.h"
#include "em_gpio.h"
#include "em_cmu.h"

#if VER_MDRIVER_SD_MAJOR != 1 || VER_MDRIVER_SD_MINOR != 0
 #error Incompatible MDRIVER_SD version number!
#endif

//TODO: use those CMD to send it through SPI interface
/* MMC/SD command (in SPI) */
#define SD_CMD0 	(0x40+0)	/* GO_IDLE_STATE */
#define SD_CMD1 	(0x40+1)	/* SEND_OP_COND (MMC) */
#define	SD_ACMD41	(0xC0+41)	/* SEND_OP_COND (SDC) */
#define SD_CMD8 	(0x40+8)	/* SEND_IF_COND */
#define SD_CMD9 	(0x40+9)	/* SEND_CSD */
#define SD_CMD10	(0x40+10)	/* SEND_CID */
#define SD_CMD12	(0x40+12)	/* STOP_TRANSMISSION */
#define SD_ACMD13	(0xC0+13)	/* SD_STATUS (SDC) */
#define SD_CMD16	(0x40+16)	/* SET_BLOCKLEN */
#define SD_CMD17	(0x40+17)	/* READ_SINGLE_BLOCK */
#define SD_CMD18	(0x40+18)	/* READ_MULTIPLE_BLOCK */
#define SD_CMD23	(0x40+23)	/* SET_BLOCK_COUNT (MMC) */
#define	SD_ACMD23	(0xC0+23)	/* SET_WR_BLK_ERASE_COUNT (SDC) */
#define SD_CMD24	(0x40+24)	/* WRITE_BLOCK */
#define SD_CMD25	(0x40+25)	/* WRITE_MULTIPLE_BLOCK */
#define SD_CMD55	(0x40+55)	/* APP_CMD */
#define SD_CMD58	(0x40+58)	/* READ_OCR */

/*
Command    Argument              Type  Description
CMD0       None                  R1    Tell the card to reset and enter its idle state.
CMD16      32-bit Block Length   R1    Select the block length.
CMD17      32-bit Block Address  R1    Read a single block.
CMD24      32-bit Block Address  R1    Write a single block.
CMD55      None                  R1    Next command will be application-specific (ACMDXX).
CMD58      None                  R3    Read OCR (Operating Conditions Register).
ACMD41     None                  R1    Initialize the card.

3 types of reply:
R1 : 1 byte :
bit
7 : always 0
6 : parameter error
5 : adress error
4 : erase seq num
3 : crc error
2 : illegal command
1 : erase resed
0 : in idle state

R2 : 2 byte (not interrested)

R3 : 5 bytes :
bit (byte 1) :
7 : always 0
6 : parameter error
5 : adress error
4 : erase seq num
3 : crc error
2 : illegal command
1 : erase resed
0 : in idle state
bytes 2-5 : operating condition register, MSB first
*/

#include "../../version/ver_mdriver_ram.h"
#if VER_MDRIVER_RAM_MAJOR != 1 || VER_MDRIVER_RAM_MINOR != 2
 #error Incompatible MDRIVER_RAM version number!
#endif

typedef enum {
	SD_RESPONSE_R1 = 1,
	SD_RESPONSE_R2 = 1,
	SD_RESPONSE_R3 = 3
} sd_response_type;


static int sd_vol0_card_present(void) {
	return (GPIO_PortInGet(gpioPortC) & 0x8);//pin = PC3
}
/*
 * transform a 32 bit value into a 4 bytes array
 */
static void sd_packarg(unsigned char *argument, unsigned long value)
{
  argument[3] = (unsigned char)(value >> 24);
  argument[2] = (unsigned char)(value >> 16);
  argument[1] = (unsigned char)(value >> 8);
  argument[0] = (unsigned char)(value);
}

typedef struct {
  unsigned long  maxsector;
  int            use;
  struct {
    uint8_t spiNumber;
    uint8_t location;
    int (*is_sd_card_present) (void);
    void (*spi_setupRXInt) (char* receiveBuffer, int bytesToReceive);
    void (*spi_setupTXInt) (char* transmitBuffer, int transmitBufferSize);
    void (*usart_wait_TX_finished) (void);
    void (*usart_wait_RX_finished) (void);
  } usart_data;
  F_DRIVER     * driver;
} t_SdDrv;

static F_DRIVER  t_drivers[1];

static t_SdDrv  SdDrv[1] =
{
  { ( MDRIVER_SD_VOLUME0_SIZE / MDRIVER_SD_SECTOR_SIZE ), 0,
           {1, 1, sd_vol0_card_present, SPI1_setupRXInt, SPI1_setupTXInt, USART1_Wait_TX_finished, USART1_Wait_RX_finished},
           &t_drivers[0] }
};
static int sd_card_send_command(F_DRIVER * driver, unsigned char command, unsigned char response_type, unsigned char* argument, unsigned char* response) {
  t_SdDrv* pSdDrv = (t_SdDrv*) driver->user_ptr;
  char txBuffer[5];
  char rxBuffer[5];
  int txSize;
  int rxSize;
  int i;
  if (response_type == SD_RESPONSE_R1) {
    rxSize = 1;
  } else if (response_type == SD_RESPONSE_R2) {
    rxSize = 2;
  } else {
    rxSize = 5;
  }
  if (argument == NULL) {
    txSize = 1;
  } else {
    txSize = 5;
  }
  txBuffer[0] = command;
  for (i = 1 ; i < txSize ; i++) {
    txBuffer[i] = argument[i - 1];
  }
  pSdDrv->usart_data.spi_setupRXInt(rxBuffer, rxSize);
  pSdDrv->usart_data.spi_setupTXInt(txBuffer, txSize);

  pSdDrv->usart_data.usart_wait_TX_finished();
  pSdDrv->usart_data.usart_wait_RX_finished();
  if (response != NULL) {
    for (i = 1 ; i < rxSize ; i++) {
      response[i -1] = rxBuffer[i];
    }
  }
  return 1;//TODO: we could do check on bit 7 of 1st rx byte at least....
}

static int sd_readsector ( F_DRIVER * driver, void * data, unsigned long sector ) {
  char arg_sector_adress[4];
  sd_packarg((unsigned char*) arg_sector_adress, sector);
  return sd_card_send_command(driver, SD_CMD17, SD_RESPONSE_R3, (unsigned char*) arg_sector_adress, (unsigned char*) data);
}
static int sd_writesector ( F_DRIVER * driver, void * data, unsigned long sector ) {
  t_SdDrv* pSdDrv = (t_SdDrv*) driver->user_ptr;
  char arg_sector_adress[4];
  //first, transmit the adress where to write
  sd_packarg((unsigned char*) arg_sector_adress, sector);
  if (sd_card_send_command(driver, SD_CMD17, SD_RESPONSE_R3, (unsigned char*) arg_sector_adress, NULL) != 0) {
    return -1;
  }
  //now, transmit the data to write
  pSdDrv->usart_data.spi_setupTXInt(data, 4);
  pSdDrv->usart_data.usart_wait_RX_finished();
  return 1;
}
static int sd_getphy ( F_DRIVER * driver, F_PHY * phy ) {
  t_SdDrv * p = (t_SdDrv *)( driver->user_ptr );

  if (p->usart_data.is_sd_card_present() != 0) {
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
    char commandCRC = 0x95;
    char arg_block_length[4];
    
    GPIO_Mode_TypeDef gpioModeSD_EN = gpioModeInput;
    GPIO_PinModeSet(gpioPortC, 3, gpioModeSD_EN, 0); /* init pin SD_EN to check if SD is plugged */
    
    //TODO: check specif of SPI protocol and init SD through SPI protocol
    /* Enabling clock to USART 1*/
    CMU_ClockEnable(cmuClock_USART1, true);
    SPI_setup(1, 1, 1);//init SD SPI
    //TODO: check specif of SD protocol and init SD through SD protocol
    //TODO: goto to IDLE state?
    SPI1_setupTXInt(&commandCRC, 1);//no reply to answer from the CRC command
    USART1_Wait_TX_finished();
    sd_packarg((unsigned char*) arg_block_length, MDRIVER_SD_SECTOR_SIZE);//set sector size
    sd_card_send_command(p, SD_CMD16, SD_RESPONSE_R1, arg_block_length, NULL);

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

