/******************** (C) COPYRIGHT 2013 STMicroelectronics ********************
* File Name          : LSM303C_MAG.c
* Author             : MSH Application Team
* Author             : Alberto Zancanato
* Version            : ver 1.1
* Date               : 09/2013
* Description        : LSM303C_MAG driver file
*                      
* HISTORY:
* Date               |	Modification                    |	Author
* 08/2012         	 |	Initial Revision                |	Alberto Zancanato
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

/* Includes ------------------------------------------------------------------*/
#include "LSM303C_MAG_driver.h"
#include "lsm303c.h"
//EXAMPLE used to fill LSM303C_MAG_ReadReg and LSM303C_MAG_WriteReg
//#include "i2C_mems.h"	
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define LSM303C_MAG_flags_BLE	0x01
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
u8_t LSM303C_MAG_flags=0;
/* Private function prototypes -----------------------------------------------*/

/*******************************************************************************
* Function Name		: LSM303C_MAG_ReadReg
* Description		: Generic Reading function. It must be fullfilled with either
*					: I2C or SPI reading functions					
* Input				: Register Address
* Output			: Data REad
* Return			: None
*******************************************************************************/
u8_t LSM303C_MAG_ReadReg(u8_t Reg, u8_t* Data)
{
  return spi_read(DEV_MAG, Reg, Data);
}

/*******************************************************************************
* Function Name		: LSM303C_MAG_WriteReg
* Description		: Generic Writing function. It must be fullfilled with either
*					: I2C or SPI writing function
* Input				: Register Address, Data to be written
* Output			: None
* Return			: None
*******************************************************************************/
u8_t LSM303C_MAG_WriteReg(u8_t Reg, u8_t Data)
{
  spi_send(DEV_MAG, Reg, Data);
  return 1;
}
/* Private functions ---------------------------------------------------------*/


