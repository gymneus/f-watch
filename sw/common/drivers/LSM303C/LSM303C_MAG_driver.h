/******************** (C) COPYRIGHT 2013 STMicroelectronics ********************
* File Name          : LSM303C_MAG_driver.h
* Author             : MSH Application Team
* Author             : Alberto Zancanato
* Version            : ver 1.1
* Date               : 09/2013
* Description        : Descriptor Header for LSM303C_MAG driver file
*
* HISTORY:
* Date            | Modification                    | Author
* 08/2012         |	Initial Revision                |	Alberto Zancanato
*
********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*
* THIS SOFTWARE IS SPECIFICALLY DESIGNED FOR EXCLUSIVE USE WITH ST PARTS.
*
*******************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __LSM303C_MAG_DRIVER__H
#define __LSM303C_MAG_DRIVER__H

/* Includes ------------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/

//these could change accordingly with the architecture

#ifndef __ARCHDEP__TYPES
#define __ARCHDEP__TYPES
typedef unsigned char u8_t;
typedef unsigned short int u16_t;
typedef short int i16_t;
typedef signed char i8_t;
#endif /*__ARCHDEP__TYPES*/

typedef u8_t LSM303C_MAG_Int1PinConf_t;
typedef u8_t LSM303C_MAG_Int2PinConf_t;
typedef u8_t LSM303C_MAG_Int1Conf_t;
typedef u8_t LSM303C_MAG_Axis_t;

/* Exported common structure --------------------------------------------------------*/

#ifndef __SHARED__TYPES
#define __SHARED__TYPES

typedef enum {
  MEMS_SUCCESS				=		0x01,
  MEMS_ERROR				=		0x00	
} status_t;

typedef enum {
  MEMS_ENABLE				=		0x01,
  MEMS_DISABLE				=		0x00	
} State_t;

typedef struct {
  i16_t AXIS_X;
  i16_t AXIS_Y;
  i16_t AXIS_Z;
} AxesRaw_t;

typedef u8_t IntPinConf_t;
typedef u8_t Axis_t;
typedef u8_t IntConf_t;

#endif /*__SHARED__TYPES*/
typedef enum { 
	LSM303C_MAG_ST_DISABLE 	=	 0x00,
	LSM303C_MAG_ST_ENABLE 	=	 0x01
} LSM303C_MAG_ST_t;

typedef enum { 
	LSM303C_MAG_DO_0_625_Hz 	=	 0x00,
	LSM303C_MAG_DO_1_25_Hz 	=	 0x04,
	LSM303C_MAG_DO_2_5_Hz 	=	 0x08,
	LSM303C_MAG_DO_5_Hz 	=	 0x0C,
	LSM303C_MAG_DO_10_Hz 	=	 0x10,
	LSM303C_MAG_DO_20_hZ 	=	 0x14,
	LSM303C_MAG_DO_40_Hz 	=	 0x18,
	LSM303C_MAG_DO_80_Hz 	=	 0x1C
} LSM303C_MAG_DO_t;

typedef enum { 
	LSM303C_MAG_OMXY_LOW_POWER 	=	 0x00,
	LSM303C_MAG_OMXY_MEDIUM_PERFORMANCE =		 0x20,
	LSM303C_MAG_OMXY_HIGH_PERFORMANCE 	=	 0x40,
	LSM303C_MAG_OMXY_ULTRA_HIGH_PERFORMANCE= 		 0x60
} LSM303C_MAG_OMXY_t;

typedef enum { 
	LSM303C_MAG_TEMP_EN_DISABLE =		 0x00,
	LSM303C_MAG_TEMP_EN_ENABLE 	=	 0x80
} LSM303C_MAG_TEMP_EN_t;

typedef enum { 
	LSM303C_MAG_SOFT_RST_DISABLE =		 0x00,
	LSM303C_MAG_SOFT_RST_ENABLE =		 0x04
} LSM303C_MAG_SOFT_RST_t;

typedef enum { 
	LSM303C_MAG_REBOOT_NO 	=	 0x00,
	LSM303C_MAG_REBOOT_YES 	=	 0x08
} LSM303C_MAG_REBOOT_t;

typedef enum { 
	LSM303C_MAG_FS_4_Ga 	=	 0x00,
	LSM303C_MAG_FS_8_Ga 	=	 0x20,
	LSM303C_MAG_FS_12_Ga 	=	 0x40,
	LSM303C_MAG_FS_16_Ga 	=	 0x60
} LSM303C_MAG_FS_t;

