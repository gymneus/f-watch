/*
 * TODO : license
 */

//TODO: use those CMD to send it through SPI interface
/* MMC/SD command (in SPI) */
#define USD_CMD0	(0x40+0)	/* GO_IDLE_STATE */
#define USD_CMD1	(0x40+1)	/* SEND_OP_COND (MMC) */
#define	USD_ACMD41	(0xC0+41)	/* SEND_OP_COND (SDC) */
#define USD_CMD8	(0x40+8)	/* SEND_IF_COND */
#define USD_CMD9	(0x40+9)	/* SEND_CSD */
#define USD_CMD10	(0x40+10)	/* SEND_CID */
#define USD_CMD12	(0x40+12)	/* STOP_TRANSMISSION */
#define USD_ACMD13	(0xC0+13)	/* SD_STATUS (SDC) */
#define USD_CMD16	(0x40+16)	/* SET_BLOCKLEN */
#define USD_CMD17	(0x40+17)	/* READ_SINGLE_BLOCK */
#define USD_CMD18	(0x40+18)	/* READ_MULTIPLE_BLOCK */
#define USD_CMD23	(0x40+23)	/* SET_BLOCK_COUNT (MMC) */
#define	USD_ACMD23	(0xC0+23)	/* SET_WR_BLK_ERASE_COUNT (SDC) */
#define USD_CMD24	(0x40+24)	/* WRITE_BLOCK */
#define USD_CMD25	(0x40+25)	/* WRITE_MULTIPLE_BLOCK */
#define USD_CMD55	(0x40+55)	/* APP_CMD */
#define USD_CMD58	(0x40+58)	/* READ_OCR */

static int sd_readsector ( F_DRIVER * driver, void * data, unsigned long sector ) {
	//TODO:implement sd_readsector
	return -1;
}
static int sd_writesector ( F_DRIVER * driver, void * data, unsigned long sector ) {
	//TODO:implement sd_writesector
	return -1;
}
static int sd_getphy ( F_DRIVER * driver, F_PHY * phy ) {
	//TODO:implement sd_getphy
	return -1;
}
static void sd_release ( F_DRIVER * driver ) {
	//TODO:implement sd_release
	return;
}
F_DRIVER * sd_initfunc ( unsigned long driver_param ) {
	//TODO:implement sd_initfunc
  /* Enabling clock to USART 0*/
	CMU_ClockEnable(cmuClock_USART0, true);
	SPI_setup(USART0_NUM, GPIO_POS1, false);
	return NULL;
}