/*******************************************************************************
* Function Name  : LSM303C_MAG_SelfTest
* Description    : Enable/Disable LSM303C_MAG Self Test
* Input          : Self test Enable/Disable [LSM303C_MAG_ST_t]
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t LSM303C_MAG_SelfTest(LSM303C_MAG_ST_t ov){
  u8_t value;

  if( !LSM303C_MAG_ReadReg(LSM303C_MAG_CTRL_REG1, &value) )
    return MEMS_ERROR;

  value &= ~LSM303C_MAG_ST_ENABLE; //mask
  value |= ov;	

  if( !LSM303C_MAG_WriteReg(LSM303C_MAG_CTRL_REG1, value) )
    return MEMS_ERROR;

  return MEMS_SUCCESS;
}

/*******************************************************************************
* Function Name  : LSM303C_MAG_SetODR
* Description    : Sets LSM303C_MAG Output Data Rate
* Input          : Set Output data rate [LSM303C_MAG_DO_t]
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t LSM303C_MAG_SetODR(LSM303C_MAG_DO_t ov){
  u8_t value;

  if( !LSM303C_MAG_ReadReg(LSM303C_MAG_CTRL_REG1, &value) )
    return MEMS_ERROR;
	
  value &= ~LSM303C_MAG_DO_80_Hz; //mask
  value |= ov;		

  if( !LSM303C_MAG_WriteReg(LSM303C_MAG_CTRL_REG1, value) )
    return MEMS_ERROR;

  return MEMS_SUCCESS;
}

/*******************************************************************************
* Function Name  : LSM303C_MAG_XY_AxOperativeMode
* Description    : Sets LSM303C_MAG Output Data Rate
* Input          : Set Output data rate [LSM303C_MAG_OMXY_t]
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t LSM303C_MAG_XY_AxOperativeMode(LSM303C_MAG_OMXY_t ov){
  u8_t value;

  if( !LSM303C_MAG_ReadReg(LSM303C_MAG_CTRL_REG1, &value) )
    return MEMS_ERROR;
	
  value &= ~LSM303C_MAG_OMXY_ULTRA_HIGH_PERFORMANCE; //mask
  value |= ov;	

  if( !LSM303C_MAG_WriteReg(LSM303C_MAG_CTRL_REG1, value) )
    return MEMS_ERROR;

  return MEMS_SUCCESS;
}

/*******************************************************************************
* Function Name  : LSM303C_MAG_TemperatureEN
* Description    : Enable/Disable LSM303C_MAG Temperature
* Input          : Temperature Enable/Disable [LSM303C_MAG_TEMP_EN_t]
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t LSM303C_MAG_TemperatureEN(LSM303C_MAG_TEMP_EN_t ov){
  u8_t value;

  if( !LSM303C_MAG_ReadReg(LSM303C_MAG_CTRL_REG1, &value) )
    return MEMS_ERROR;

  value &= ~LSM303C_MAG_TEMP_EN_ENABLE; //mask
  value |= ov;	

  if( !LSM303C_MAG_WriteReg(LSM303C_MAG_CTRL_REG1, value) )
    return MEMS_ERROR;

  return MEMS_SUCCESS;
}

/*******************************************************************************
* Function Name  : LSM303C_MAG_SoftReset
* Description    : Enable/Disable LSM303C_MAG SoftReset
* Input          : SoftReset Enable/Disable [LSM303C_MAG_SOFT_RST_t]
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t LSM303C_MAG_SoftReset(LSM303C_MAG_SOFT_RST_t ov){
  u8_t value;

  if( !LSM303C_MAG_ReadReg(LSM303C_MAG_CTRL_REG2, &value) )
    return MEMS_ERROR;

  value &= ~LSM303C_MAG_SOFT_RST_ENABLE; //mask
  value |= ov;	

  if( !LSM303C_MAG_WriteReg(LSM303C_MAG_CTRL_REG2, value) )
    return MEMS_ERROR;

  return MEMS_SUCCESS;
}


/*******************************************************************************
* Function Name  : LSM303C_MAG_Reboot
* Description    : Enable/Disable LSM303C_MAG Reboot
* Input          : Reboot Enable/Disable [LSM303C_MAG_REBOOT_t]
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t LSM303C_MAG_Reboot(LSM303C_MAG_REBOOT_t ov){
  u8_t value;

  if( !LSM303C_MAG_ReadReg(LSM303C_MAG_CTRL_REG2, &value) )
    return MEMS_ERROR;

  value &= ~LSM303C_MAG_REBOOT_YES; //mask
  value |= ov;		

  if( !LSM303C_MAG_WriteReg(LSM303C_MAG_CTRL_REG2, value) )
    return MEMS_ERROR;

  return MEMS_SUCCESS;
}

/*******************************************************************************
* Function Name  : LSM303C_MAG_SetFullScale
* Description    : set LSM303C_MAG Full scale
* Input          : set LSM303C_MAG Full scale [LSM303C_MAG_FS_t]
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t LSM303C_MAG_SetFullScale(LSM303C_MAG_FS_t ov){
  u8_t value;

  if( !LSM303C_MAG_ReadReg(LSM303C_MAG_CTRL_REG2, &value) )
    return MEMS_ERROR;

  value &= ~LSM303C_MAG_FS_16_Ga; //mask
  value |= ov;	

  if( !LSM303C_MAG_WriteReg(LSM303C_MAG_CTRL_REG2, value) )
    return MEMS_ERROR;

  return MEMS_SUCCESS;
}

/*******************************************************************************
* Function Name  : LSM303C_MAG_SetMode
* Description    : set LSM303C_MAG Mode
* Input          : set LSM303C_MAG Mode [LSM303C_MAG_MD_t]
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t LSM303C_MAG_SetMode(LSM303C_MAG_MD_t ov){
  u8_t value;

  if( !LSM303C_MAG_ReadReg(LSM303C_MAG_CTRL_REG3, &value) )
    return MEMS_ERROR;

  value &= ~LSM303C_MAG_MD_POWER_DOWN_2; //mask
  value |= ov;		

  if( !LSM303C_MAG_WriteReg(LSM303C_MAG_CTRL_REG3, value) )
    return MEMS_ERROR;

  return MEMS_SUCCESS;
}

/*******************************************************************************
* Function Name  : LSM303C_MAG_SerialInterfaceMode
* Description    : set LSM303C_MAG SerialInterfaceMode
* Input          : set LSM303C_MAG SerialInterfaceMode [LSM303C_MAG_SIM_t]
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t LSM303C_MAG_SerialInterfaceMode(LSM303C_MAG_SIM_t ov){
  u8_t value;

  if( !LSM303C_MAG_ReadReg(LSM303C_MAG_CTRL_REG3, &value) )
    return MEMS_ERROR;

	
  value &= ~LSM303C_MAG_SIM_3_WIRE; //mask
  value |= ov;	

  if( !LSM303C_MAG_WriteReg(LSM303C_MAG_CTRL_REG3, value) )
    return MEMS_ERROR;

  return MEMS_SUCCESS;
}

/*******************************************************************************
* Function Name  : LSM303C_MAG_LowPowerMode
* Description    : set LSM303C_MAG LowPowerMode
* Input          : set LSM303C_MAG LowPowerMode [LSM303C_MAG_LP_t]
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
//#WARNING: If you enable the LP bit you change the ODR and all the axis Operative Mode
status_t LSM303C_MAG_LowPowerMode(LSM303C_MAG_LP_t ov){
  u8_t value;

  if( !LSM303C_MAG_ReadReg(LSM303C_MAG_CTRL_REG3, &value) )
    return MEMS_ERROR;

  value &= ~LSM303C_MAG_LP_ENABLE; //mask
  value |= ov;		

  if( !LSM303C_MAG_WriteReg(LSM303C_MAG_CTRL_REG3, value) )
    return MEMS_ERROR;

  return MEMS_SUCCESS;
}

/*******************************************************************************
* Function Name  : LSM303C_MAG_EndianDataFormat
* Description    : set LSM303C_MAG EndianDataFormat
* Input          : set LSM303C_MAG EndianDataFormat [LSM303C_MAG_BLE_t]
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
//#WARNING: If you Use this function you have to change the order of the output register
status_t LSM303C_MAG_EndianDataFormat(LSM303C_MAG_BLE_t ov){
  u8_t value;

  if( !LSM303C_MAG_ReadReg(LSM303C_MAG_CTRL_REG4, &value) )
    return MEMS_ERROR;

  value &= ~LSM303C_MAG_BLE_MSb_LOW; //mask
  value |= ov;	
  
  if (ov) LSM303C_MAG_flags|=LSM303C_MAG_flags_BLE;
  else LSM303C_MAG_flags&=~LSM303C_MAG_flags_BLE;
  
  if( !LSM303C_MAG_WriteReg(LSM303C_MAG_CTRL_REG4, value) )
    return MEMS_ERROR;

  return MEMS_SUCCESS;
}

/*******************************************************************************
* Function Name  : LSM303C_MAG_Z_AxOperativeMode
* Description    : set LSM303C_MAG Z axis operative mode
* Input          : set LSM303C_MAG Z axis operative mode [LSM303C_MAG_OMZ_t]
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/

status_t LSM303C_MAG_Z_AxOperativeMode(LSM303C_MAG_OMZ_t ov){
  u8_t value;

  if( !LSM303C_MAG_ReadReg(LSM303C_MAG_CTRL_REG4, &value) )
    return MEMS_ERROR;

  value &= ~LSM303C_MAG_OMZ_ULTRA_HIGH_PERFORMANCE; //mask
  value |= ov;	

  if( !LSM303C_MAG_WriteReg(LSM303C_MAG_CTRL_REG4, value) )
    return MEMS_ERROR;

  return MEMS_SUCCESS;
}

/*******************************************************************************
* Function Name  : LSM303C_MAG_BlockDataUpdate
* Description    : Enable/Disable LSM303C_MAG Z BlockDataUpdate
* Input          : Enable/Disable LSM303C_MAG Z BlockDataUpdate[LSM303C_MAG_BDU_t]
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/

status_t LSM303C_MAG_BlockDataUpdate(LSM303C_MAG_BDU_t ov){
  u8_t value;

  if( !LSM303C_MAG_ReadReg(LSM303C_MAG_CTRL_REG5, &value) )
    return MEMS_ERROR;


  value &= ~LSM303C_MAG_BDU_ENABLE; //mask
  value |= ov;		

  if( !LSM303C_MAG_WriteReg(LSM303C_MAG_CTRL_REG5, value) )
    return MEMS_ERROR;

  return MEMS_SUCCESS;
}

/*******************************************************************************
* Function Name  : LSM303C_MAG_X_AxDataAvailable
* Description    : LSM303C_MAG X Axis data available
* Input          : pointer to LSM303C_MAG_XDA_t
* Output         : LSM303C_MAG X Axis data available[LSM303C_MAG_XDA_t]
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/

status_t LSM303C_MAG_X_AxDataAvailable(LSM303C_MAG_XDA_t *value){

  if( !LSM303C_MAG_ReadReg(LSM303C_MAG_STATUS_REG, (u8_t *)value) )
    return MEMS_ERROR;

  *value &= LSM303C_MAG_XDA_YES; //mask

  return MEMS_SUCCESS;
}

/*******************************************************************************
* Function Name  : LSM303C_MAG_Y_AxDataAvailable
* Description    : LSM303C_MAG Y Axis data available
* Input          : pointer to LSM303C_MAG_YDA_t
* Output         : LSM303C_MAG Y Axis data available[LSM303C_MAG_YDA_t]
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/

status_t LSM303C_MAG_Y_AxDataAvailable(LSM303C_MAG_YDA_t *value){

  if( !LSM303C_MAG_ReadReg(LSM303C_MAG_STATUS_REG, (u8_t *)value) )
    return MEMS_ERROR;

  *value &= LSM303C_MAG_YDA_YES; //mask

  return MEMS_SUCCESS;
}

/*******************************************************************************
* Function Name  : LSM303C_MAG_Z_AxDataAvailable
* Description    : LSM303C_MAG Z Axis data available
* Input          : pointer to LSM303C_MAG_ZDA_t
* Output         : LSM303C_MAG Z Axis data available[LSM303C_MAG_ZDA_t]
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/

status_t LSM303C_MAG_Z_AxDataAvailable(LSM303C_MAG_ZDA_t *value){

  if( !LSM303C_MAG_ReadReg(LSM303C_MAG_STATUS_REG, (u8_t *)value) )
    return MEMS_ERROR;

  *value &= LSM303C_MAG_ZDA_YES; //mask

  return MEMS_SUCCESS;
}

/*******************************************************************************
* Function Name  : LSM303C_MAG_XYZ_AxDataAvailable
* Description    : LSM303C_MAG  Axis data available for all axis
* Input          : pointer to LSM303C_MAG_XYZDA_t
* Output         : LSM303C_MAG XYZ Axis data available[LSM303C_MAG_XYZDA_t]
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/

status_t LSM303C_MAG_XYZ_AxDataAvailable(LSM303C_MAG_XYZDA_t *value){

  if( !LSM303C_MAG_ReadReg(LSM303C_MAG_STATUS_REG, (u8_t *)value) )
    return MEMS_ERROR;

  *value &= LSM303C_MAG_XYZDA_YES; //mask

  return MEMS_SUCCESS;
}


/*******************************************************************************
* Function Name  : LSM303C_MAG_X_AxisOverrun
* Description    : LSM303C_MAG  X Data Axis Overrunning
* Input          : pointer to LSM303C_MAG_XOR_t
* Output         : LSM303C_MAG X Axis data overrun [LSM303C_MAG_XOR_t]
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/

status_t LSM303C_MAG_X_AxisOverrun(LSM303C_MAG_XOR_t *value){

  if( !LSM303C_MAG_ReadReg(LSM303C_MAG_STATUS_REG, (u8_t *)value) )
    return MEMS_ERROR;

  *value &= LSM303C_MAG_XOR_YES; //mask

  return MEMS_SUCCESS;
}

/*******************************************************************************
* Function Name  : LSM303C_MAG_Y_AxisOverrun
* Description    : LSM303C_MAG  Y Data Axis Overrunning
* Input          : pointer to LSM303C_MAG_YOR_t
* Output         : LSM303C_MAG Y Axis data overrun [LSM303C_MAG_YOR_t]
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/

status_t LSM303C_MAG_Y_AxisOverrun(LSM303C_MAG_YOR_t *value){

  if( !LSM303C_MAG_ReadReg(LSM303C_MAG_STATUS_REG, (u8_t *)value) )
    return MEMS_ERROR;

  *value &= LSM303C_MAG_YOR_YES; //mask

  return MEMS_SUCCESS;
}

/*******************************************************************************
* Function Name  : LSM303C_MAG_Z_AxisOverrun
* Description    : LSM303C_MAG  Z Data Axis Overrunning
* Input          : pointer to LSM303C_MAG_ZOR_t
* Output         : LSM303C_MAG Z Axis data overrun [LSM303C_MAG_ZOR_t]
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/

status_t LSM303C_MAG_Z_AxisOverrun(LSM303C_MAG_ZOR_t *value){

  if( !LSM303C_MAG_ReadReg(LSM303C_MAG_STATUS_REG, (u8_t *)value) )
    return MEMS_ERROR;

  *value &= LSM303C_MAG_ZOR_YES; //mask

  return MEMS_SUCCESS;
}

/*******************************************************************************
* Function Name  : LSM303C_MAG_XYZ_AxisOverrun
* Description    : LSM303C_MAG  XYZ Data Axis Overrunning
* Input          : pointer to LSM303C_MAG_XYZOR_t
* Output         : LSM303C_MAG XYZ Axis data overrun [LSM303C_MAG_XYZOR_t]
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/

status_t LSM303C_MAG_XYZ_AxisOverrun(LSM303C_MAG_XYZOR_t *value){

  if( !LSM303C_MAG_ReadReg(LSM303C_MAG_STATUS_REG, (u8_t *)value) )
    return MEMS_ERROR;

  *value &= LSM303C_MAG_XYZOR_YES; //mask

  return MEMS_SUCCESS;
}

/*******************************************************************************
* Function Name  : LSM303C_MAG_EnableINTpin
* Description    : Enable/Disable LSM303C_MAG INT pin
* Input          : Enable/Disable LSM303C_MAG INT pin[LSM303C_MAG_IEN_t]
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/

status_t LSM303C_MAG_EnableINTpin(LSM303C_MAG_IEN_t ov){
  u8_t value;

  if( !LSM303C_MAG_ReadReg(LSM303C_MAG_INT_CFG, &value) )
    return MEMS_ERROR;

  value &= ~LSM303C_MAG_INTERRUPT_ENABLE; //mask
  value |= ov;			

  if( !LSM303C_MAG_WriteReg(LSM303C_MAG_INT_CFG, value) )
    return MEMS_ERROR;

  return MEMS_SUCCESS;
}

/*******************************************************************************
* Function Name  : LSM303C_MAG_INTpinLatched
* Description    : Enable/Disable LSM303C_MAG INT latched
* Input          : Enable/Disable LSM303C_MAG INT latched[LSM303C_MAG_LIR_t]
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/

status_t LSM303C_MAG_INTpinLatched(LSM303C_MAG_LIR_t ov){
  u8_t value;

  if( !LSM303C_MAG_ReadReg(LSM303C_MAG_INT_CFG, &value) )
    return MEMS_ERROR;

  value &= ~LSM303C_MAG_LIR_NOT_LATCH; //mask
  value |= ov;	

  if( !LSM303C_MAG_WriteReg(LSM303C_MAG_INT_CFG, value) )
    return MEMS_ERROR;

  return MEMS_SUCCESS;
}

/*******************************************************************************
* Function Name  : LSM303C_MAG_INTpinHighLow
* Description    : LSM303C_MAG INT active low/high
* Input          : LSM303C_MAG INT active low/high[LSM303C_MAG_IEA_t]
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/

status_t LSM303C_MAG_INTpinHighLow(LSM303C_MAG_IEA_t ov){
  u8_t value;

  if( !LSM303C_MAG_ReadReg(LSM303C_MAG_INT_CFG, &value) )
    return MEMS_ERROR;

  value &= ~LSM303C_MAG_IEA_HIGH; //mask
  value |= ov;	

  if( !LSM303C_MAG_WriteReg(LSM303C_MAG_INT_CFG, value) )
    return MEMS_ERROR;

  return MEMS_SUCCESS;
}

/*******************************************************************************
* Function Name  : LSM303C_MAG_Z_AxINT_Enable
* Description    : LSM303C_MAG enable interrupt on Z Axis 
* Input          : LSM303C_MAG enable interrupt on Z Axis [LSM303C_MAG_ZIEN_t]
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/

status_t LSM303C_MAG_Z_AxINT_Enable(LSM303C_MAG_ZIEN_t ov){
  u8_t value;

  if( !LSM303C_MAG_ReadReg(LSM303C_MAG_INT_CFG, &value) )
    return MEMS_ERROR;

  value &= ~LSM303C_MAG_ZIEN_ENABLE; //mask
  value |= ov;	
  
  if( !LSM303C_MAG_WriteReg(LSM303C_MAG_INT_CFG, value) )
    return MEMS_ERROR;

  return MEMS_SUCCESS;
}

/*******************************************************************************
* Function Name  : LSM303C_MAG_Y_AxINT_Enable
* Description    : LSM303C_MAG enable interrupt on Y Axis 
* Input          : LSM303C_MAG enable interrupt on Y Axis [LSM303C_MAG_YIEN_t]
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/

status_t LSM303C_MAG_Y_AxINT_Enable(LSM303C_MAG_YIEN_t ov){
  u8_t value;

  if( !LSM303C_MAG_ReadReg(LSM303C_MAG_INT_CFG, &value) )
    return MEMS_ERROR;

  value &= ~LSM303C_MAG_YIEN_ENABLE; //mask
  value |= ov;		

  if( !LSM303C_MAG_WriteReg(LSM303C_MAG_INT_CFG, value) )
    return MEMS_ERROR;

  return MEMS_SUCCESS;
}

/*******************************************************************************
* Function Name  : LSM303C_MAG_X_AxINT_Enable
* Description    : LSM303C_MAG enable interrupt on X Axis 
* Input          : LSM303C_MAG enable interrupt on X Axis [LSM303C_MAG_XIEN_t]
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/

status_t LSM303C_MAG_X_AxINT_Enable(LSM303C_MAG_XIEN_t ov){
  u8_t value;

  if( !LSM303C_MAG_ReadReg(LSM303C_MAG_INT_CFG, &value) )
    return MEMS_ERROR;

  value &= ~LSM303C_MAG_XIEN_ENABLE; //mask
  value |= ov;		

  if( !LSM303C_MAG_WriteReg(LSM303C_MAG_INT_CFG, value) )
    return MEMS_ERROR;

  return MEMS_SUCCESS;
}


/*******************************************************************************
* Function Name  : LSM303C_MAG_InterruptFlag
* Description    : LSM303C_MAG  Interrupt Flag
* Input          : pointer to LSM303C_MAG_INT_t
* Output         : LSM303C_MAG Interrupt Flag [LSM303C_MAG_INT_t]
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/

status_t LSM303C_MAG_InterruptFlag(LSM303C_MAG_INT_t *value){

  if( !LSM303C_MAG_ReadReg(LSM303C_MAG_INT_SRC, (u8_t *)value) )
    return MEMS_ERROR;

  *value &= LSM303C_MAG_INT_FLAG_UP; //mask

  return MEMS_SUCCESS;
}

/*******************************************************************************
* Function Name  : LSM303C_MAG_MeasOverFlow
* Description    : LSM303C_MAG  measure overflow
* Input          : pointer to LSM303C_MAG_MROI_t
* Output         : LSM303C_MAG Interrupt Flag [LSM303C_MAG_MROI_t]
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/

status_t LSM303C_MAG_MeasOverFlow(LSM303C_MAG_MROI_t *value){

  if( !LSM303C_MAG_ReadReg(LSM303C_MAG_INT_SRC, (u8_t *)value) )
    return MEMS_ERROR;

  *value &= LSM303C_MAG_MROI_OVERFLOW; //mask

  return MEMS_SUCCESS;
}


/*******************************************************************************
* Function Name  : LSM303C_MAG_Z_NegativeThresholdFlag
* Description    : LSM303C_MAG  Z_NegativeThreshold
* Input          : pointer to LSM303C_MAG_NTH_Z_t
* Output         : LSM303C_MAG Interrupt Flag [LSM303C_MAG_NTH_Z_t]
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/

status_t LSM303C_MAG_Z_NegativeThresholdFlag(LSM303C_MAG_NTH_Z_t *value){

  if( !LSM303C_MAG_ReadReg(LSM303C_MAG_INT_SRC, (u8_t *)value) )
    return MEMS_ERROR;

  *value &= LSM303C_MAG_NTH_Z_PASS; //mask

  return MEMS_SUCCESS;
}

/*******************************************************************************
* Function Name  : LSM303C_MAG_Y_NegativeThresholdFlag
* Description    : LSM303C_MAG  Y_NegativeThreshold
* Input          : pointer to LSM303C_MAG_NTH_Y_t
* Output         : LSM303C_MAG Interrupt Flag [LSM303C_MAG_NTH_Y_t]
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/

status_t LSM303C_MAG_Y_NegativeThresholdFlag(LSM303C_MAG_NTH_Y_t *value){

  if( !LSM303C_MAG_ReadReg(LSM303C_MAG_INT_SRC, (u8_t *)value) )
    return MEMS_ERROR;

  *value &= LSM303C_MAG_NTH_Y_PASS; //mask

  return MEMS_SUCCESS;
}

/*******************************************************************************
* Function Name  : LSM303C_MAG_X_NegativeThresholdFlag
* Description    : LSM303C_MAG  X_NegativeThreshold
* Input          : pointer to LSM303C_MAG_NTH_X_t
* Output         : LSM303C_MAG Interrupt Flag [LSM303C_MAG_NTH_X_t]
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/

status_t LSM303C_MAG_X_NegativeThresholdFlag(LSM303C_MAG_NTH_X_t *value){

  if( !LSM303C_MAG_ReadReg(LSM303C_MAG_INT_SRC, (u8_t *)value) )
    return MEMS_ERROR;

  *value &= LSM303C_MAG_NTH_X_PASS; //mask

  return MEMS_SUCCESS;
}

/*******************************************************************************
* Function Name  : LSM303C_MAG_Z_PositiveThresholdFlag
* Description    : LSM303C_MAG  Z Positive threshold
* Input          : pointer to LSM303C_MAG_PTH_Z_t
* Output         : LSM303C_MAG Interrupt Flag [LSM303C_MAG_PTH_Z_t]
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/

status_t LSM303C_MAG_Z_PositiveThresholdFlag(LSM303C_MAG_PTH_Z_t *value){

  if( !LSM303C_MAG_ReadReg(LSM303C_MAG_INT_SRC, (u8_t *)value) )
    return MEMS_ERROR;

  *value &= LSM303C_MAG_PTH_Z_PASS; //mask

  return MEMS_SUCCESS;
}
/*******************************************************************************
* Function Name  : LSM303C_MAG_Y_PositiveThresholdFlag
* Description    : LSM303C_MAG  Y Positive threshold
* Input          : pointer to LSM303C_MAG_PTH_Y_t
* Output         : LSM303C_MAG Interrupt Flag [LSM303C_MAG_PTH_Y_t]
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/

status_t LSM303C_MAG_Y_PositiveThresholdFlag(LSM303C_MAG_PTH_Y_t *value){

  if( !LSM303C_MAG_ReadReg(LSM303C_MAG_INT_SRC, (u8_t *)value) )
    return MEMS_ERROR;

  *value &= LSM303C_MAG_PTH_Y_PASS; //mask

  return MEMS_SUCCESS;
}

/*******************************************************************************
* Function Name  : LSM303C_MAG_X_PositiveThresholdFlag
* Description    : LSM303C_MAG  X Positive threshold
* Input          : pointer to LSM303C_MAG_PTH_X_t
* Output         : LSM303C_MAG Interrupt Flag [LSM303C_MAG_PTH_X_t]
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/

status_t LSM303C_MAG_X_PositiveThresholdFlag(LSM303C_MAG_PTH_X_t *value){

  if( !LSM303C_MAG_ReadReg(LSM303C_MAG_INT_SRC, (u8_t *)value) )
    return MEMS_ERROR;

  *value &= LSM303C_MAG_PTH_X_PASS; //mask

  return MEMS_SUCCESS;
}




/*******************************************************************************
* Function Name  : LSM303C_MAG_GetMagRaw
* Description    : Read magnetic output register
* Input          : pointer to AxesRaw_t
* Output         : Magnetic Output Registers buffer AxesRaw_t
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t LSM303C_MAG_GetMagRaw(AxesRaw_t* buff) {
  u8_t valueL;
  u8_t valueH;
	
  if (LSM303C_MAG_flags&LSM303C_MAG_flags_BLE)
  {
 	  if( !LSM303C_MAG_ReadReg(LSM303C_MAG_OUTX_H, &valueL) )
		  return MEMS_ERROR;
	  
	  if( !LSM303C_MAG_ReadReg(LSM303C_MAG_OUTX_L, &valueH) )
		  return MEMS_ERROR;
	  
	  buff->AXIS_X = (i16_t)( (valueH << 8) | valueL );
	  
	  if( !LSM303C_MAG_ReadReg(LSM303C_MAG_OUTY_H, &valueL) )
		  return MEMS_ERROR;
	  
	  if( !LSM303C_MAG_ReadReg(LSM303C_MAG_OUTY_L, &valueH) )
		  return MEMS_ERROR;
	  
	  buff->AXIS_Y = (i16_t)( (valueH << 8) | valueL );
	  
	   if( !LSM303C_MAG_ReadReg(LSM303C_MAG_OUTZ_H, &valueL) )
		  return MEMS_ERROR;
	  
	  if( !LSM303C_MAG_ReadReg(LSM303C_MAG_OUTZ_L, &valueH) )
		  return MEMS_ERROR;
	  
	  buff->AXIS_Z = (i16_t)( (valueH << 8) | valueL ); 
  }
  else
  {
	  if( !LSM303C_MAG_ReadReg(LSM303C_MAG_OUTX_L, &valueL) )
		  return MEMS_ERROR;
	  
	  if( !LSM303C_MAG_ReadReg(LSM303C_MAG_OUTX_H, &valueH) )
		  return MEMS_ERROR;
	  
	  buff->AXIS_X = (i16_t)( (valueH << 8) | valueL );
	  
	  if( !LSM303C_MAG_ReadReg(LSM303C_MAG_OUTY_L, &valueL) )
		  return MEMS_ERROR;
	  
	  if( !LSM303C_MAG_ReadReg(LSM303C_MAG_OUTY_H, &valueH) )
		  return MEMS_ERROR;
	  
	  buff->AXIS_Y = (i16_t)( (valueH << 8) | valueL );
	  
	   if( !LSM303C_MAG_ReadReg(LSM303C_MAG_OUTZ_L, &valueL) )
		  return MEMS_ERROR;
	  
	  if( !LSM303C_MAG_ReadReg(LSM303C_MAG_OUTZ_H, &valueH) )
		  return MEMS_ERROR;
	  
	  buff->AXIS_Z = (i16_t)( (valueH << 8) | valueL );
  }
  return MEMS_SUCCESS;  
}


/*******************************************************************************
* Function Name  : LSM303C_MAG_GetTemperatureRaw
* Description    : Read Temperature output register
* Input          : pointer to u16_t
* Output         : Temperature data row to u16_t
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t LSM303C_MAG_GetTemperatureRaw(u16_t* buff) {
  u8_t valueL;
  u8_t valueH;
	
  if (LSM303C_MAG_flags|LSM303C_MAG_flags_BLE)
  {
 	  if( !LSM303C_MAG_ReadReg(LSM303C_MAG_TEMP_OUT_H, &valueL) )
		  return MEMS_ERROR;
	  
	  if( !LSM303C_MAG_ReadReg(LSM303C_MAG_TEMP_OUT_L, &valueH) )
		  return MEMS_ERROR;
	  
	  *buff = (i16_t)( (valueH << 8) | valueL );
  }
  else
  {
	  if( !LSM303C_MAG_ReadReg(LSM303C_MAG_TEMP_OUT_L, &valueL) )
		  return MEMS_ERROR;
	  
	  if( !LSM303C_MAG_ReadReg(LSM303C_MAG_TEMP_OUT_H, &valueH) )
		  return MEMS_ERROR;
	  
	  *buff = (i16_t)( (valueH << 8) | valueL );
  }
  return MEMS_SUCCESS;  
}


/*******************************************************************************
* Function Name  : LSM303C_MAG_SetThreshold
* Description    : Set Threshold data row
* Input          : threshold row value [u16_t]
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t LSM303C_MAG_SetThreshold(u16_t buff) {
  u8_t valueL;
  u8_t valueH;
  
  valueL=buff&0x00FF;
  valueH=buff&0xFF00>>8;
  
  if( !LSM303C_MAG_WriteReg(LSM303C_MAG_INT_THS_L, valueL) )
    return MEMS_ERROR;
  
  if( !LSM303C_MAG_WriteReg(LSM303C_MAG_INT_THS_H, valueH) )
    return MEMS_ERROR;
	  
  return MEMS_SUCCESS;  
}

