typedef enum { 
	LSM303C_MAG_MD_CONTINUOUS 	=	 0x00,
	LSM303C_MAG_MD_SINGLE 	=	 0x01,
	LSM303C_MAG_MD_POWER_DOWN_1 =		 0x02,
	LSM303C_MAG_MD_POWER_DOWN_2 =		 0x03
} LSM303C_MAG_MD_t;

typedef enum { 
	LSM303C_MAG_SIM_4_WIRE =		 0x00,
	LSM303C_MAG_SIM_3_WIRE =		 0x04
} LSM303C_MAG_SIM_t;

typedef enum { 
	LSM303C_MAG_LP_DISABLE =		 0x00,
	LSM303C_MAG_LP_ENABLE =		 0x20
} LSM303C_MAG_LP_t;

typedef enum { 
	LSM303C_MAG_BLE_LSb_LOW= 		 0x00,
	LSM303C_MAG_BLE_MSb_LOW =		 0x02
} LSM303C_MAG_BLE_t;

typedef enum { 
	LSM303C_MAG_OMZ_LOW_PW =		 0x00,
	LSM303C_MAG_OMZ_MEDIUM_PERFORMANCE 	=	 0x04,
	LSM303C_MAG_OMZ_HIGH_PERFORMANCE 	=	 0x08,
	LSM303C_MAG_OMZ_ULTRA_HIGH_PERFORMANCE= 		 0x0C
} LSM303C_MAG_OMZ_t;

typedef enum { 
	LSM303C_MAG_BDU_DISABLE 		= 0x00,
	LSM303C_MAG_BDU_ENABLE 		= 0x40
} LSM303C_MAG_BDU_t;

typedef enum { 
	LSM303C_MAG_XDA_NO 		= 0x00,
	LSM303C_MAG_XDA_YES 	=	 0x01
} LSM303C_MAG_XDA_t;

typedef enum { 
	LSM303C_MAG_YDA_NO 		= 0x00,
	LSM303C_MAG_YDA_YES 	=	 0x02
} LSM303C_MAG_YDA_t;

typedef enum { 
	LSM303C_MAG_ZDA_NO 		 =0x00,
	LSM303C_MAG_ZDA_YES =		 0x04
} LSM303C_MAG_ZDA_t;

typedef enum { 
	LSM303C_MAG_XYZDA_NO =		 0x00,
	LSM303C_MAG_XYZDA_YES =		 0x08
} LSM303C_MAG_XYZDA_t;

typedef enum { 
	LSM303C_MAG_XOR_NO 	=	 0x00,
	LSM303C_MAG_XOR_YES =		 0x10
} LSM303C_MAG_XOR_t;

typedef enum { 
	LSM303C_MAG_YOR_NO 	=	 0x00,
	LSM303C_MAG_YOR_YES =		 0x20
} LSM303C_MAG_YOR_t;

typedef enum { 
	LSM303C_MAG_ZOR_NO 	=	 0x00,
	LSM303C_MAG_ZOR_YES =		 0x40
} LSM303C_MAG_ZOR_t;

typedef enum { 
	LSM303C_MAG_XYZOR_NO =		 0x00,
	LSM303C_MAG_XYZOR_YES =		 0x80
} LSM303C_MAG_XYZOR_t;

typedef enum { 
	LSM303C_MAG_INTERRUPT_DISABLE =		 0x00,
	LSM303C_MAG_INTERRUPT_ENABLE =		 0x01
} LSM303C_MAG_IEN_t;

typedef enum { 
	LSM303C_MAG_LIR_LATCH 	=	 0x00,
	LSM303C_MAG_LIR_NOT_LATCH 	=	 0x02
} LSM303C_MAG_LIR_t;

typedef enum { 
	LSM303C_MAG_IEA_LOW 	=	 0x00,
	LSM303C_MAG_IEA_HIGH 	=	 0x04
} LSM303C_MAG_IEA_t;

typedef enum { 
	LSM303C_MAG_ZIEN_DISABLE 	=	 0x00,
	LSM303C_MAG_ZIEN_ENABLE 	=	 0x20
} LSM303C_MAG_ZIEN_t;

typedef enum { 
	LSM303C_MAG_YIEN_DISABLE 	=	 0x00,
	LSM303C_MAG_YIEN_ENABLE 	=	 0x40
} LSM303C_MAG_YIEN_t;

typedef enum { 
	LSM303C_MAG_XIEN_DISABLE 	=	 0x00,
	LSM303C_MAG_XIEN_ENABLE 	=	 0x80
} LSM303C_MAG_XIEN_t;

typedef enum { 
	LSM303C_MAG_INT_FLAG_DOWN 	=	 0x00,
	LSM303C_MAG_INT_FLAG_UP 	=	 0x01
} LSM303C_MAG_INT_t;

typedef enum { 
	LSM303C_MAG_MROI_NO_OVERFLOW =		 0x00,
	LSM303C_MAG_MROI_OVERFLOW 	=	 0x02
} LSM303C_MAG_MROI_t;

typedef enum { 
	LSM303C_MAG_NTH_Z_NOT_PASS 	=	 0x00,
	LSM303C_MAG_NTH_Z_PASS 	=	 0x04
} LSM303C_MAG_NTH_Z_t;

typedef enum { 
	LSM303C_MAG_NTH_Y_NOT_PASS 	=	 0x00,
	LSM303C_MAG_NTH_Y_PASS 	=	 0x08
} LSM303C_MAG_NTH_Y_t;

typedef enum { 
	LSM303C_MAG_NTH_X_NOT_PASS 	=	 0x00,
	LSM303C_MAG_NTH_X_PASS 	=	 0x10
} LSM303C_MAG_NTH_X_t;

typedef enum { 
	LSM303C_MAG_PTH_Z_NOT_PASS 	=	 0x00,
	LSM303C_MAG_PTH_Z_PASS 	=	 0x20
} LSM303C_MAG_PTH_Z_t;

typedef enum { 
	LSM303C_MAG_PTH_Y_NOT_PASS =		 0x00,
	LSM303C_MAG_PTH_Y_PASS 	=	 0x40
} LSM303C_MAG_PTH_Y_t;

typedef enum { 
	LSM303C_MAG_PTH_X_NOT_PASS 	=	 0x00,
	LSM303C_MAG_PTH_X_PASS 	=	 0x80
} LSM303C_MAG_PTH_X_t;

/* Registers Name ------------------------------------------------------------------------*/

#define 	LSM303C_MAG_WHO_AM__I_		 0x0F
#define 	LSM303C_MAG_CTRL_REG1		 0x20
#define 	LSM303C_MAG_CTRL_REG2		 0x21
#define 	LSM303C_MAG_CTRL_REG3		 0x22
#define 	LSM303C_MAG_CTRL_REG4		 0x23
#define 	LSM303C_MAG_CTRL_REG5		 0x24
#define 	LSM303C_MAG_STATUS_REG		 0x27
#define 	LSM303C_MAG_OUTX_L		 0x28
#define 	LSM303C_MAG_OUTX_H		 0x29
#define 	LSM303C_MAG_OUTY_L		 0x2A
#define 	LSM303C_MAG_OUTY_H		 0x2B
#define 	LSM303C_MAG_OUTZ_L		 0x2C
#define 	LSM303C_MAG_OUTZ_H		 0x2D
#define 	LSM303C_MAG_TEMP_OUT_L		 0x2E
#define 	LSM303C_MAG_TEMP_OUT_H		 0x2F
#define 	LSM303C_MAG_INT_CFG		 0x30
#define 	LSM303C_MAG_INT_SRC		 0x31
#define 	LSM303C_MAG_INT_THS_L		 0x32
#define 	LSM303C_MAG_INT_THS_H		 0x33
/* Exported macro ------------------------------------------------------------*/

#ifndef __SHARED__MACROS

#define __SHARED__MACROS
#define ValBit(VAR,Place)         (VAR & (1<<Place))
#define BIT(x) ( (x) )

#endif /*__SHARED__MACROS*/

/* Exported constants --------------------------------------------------------*/

#ifndef __SHARED__CONSTANTS
#define __SHARED__CONSTANTS

#define MEMS_SET                                        0x01
#define MEMS_RESET                                      0x00

#endif /*__SHARED__CONSTANTS*/

/************** I2C Address *****************/

#define LSM303C_MAG_MEMS_I2C_ADDRESS         0x3C

/* Exported functions --------------------------------------------------------*/

/**********Sensor Configuration Functions***********/
status_t LSM303C_MAG_SetODR(LSM303C_MAG_DO_t ov);
status_t LSM303C_MAG_SetFullScale(LSM303C_MAG_FS_t ov);
status_t LSM303C_MAG_SetMode(LSM303C_MAG_MD_t ov);
status_t LSM303C_MAG_BlockDataUpdate(LSM303C_MAG_BDU_t ov);
status_t LSM303C_MAG_TemperatureEN(LSM303C_MAG_TEMP_EN_t ov);
status_t LSM303C_MAG_XY_AxOperativeMode(LSM303C_MAG_OMXY_t ov);
status_t LSM303C_MAG_Z_AxOperativeMode(LSM303C_MAG_OMZ_t ov);

/***************Interrupt Functions****************/
status_t LSM303C_MAG_EnableINTpin(LSM303C_MAG_IEN_t ov);
status_t LSM303C_MAG_INTpinLatched(LSM303C_MAG_LIR_t ov);
status_t LSM303C_MAG_INTpinHighLow(LSM303C_MAG_IEA_t ov);
status_t LSM303C_MAG_Z_AxINT_Enable(LSM303C_MAG_ZIEN_t ov);
status_t LSM303C_MAG_Y_AxINT_Enable(LSM303C_MAG_YIEN_t ov);
status_t LSM303C_MAG_X_AxINT_Enable(LSM303C_MAG_XIEN_t ov);
status_t LSM303C_MAG_InterruptFlag(LSM303C_MAG_INT_t *value);

status_t LSM303C_MAG_Z_NegativeThresholdFlag(LSM303C_MAG_NTH_Z_t *value);
status_t LSM303C_MAG_Y_NegativeThresholdFlag(LSM303C_MAG_NTH_Y_t *value);
status_t LSM303C_MAG_X_NegativeThresholdFlag(LSM303C_MAG_NTH_X_t *value);
status_t LSM303C_MAG_Z_PositiveThresholdFlag(LSM303C_MAG_PTH_Z_t *value);
status_t LSM303C_MAG_Y_PositiveThresholdFlag(LSM303C_MAG_PTH_Y_t *value);
status_t LSM303C_MAG_X_PositiveThresholdFlag(LSM303C_MAG_PTH_X_t *value);
status_t LSM303C_MAG_SetThreshold(u16_t buff);

status_t LSM303C_MAG_X_AxDataAvailable(LSM303C_MAG_XDA_t *value);
status_t LSM303C_MAG_Y_AxDataAvailable(LSM303C_MAG_YDA_t *value);
status_t LSM303C_MAG_Z_AxDataAvailable(LSM303C_MAG_ZDA_t *value);
status_t LSM303C_MAG_XYZ_AxDataAvailable(LSM303C_MAG_XYZDA_t *value);
status_t LSM303C_MAG_X_AxisOverrun(LSM303C_MAG_XOR_t *value);
status_t LSM303C_MAG_Y_AxisOverrun(LSM303C_MAG_YOR_t *value);
status_t LSM303C_MAG_Z_AxisOverrun(LSM303C_MAG_ZOR_t *value);
status_t LSM303C_MAG_Z_AxisOverrun(LSM303C_MAG_ZOR_t *value);
status_t LSM303C_MAG_XYZ_AxisOverrun(LSM303C_MAG_XYZOR_t *value);
status_t LSM303C_MAG_MeasOverFlow(LSM303C_MAG_MROI_t *value);

/***************Utility Functions****************/
status_t LSM303C_MAG_SoftReset(LSM303C_MAG_SOFT_RST_t ov);
status_t LSM303C_MAG_Reboot(LSM303C_MAG_REBOOT_t ov);
status_t LSM303C_MAG_SelfTest(LSM303C_MAG_ST_t ov);
status_t LSM303C_MAG_LowPowerMode(LSM303C_MAG_LP_t ov);
status_t LSM303C_MAG_EndianDataFormat(LSM303C_MAG_BLE_t ov);
status_t LSM303C_MAG_SerialInterfaceMode(LSM303C_MAG_SIM_t ov);

/****************Reading Functions*****************/
status_t LSM303C_MAG_GetMagRaw(AxesRaw_t* buff);
status_t LSM303C_MAG_GetTemperatureRaw(u16_t* buff);

/*********************Generic*********************/
u8_t LSM303C_MAG_ReadReg(u8_t Reg, u8_t* Data);
u8_t LSM303C_MAG_WriteReg(u8_t Reg, u8_t Data);

#endif /* __LSM303C_MAG_H */

/******************* (C) COPYRIGHT 2013 STMicroelectronics *****END OF FILE****/



