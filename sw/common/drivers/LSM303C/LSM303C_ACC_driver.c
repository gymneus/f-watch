/******************** (C) COPYRIGHT 2013 STMicroelectronics ********************
* File Name          : LSM303C_ACC.c
* Author             : MSH Application Team
* Author             : $Author:$
* Version            : rev 1.0
* Date               : $Date:$
* Description        : LSM303C_ACC driver file
*                      
* HISTORY:
* Date               |	Modification                    |	Author
* 09/2012        	 |	Initial Revision                |	Alberto Zancanato
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
#include "LSM303C_ACC_driver.h"
#include "lsm303c.h"

//EXAMPLE to fill LSM303C_ACC_ReadReg and LSM303C_ACC_WriteReg
//#include "i2C_mems.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/

/*******************************************************************************
* Function Name		: LSM303C_ACC_ReadReg
* Description		: Generic Reading function. It must be fullfilled with either
*					: I2C or SPI reading functions					
* Input				: Register Address
* Output			: Data REad
* Return			: None
*******************************************************************************/
u8_t LSM303C_ACC_ReadReg(u8_t Reg, u8_t* Data)
{
  *Data = spi_read(DEV_ACC, Reg);
  return MEMS_SUCCESS;
}

/*******************************************************************************
* Function Name		: LSM303C_ACC_WriteReg
* Description		: Generic Writing function. It must be fullfilled with either
*					: I2C or SPI writing function
* Input				: Register Address, Data to be written
* Output			: None
* Return			: None
*******************************************************************************/
u8_t LSM303C_ACC_WriteReg(u8_t Reg, u8_t Data)
{
  spi_send(DEV_ACC, Reg, Data);
  return 1;
}
/* Private functions ---------------------------------------------------------*/


/*******************************************************************************
* Function Name  : LSM303C_ACC_SelfTest
* Description    : Enable/Disable LSM303C_ACC Self Test
* Input          : Self test [LSM303C_ACC_ST_t]
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t LSM303C_ACC_SelfTest(LSM303C_ACC_ST_t ov){
  u8_t value;

  if( !LSM303C_ACC_ReadReg(LSM303C_ACC_CTRL5, &value) )
    return MEMS_ERROR;

  value &= ~LSM303C_ACC_ST_NA; //mask
  value |= ov;	

  if( !LSM303C_ACC_WriteReg(LSM303C_ACC_CTRL5, value) )
    return MEMS_ERROR;

  return MEMS_SUCCESS;
}

/*******************************************************************************
* Function Name  : LSM303C_ACC_SetODR
* Description    : Sets LSM303C_ACC Output Data Rate
* Input          : Set Output data rate [LSM303C_ACC_ODR_t]
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t LSM303C_ACC_SetODR(LSM303C_ACC_ODR_t ov){
  u8_t value;

  if( !LSM303C_ACC_ReadReg(LSM303C_ACC_CTRL1, &value) )
    return MEMS_ERROR;

  value &= ~LSM303C_ACC_ODR_MASK; //mask
  value |= ov;	
	
  if( !LSM303C_ACC_WriteReg(LSM303C_ACC_CTRL1, value) )
    return MEMS_ERROR;

  return MEMS_SUCCESS;
}

/*******************************************************************************
* Function Name  : LSM303C_ACC_AxOperativeMode
* Description    : Sets LSM303C_ACC Output Data Rate
* Input          : Set Output data rate [LSM303C_ACC_HR_t]
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
//#WARNING: If you enable the HR bit Low pass cut off frequency will change
status_t LSM303C_ACC_AxOperativeMode(LSM303C_ACC_HR_t ov){
  u8_t value;

  if( !LSM303C_ACC_ReadReg(LSM303C_ACC_CTRL1, &value) )
    return MEMS_ERROR;

  value &= ~LSM303C_ACC_HIGH_RES_ON; //mask
  value |= ov;

  if( !LSM303C_ACC_WriteReg(LSM303C_ACC_CTRL1, value) )
    return MEMS_ERROR;

  return MEMS_SUCCESS;
}

/*******************************************************************************
* Function Name  : LSM303C_ACC_SoftReset
* Description    : Enable/Disable LSM303C_ACC SoftReset
* Input          : SoftReset Enable/Disable [LSM303C_ACC_SOFT_RESET_t]
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t LSM303C_ACC_SoftReset(LSM303C_ACC_SOFT_RESET_t ov){
  u8_t value;

  if( !LSM303C_ACC_ReadReg(LSM303C_ACC_CTRL5, &value) )
    return MEMS_ERROR;

  value &= ~LSM303C_ACC_SOFT_RESET_ENABLE; //mask
  value |= ov;	

  if( !LSM303C_ACC_WriteReg(LSM303C_ACC_CTRL5, value) )
    return MEMS_ERROR;

  return MEMS_SUCCESS;
}

/*******************************************************************************
* Function Name  : LSM303C_ACC_SetFullScale
* Description    : set LSM303C_ACC Full scale
* Input          : set LSM303C_ACC Full scale [LSM303C_ACC_FS_t]
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t LSM303C_ACC_SetFullScale(LSM303C_ACC_FS_t ov){
  u8_t value;

  if( !LSM303C_ACC_ReadReg(LSM303C_ACC_CTRL4, &value) )
    return MEMS_ERROR;

  value &= ~LSM303C_ACC_FS_8g; //mask
  value |= ov;	


  if( !LSM303C_ACC_WriteReg(LSM303C_ACC_CTRL4, value) )
    return MEMS_ERROR;

  return MEMS_SUCCESS;
}

/*******************************************************************************
* Function Name  : LSM303C_ACC_SerialInterfaceMode
* Description    : set LSM303C_ACC SerialInterfaceMode
* Input          : set LSM303C_ACC SerialInterfaceMode [LSM303C_ACC_SIM_t]
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t LSM303C_ACC_SerialInterfaceMode(LSM303C_ACC_SIM_t ov){
  u8_t value;

  if( !LSM303C_ACC_ReadReg(LSM303C_ACC_CTRL4, &value) )
    return MEMS_ERROR;

  value &= ~LSM303C_ACC_SIM_3WIRE_INTERFACE; //mask
  value |= ov;		


  if( !LSM303C_ACC_WriteReg(LSM303C_ACC_CTRL4, value) )
    return MEMS_ERROR;

  return MEMS_SUCCESS;
}

/*******************************************************************************
* Function Name  : LSM303C_ACC_BlockDataUpdate
* Description    : Enable/Disable LSM303C_ACC BlockDataUpdate
* Input          : Enable/Disable LSM303C_ACC BlockDataUpdate[LSM303C_ACC_BDU_t]
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/

status_t LSM303C_ACC_BlockDataUpdate(LSM303C_ACC_BDU_t ov){
  u8_t value;

  if( !LSM303C_ACC_ReadReg(LSM303C_ACC_CTRL1, &value) )
    return MEMS_ERROR;

  value &= ~LSM303C_ACC_BDU_ENABLE; //mask
  value |= ov;	

  if( !LSM303C_ACC_WriteReg(LSM303C_ACC_CTRL1, value) )
    return MEMS_ERROR;

  return MEMS_SUCCESS;
}



/*******************************************************************************
* Function Name  : LSM303C_ACC_EnableInterruptGeneratorOne
* Description    : Enable/Disable LSM303C_ACC interrupt generator one
* Input          : Enable/Disable LSM303C_ACC interrupt generator one[LSM303C_ACC_IG_CONFIG_t]
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
//You can use LSM303C_ACC_IG_CONFIG_t with or condition see DS 4D and 6D interrupt
status_t LSM303C_ACC_EnableInterruptGeneratorOne(LSM303C_ACC_IG_CONFIG_t ov){
  u8_t valueCTRL7, valueCFG1;
  
 	if( !LSM303C_ACC_ReadReg(LSM303C_ACC_CTRL7, &valueCTRL7) )
		return MEMS_ERROR; 
	if( !LSM303C_ACC_ReadReg(LSM303C_ACC_IG_CFG1, &valueCFG1) )
		return MEMS_ERROR;	
 
	if (ov&LSM303C_ACC_IG_4D)
	{
	  valueCFG1 &= 0x80; //disable all interrupt generation

	  if( !LSM303C_ACC_WriteReg(LSM303C_ACC_IG_CFG1, valueCFG1) )
		return MEMS_ERROR; 

	  valueCTRL7 |= LSM303C_ACC_4D_INTGEN1_EN; //enable 4D recognition

	  if( !LSM303C_ACC_WriteReg(LSM303C_ACC_CTRL7, valueCTRL7) )
		return MEMS_ERROR; 
	}
	else
	{

	  valueCFG1 &= ~0x7F; //enable selected interrupt
	  valueCFG1 |= ov;			

	  if( !LSM303C_ACC_WriteReg(LSM303C_ACC_IG_CFG1, valueCFG1) )
		return MEMS_ERROR; 

	  valueCTRL7 &= ~LSM303C_ACC_4D_INTGEN1_EN; //disable 4d recognition

	  if( !LSM303C_ACC_WriteReg(LSM303C_ACC_CTRL7, valueCTRL7) )
		return MEMS_ERROR; 
	}
        return MEMS_SUCCESS;
}

/*******************************************************************************
* Function Name  : LSM303C_ACC_EnableInterruptGeneratorTwo
* Description    : Enable/Disable LSM303C_ACC interrupt generator two
* Input          : Enable/Disable LSM303C_ACC interrupt generator two[LSM303C_ACC_IG_CONFIG_t]
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
//You can use LSM303C_ACC_IG_CONFIG_t with or condition see DS 4D and 6D interrupt
status_t LSM303C_ACC_EnableInterruptGeneratorTwo(LSM303C_ACC_IG_CONFIG_t ov){
  u8_t valueCTRL7, valueCFG2;
  
 	if( !LSM303C_ACC_ReadReg(LSM303C_ACC_CTRL7, &valueCTRL7) )
		return MEMS_ERROR; 
	if( !LSM303C_ACC_ReadReg(LSM303C_ACC_IG_CFG2, &valueCFG2) )
		return MEMS_ERROR;	
 
	if (ov&LSM303C_ACC_IG_4D)
	{
	  valueCFG2 &= 0x80; //disable all interrupt generation

	  if( !LSM303C_ACC_WriteReg(LSM303C_ACC_IG_CFG2, valueCFG2) )
		return MEMS_ERROR; 

	  valueCTRL7 |= LSM303C_ACC_4D_INTGEN2_EN; //enable 4D recognition

	  if( !LSM303C_ACC_WriteReg(LSM303C_ACC_CTRL7, valueCTRL7) )
		return MEMS_ERROR; 
	}
	else
	{
	
	  valueCFG2 &= ~0x7F; //enable selected interrupt
	  valueCFG2 |= ov;	

	  if( !LSM303C_ACC_WriteReg(LSM303C_ACC_IG_CFG2, valueCFG2) )
		return MEMS_ERROR; 

	  valueCTRL7 &= ~LSM303C_ACC_4D_INTGEN2_EN; //disable 4d recognition

	  if( !LSM303C_ACC_WriteReg(LSM303C_ACC_CTRL7, valueCTRL7) )
		return MEMS_ERROR; 
	}
        return MEMS_SUCCESS;
}

/*******************************************************************************
* Function Name  : LSM303C_ACC_InterruptGeneratorOne_LogicCondition
* Description    : LSM303C_ACC interrupt generator one LogicCondition
* Input          : LSM303C_ACC interrupt generator one LogicCondition[LSM303C_ACC_AOI_IG_t]
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/

status_t LSM303C_ACC_InterruptGeneratorOne_LogicCondition(LSM303C_ACC_AOI_IG_t ov){
  u8_t value;

  if( !LSM303C_ACC_ReadReg(LSM303C_ACC_IG_CFG1, &value) )
    return MEMS_ERROR;

	value &= ~LSM303C_ACC_AOI_IG_AND; //mask
	value |= ov;	

  if( !LSM303C_ACC_WriteReg(LSM303C_ACC_IG_CFG1, value) )
    return MEMS_ERROR;

  return MEMS_SUCCESS;
}

/*******************************************************************************
* Function Name  : LSM303C_ACC_InterruptGeneratorTwo_LogicCondition
* Description    : LSM303C_ACC interrupt generator two LogicCondition
* Input          : LSM303C_ACC interrupt generator two LogicCondition[LSM303C_ACC_AOI_IG_t]
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/

status_t LSM303C_ACC_InterruptGeneratorTwo_LogicCondition(LSM303C_ACC_AOI_IG_t ov){
  u8_t value;

  if( !LSM303C_ACC_ReadReg(LSM303C_ACC_IG_CFG2, &value) )
    return MEMS_ERROR;

	value &= ~LSM303C_ACC_AOI_IG_AND; //mask
	value |= ov;	
	
  if( !LSM303C_ACC_WriteReg(LSM303C_ACC_IG_CFG2, value) )
    return MEMS_ERROR;

  return MEMS_SUCCESS;
}

/*******************************************************************************
* Function Name  : LSM303C_ACC_InterruptGeneratorOne_Flag
* Description    : read interrupt one generator flags
* Input          : pointer to LSM303C_ACC_IG_FLAGS_t
* Output         : LSM303C_ACC XYZ Axis data overrun [LSM303C_ACC_IG_FLAGS_t]
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
//You can use LSM303C_ACC_IG_FLAGS_t with or condition to interpret value
status_t LSM303C_ACC_InterruptGeneratorOne_Flag(LSM303C_ACC_IG_FLAGS_t *value){

  if( !LSM303C_ACC_ReadReg(LSM303C_ACC_IG_SRC1, (u8_t *)value) )
    return MEMS_ERROR;

  *value &= 0x7F; //mask

  return MEMS_SUCCESS;
}

/*******************************************************************************
* Function Name  : LSM303C_ACC_InterruptGeneratorTwo_Flag
* Description    : read interrupt two generator flags
* Input          : pointer to LSM303C_ACC_IG_FLAGS_t
* Output         : LSM303C_ACC XYZ Axis data overrun [LSM303C_ACC_IG_FLAGS_t]
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
//You can use LSM303C_ACC_IG_FLAGS_t with or condition to interpret value
status_t LSM303C_ACC_InterruptGeneratorTwo_Flag(LSM303C_ACC_IG_FLAGS_t *value){

  if( !LSM303C_ACC_ReadReg(LSM303C_ACC_IG_SRC2, (u8_t *)value) )
    return MEMS_ERROR;

  *value &= 0x7F; //mask

  return MEMS_SUCCESS;
}

/*******************************************************************************
* Function Name  : LSM303C_ACC_InterruptGeneratorOne_Wait
* Description    : Enable/Disable LSM303C_ACC interrupt generator one Wait
* Input          : Enable/Disable LSM303C_ACC interrupt generator one Wait[LSM303C_ACC_WAIT_IG_t]
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/

status_t LSM303C_ACC_InterruptGeneratorOne_Wait(LSM303C_ACC_WAIT_IG_t ov){
  u8_t value;

  if( !LSM303C_ACC_ReadReg(LSM303C_ACC_IG_DUR1, &value) )
    return MEMS_ERROR;

  value &= ~LSM303C_ACC_WAIT_IG_ON; //mask
  value |= ov;	

  if( !LSM303C_ACC_WriteReg(LSM303C_ACC_IG_DUR1, value) )
    return MEMS_ERROR;

  return MEMS_SUCCESS;
}

/*******************************************************************************
* Function Name  : LSM303C_ACC_InterruptGeneratorTwo_Wait
* Description    : Enable/Disable LSM303C_ACC interrupt generator two Wait
* Input          : Enable/Disable LSM303C_ACC interrupt generator two Wait[LSM303C_ACC_WAIT_IG_t]
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/

status_t LSM303C_ACC_InterruptGeneratorTwo_Wait(LSM303C_ACC_WAIT_IG_t ov){
  u8_t value;

  if( !LSM303C_ACC_ReadReg(LSM303C_ACC_IG_DUR2, &value) )
    return MEMS_ERROR;

  value &= ~LSM303C_ACC_WAIT_IG_ON; //mask
  value |= ov;	
  
  if( !LSM303C_ACC_WriteReg(LSM303C_ACC_IG_DUR2, value) )
    return MEMS_ERROR;

  return MEMS_SUCCESS;
}

/*******************************************************************************
* Function Name  : LSM303C_ACC_EnableInterruptPinOne
* Description    : Enable/Disable LSM303C_ACC interrupt pin one
* Input          : Enable/Disable LSM303C_ACC interrupt pin one[LSM303C_ACC_INT1_DRDY_t]
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t LSM303C_ACC_EnableInterruptPinOne(LSM303C_ACC_INT1_DRDY_t ov){
  u8_t value;

  if( !LSM303C_ACC_ReadReg(LSM303C_ACC_CTRL3, &value) )
    return MEMS_ERROR;

  value &= ~0x7F; //mask
  value |= ov;		

  if( !LSM303C_ACC_WriteReg(LSM303C_ACC_CTRL3, value) )
    return MEMS_ERROR;

  return MEMS_SUCCESS;
}

/*******************************************************************************
* Function Name  : LSM303C_ACC_EnableInterruptPinTwo
* Description    : Enable/Disable LSM303C_ACC interrupt pin two
* Input          : Enable/Disable LSM303C_ACC interrupt pin two[LSM303C_ACC_INT2_DRDY_t]
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t LSM303C_ACC_EnableInterruptPinTwo(LSM303C_ACC_INT2_DRDY_t ov){
  u8_t value;

  if( !LSM303C_ACC_ReadReg(LSM303C_ACC_CTRL6, &value) )
    return MEMS_ERROR;

  value &= ~0x3F; //mask
  value |= ov;

  if( !LSM303C_ACC_WriteReg(LSM303C_ACC_CTRL6, value) )
    return MEMS_ERROR;

  return MEMS_SUCCESS;
}

/*******************************************************************************
* Function Name  : LSM303C_ACC_Reboot
* Description    : LSM303C_ACC_Reboot 
* Input          : None
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t LSM303C_ACC_Reboot(void){
  u8_t value;

  if( !LSM303C_ACC_ReadReg(LSM303C_ACC_CTRL6, &value) )
    return MEMS_ERROR;

  value |= LSM303C_ACC_REBOOT; 

  if( !LSM303C_ACC_WriteReg(LSM303C_ACC_CTRL6, value) )
    return MEMS_ERROR;

  return MEMS_SUCCESS;
}

/*******************************************************************************
* Function Name  : LSM303C_ACC_EnableAxis
* Description    : LSM303C_ACC_EnableAxis
* Input          : Enable/Disable LSM303C_ACC axis [LSM303C_ACC_AXIS_EN_t]
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
//You can use LSM303C_ACC_AXIS_EN_t with or condition
status_t LSM303C_ACC_EnableAxis( u8_t ov){
  u8_t value;

  if( !LSM303C_ACC_ReadReg(LSM303C_ACC_CTRL1, &value) )
    return MEMS_ERROR;

  value &= ~0x07; //mask
  value |= ov;	

  if( !LSM303C_ACC_WriteReg(LSM303C_ACC_CTRL1, value) )
    return MEMS_ERROR;

  return MEMS_SUCCESS;
}

/*******************************************************************************
* Function Name  : LSM303C_ACC_HighPassFilterMode
* Description    : LSM303C_ACC_HighPassFilterMode
* Input          : Select LSM303C_ACC high pass filter mode [LSM303C_ACC_HPM_t]
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
//Referring DS to HP filter Usage
status_t LSM303C_ACC_HighPassFilterMode(LSM303C_ACC_HPM_t ov){
  u8_t value;

  if( !LSM303C_ACC_ReadReg(LSM303C_ACC_CTRL2, &value) )
    return MEMS_ERROR;

  value &= ~0x18; //mask
  value |= ov;		

  if( !LSM303C_ACC_WriteReg(LSM303C_ACC_CTRL2, value) )
    return MEMS_ERROR;

  return MEMS_SUCCESS;
}

/*******************************************************************************
* Function Name  : LSM303C_ACC_I2C_Mode
* Description    : Enable/Disable LSM303C_ACC I2C
* Input          : Enable/Disable LSM303C_ACC I2C[LSM303C_ACC_I2C_t]
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/

status_t LSM303C_ACC_I2C_Mode(LSM303C_ACC_I2C_t ov){
  u8_t value;

  if( !LSM303C_ACC_ReadReg(LSM303C_ACC_CTRL4, &value) )
    return MEMS_ERROR;

  value &= ~LSM303C_ACC_I2C_DISABLE; //mask
  value |= ov;		

  if( !LSM303C_ACC_WriteReg(LSM303C_ACC_CTRL4, value) )
    return MEMS_ERROR;

  return MEMS_SUCCESS;
}

/*******************************************************************************
* Function Name  : LSM303C_ACC_AuotoInc
* Description    : Enable/Disable LSM303C_ACC AuotoInc
* Input          : Enable/Disable LSM303C_ACC AuotoInc[LSM303C_ACC_IF_ADD_INC_t]
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/

status_t LSM303C_ACC_AuotoInc(LSM303C_ACC_IF_ADD_INC_t ov){
  u8_t value;

  if( !LSM303C_ACC_ReadReg(LSM303C_ACC_CTRL4, &value) )
    return MEMS_ERROR;

  value &= ~LSM303C_ACC_IF_ADD_INC_ENABLE; //mask
  value |= ov;	

  if( !LSM303C_ACC_WriteReg(LSM303C_ACC_CTRL4, value) )
    return MEMS_ERROR;

  return MEMS_SUCCESS;
}

/*******************************************************************************
* Function Name  : LSM303C_ACC_Select_Bandwidth
* Description    : LSM303C_ACC LSM303C_ACC_Select_Bandwidth
* Input          : LSM303C_ACC LSM303C_ACC_Select_Bandwidth[LSM303C_ACC_BW_t]
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/

status_t LSM303C_ACC_Select_Bandwidth(LSM303C_ACC_BW_t ov){
  u8_t value;

  if( !LSM303C_ACC_ReadReg(LSM303C_ACC_CTRL4, &value) )
    return MEMS_ERROR;

  value &= ~LSM303C_ACC_BW_50_Hz; //mask
  value |= ov;	

  if( !LSM303C_ACC_WriteReg(LSM303C_ACC_CTRL4, value) )
    return MEMS_ERROR;

  return MEMS_SUCCESS;
}

/*******************************************************************************
* Function Name  : LSM303C_ACC_IntPin_Mode
* Description    : LSM303C_ACC Interrupt pin mode
* Input          : LSM303C_ACC Interrupt pin mode[LSM303C_ACC_INT_PIN_CFG_t]
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/

status_t LSM303C_ACC_IntPin_Mode(LSM303C_ACC_INT_PIN_CFG_t ov){
  u8_t value;

  if( !LSM303C_ACC_ReadReg(LSM303C_ACC_CTRL5, &value) )
    return MEMS_ERROR;

  value &= ~LSM303C_ACC_OPEN_DRAIN_ACTIVE_LOW; //mask
  value |= ov;	

  if( !LSM303C_ACC_WriteReg(LSM303C_ACC_CTRL5, value) )
    return MEMS_ERROR;

  return MEMS_SUCCESS;
}

/*******************************************************************************
* Function Name  : LSM303C_ACC_DebugMode
* Description    : Enable/Disable LSM303C_ACC Debug Mode
* Input          : Enable/Disable LSM303C_ACC Debug Mode[LSM303C_ACC_DEBUG_t]
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/

status_t LSM303C_ACC_DebugMode(LSM303C_ACC_DEBUG_t ov){
  u8_t value;

  if( !LSM303C_ACC_ReadReg(LSM303C_ACC_CTRL5, &value) )
    return MEMS_ERROR;

  value &= ~LSM303C_ACC_DEBUG_ENABLE; //mask
  value |= ov;	

  if( !LSM303C_ACC_WriteReg(LSM303C_ACC_CTRL5, value) )
    return MEMS_ERROR;

  return MEMS_SUCCESS;
}

/*******************************************************************************
* Function Name  : LSM303C_ACC_InterruptSignalsMode
* Description    : LSM303C_ACC Interrupt Signals Mode
* Input          : LSM303C_ACC Interrupt Signals Mode[LSM303C_ACC_LAT_SIG_t]
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/

status_t LSM303C_ACC_InterruptSignalsMode(LSM303C_ACC_LAT_SIG_t ov){
  u8_t value;

  if( !LSM303C_ACC_ReadReg(LSM303C_ACC_CTRL7, &value) )
    return MEMS_ERROR;

  value &= ~LSM303C_ACC_INT1_LAT_INT2_LAT; //mask
  value |= ov;	

  if( !LSM303C_ACC_WriteReg(LSM303C_ACC_CTRL7, value) )
    return MEMS_ERROR;

  return MEMS_SUCCESS;
}

/*******************************************************************************
* Function Name  : LSM303C_ACC_ResetInterruptDuration
* Description    : LSM303C_ACC Reset Interrupt Duration
* Input          : LSM303C_ACC Reset Interrupt Duration[LSM303C_ACC_RESET_DUR_t]
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/

status_t LSM303C_ACC_ResetInterruptDuration(LSM303C_ACC_RESET_DUR_t ov){
  u8_t value;

  if( !LSM303C_ACC_ReadReg(LSM303C_ACC_CTRL7, &value) )
    return MEMS_ERROR;

  value &= ~LSM303C_ACC_DUR1_RST_DUR2_RST; //mask
  value |= ov;	

  if( !LSM303C_ACC_WriteReg(LSM303C_ACC_CTRL7, value) )
    return MEMS_ERROR;

  return MEMS_SUCCESS;
}

/*******************************************************************************
* Function Name  : LSM303C_ACC_Status_Flags
* Description    : read LSM303C_ACC Status Flags
* Input          : pointer to LSM303C_ACC_STATUS_FLAGS_t
* Output         : read LSM303C_ACC Status Flags [LSM303C_ACC_STATUS_FLAGS_t]
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
//You can use LSM303C_ACC_STATUS_FLAGS_t with or condition to interpret value
status_t LSM303C_ACC_Status_Flags(u8_t *value){

  if( !LSM303C_ACC_ReadReg(LSM303C_ACC_STATUS, (u8_t *)value) )
    return MEMS_ERROR;
  return MEMS_SUCCESS;
}

/*******************************************************************************
* Function Name  : LSM303C_ACC_FIFO_Mode
* Description    : LSM303C_ACC FIFO Mode
* Input          : LSM303C_ACC FIFO Mode[LSM303C_ACC_FMODE_t]
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
//this function enable FIFO to disable use LSM303C_ACC_FIFO
status_t LSM303C_ACC_FIFO_Mode(LSM303C_ACC_FMODE_t ov){
  u8_t value;

  if( !LSM303C_ACC_ReadReg(LSM303C_ACC_CTRL3, &value) )
  return MEMS_ERROR;

  value |= LSM303C_ACC_FIFO_ENABLE; //Enable FIFO

  if( !LSM303C_ACC_WriteReg(LSM303C_ACC_CTRL3, value) )
    return MEMS_ERROR;
  
  if( !LSM303C_ACC_ReadReg(LSM303C_ACC_FIFO_CTRL, &value) )
    return MEMS_ERROR;

  value &= ~LSM303C_ACC_FMODE_BYPASS_TO_FIFO; //mask
  value |= ov;		

  if( !LSM303C_ACC_WriteReg(LSM303C_ACC_FIFO_CTRL, value) )
    return MEMS_ERROR;

  return MEMS_SUCCESS;
}

/*******************************************************************************
* Function Name  : LSM303C_ACC_FIFO_Flags
* Description    : read LSM303C_ACC FIFO Flags
* Input          : pointer to LSM303C_ACC_FIFO_FLAGS_t
* Output         : read LSM303C_ACC FIFO Flags [LSM303C_ACC_FIFO_FLAGS_t]
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
//You can use LSM303C_ACC_FIFO_FLAGS_t with or condition to interpret value
status_t LSM303C_ACC_FIFO_Flags(LSM303C_ACC_FIFO_FLAGS_t *value){

  if( !LSM303C_ACC_ReadReg(LSM303C_ACC_FIFO_SRC, (u8_t *)value) )
    return MEMS_ERROR;

  *value &= 0xE0; //mask

  return MEMS_SUCCESS;
}

/*******************************************************************************
* Function Name  : LSM303C_ACC_FIFO_StopAtTh
* Description    : LSM303C_ACC FIFO Stop at threshold
* Input          : LSM303C_ACC FIFO Stop at threshold[LSM303C_ACC_STOP_FTH_t]
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/

status_t LSM303C_ACC_FIFO_StopAtTh(LSM303C_ACC_STOP_FTH_t ov){
  u8_t value;

  if( !LSM303C_ACC_ReadReg(LSM303C_ACC_CTRL3, &value) )
    return MEMS_ERROR;

  value &= ~LSM303C_ACC_STOP_FTH_EN_FIFO_TH; //mask
  value |= ov;	

  if( !LSM303C_ACC_WriteReg(LSM303C_ACC_CTRL3, value) )
    return MEMS_ERROR;

  return MEMS_SUCCESS;
}

/*******************************************************************************
* Function Name  : LSM303C_ACC_FIFO
* Description    : Enable/Disable LSM303C_ACC FIFO
* Input          : Enable/Disable LSM303C_ACC FIFO[LSM303C_ACC_FIFO_EN_t]
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/

status_t LSM303C_ACC_FIFO(LSM303C_ACC_FIFO_EN_t ov){
  u8_t value;

  if( !LSM303C_ACC_ReadReg(LSM303C_ACC_CTRL3, &value) )
    return MEMS_ERROR;

  value &= ~LSM303C_ACC_FIFO_ENABLE; //mask
  value |= ov;		

  if( !LSM303C_ACC_WriteReg(LSM303C_ACC_CTRL3, value) )
    return MEMS_ERROR;

  return MEMS_SUCCESS;
}

/*******************************************************************************
* Function Name  : LSM303C_ACC_GetAccRaw
* Description    : Read accelerometer output register
* Input          : pointer to AxesRaw_t
* Output         : Acceleration Output Registers buffer AxesRaw_t
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t LSM303C_ACC_GetAccRaw(AxesRaw_t* buff) {
  u8_t valueL;
  u8_t valueH;

  if( !LSM303C_ACC_ReadReg(LSM303C_ACC_OUT_X_H, &valueH) )
	  return MEMS_ERROR;
  
  if( !LSM303C_ACC_ReadReg(LSM303C_ACC_OUT_X_L, &valueL) )
	  return MEMS_ERROR;
  
  buff->AXIS_X = (i16_t)( (valueH << 8) | valueL );
  
  if( !LSM303C_ACC_ReadReg(LSM303C_ACC_OUT_Y_H, &valueH) )
	  return MEMS_ERROR;
  
  if( !LSM303C_ACC_ReadReg(LSM303C_ACC_OUT_Y_L, &valueL) )
	  return MEMS_ERROR;
  
  buff->AXIS_Y = (i16_t)( (valueH << 8) | valueL );
  
   if( !LSM303C_ACC_ReadReg(LSM303C_ACC_OUT_Z_H, &valueH) )
	  return MEMS_ERROR;
  
  if( !LSM303C_ACC_ReadReg(LSM303C_ACC_OUT_Z_L, &valueL) )
	  return MEMS_ERROR;
  
  buff->AXIS_Z = (i16_t)( (valueH << 8) | valueL ); 

  return MEMS_SUCCESS;  
}


/*******************************************************************************
* Function Name  : LSM303C_ACC_GetTemperatureRaw
* Description    : Read Temperature output register
* Input          : pointer to u16_t
* Output         : Temperature data row to u16_t
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t LSM303C_ACC_GetTemperatureRaw(u16_t* buff) {
  u8_t valueL;
  u8_t valueH;
	
  if( !LSM303C_ACC_ReadReg(LSM303C_ACC_TEMP_H, &valueL) )
	  return MEMS_ERROR;
  
  if( !LSM303C_ACC_ReadReg(LSM303C_ACC_TEMP_L, &valueH) )
	  return MEMS_ERROR;
  
  *buff = (i16_t)( (valueH << 8) | valueL );

  return MEMS_SUCCESS;  
}

/*******************************************************************************
* Function Name  : LSM303C_ACC_SetInterrupt1_Threshold_X
* Description    : Set Interrupt Threshold data row
* Input          : threshold row value [u8_t]
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t LSM303C_ACC_SetInterrupt1_Threshold_X(u8_t buff) {
  
  if( !LSM303C_ACC_WriteReg(LSM303C_ACC_IG_THS_X1, buff) )
      return MEMS_ERROR;
  return MEMS_SUCCESS;  
}

/*******************************************************************************
* Function Name  : LSM303C_ACC_SetInterrupt1_Threshold_Y
* Description    : Set Interrupt Threshold data row
* Input          : threshold row value [u8_t]
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t LSM303C_ACC_SetInterrupt1_Threshold_Y(u8_t buff) {
  
  if( !LSM303C_ACC_WriteReg(LSM303C_ACC_IG_THS_Y1, buff) )
      return MEMS_ERROR;	  
  return MEMS_SUCCESS;  
}

/*******************************************************************************
* Function Name  : LSM303C_ACC_SetInterrupt1_Threshold_Z
* Description    : Set Interrupt Threshold data row
* Input          : threshold row value [u8_t]
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t LSM303C_ACC_SetInterrupt1_Threshold_Z(u8_t buff) {
  
  if( !LSM303C_ACC_WriteReg(LSM303C_ACC_IG_THS_Z1, buff) )
      return MEMS_ERROR;	  
  return MEMS_SUCCESS;  
}

/*******************************************************************************
* Function Name  : LSM303C_ACC_SetInterrupt2_Threshold_ZYX
* Description    : Set Interrupt Threshold data row
* Input          : threshold row value [u8_t]
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t LSM303C_ACC_SetInterrupt2_Threshold_ZYX(u8_t buff) {
  
  if( !LSM303C_ACC_WriteReg(LSM303C_ACC_IG_THS2, buff) )
      return MEMS_ERROR;	  
  return MEMS_SUCCESS;  
}

/*******************************************************************************
* Function Name  : LSM303C_ACC_SetInterrupt1_Duration
* Description    : Set Interrupt Duration data row
* Input          : Duration row value [u8_t]
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
//WARNING: value from 0x00->0x7F
status_t LSM303C_ACC_SetInterrupt1_Duration(u8_t buff) {

  u8_t value;
  
  buff |= 0x7F; //coerce

  if( !LSM303C_ACC_ReadReg(LSM303C_ACC_IG_DUR1, &value) )
    return MEMS_ERROR;

  value &= ~0x7F; //mask
  value |= buff;

  if( !LSM303C_ACC_WriteReg(LSM303C_ACC_IG_DUR1, value) )
    return MEMS_ERROR;

  return MEMS_SUCCESS;
}

/*******************************************************************************
* Function Name  : LSM303C_ACC_SetInterrupt2_Duration
* Description    : Set Interrupt Duration data row
* Input          : Duration row value [u8_t]
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
//WARNING: value from 0x00->0x7F
status_t LSM303C_ACC_SetInterrupt2_Duration(u8_t buff) {
  
  u8_t value;
  
  buff |= 0x7F; //coerce

  if( !LSM303C_ACC_ReadReg(LSM303C_ACC_IG_DUR2, &value) )
    return MEMS_ERROR;

  value &= ~0x7F; //mask
  value |= buff;

  if( !LSM303C_ACC_WriteReg(LSM303C_ACC_IG_DUR2, value) )
    return MEMS_ERROR;

  return MEMS_SUCCESS;
}
/*******************************************************************************
* Function Name  : LSM303C_ACC_SetReferenceValue
* Description    : Set Reference data row
* Input          : Reference row value [AxesRaw_t]
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t LSM303C_ACC_SetReferenceValue(AxesRaw_t buff) {
  u8_t valueL;
  u8_t valueH;
  
  valueL=buff.AXIS_X&0x00FF;
  valueH=buff.AXIS_X&0xFF00>>8;
  
  if( !LSM303C_ACC_WriteReg(LSM303C_ACC_XL_REFERENCE, valueL) )
    return MEMS_ERROR;
  
  if( !LSM303C_ACC_WriteReg(LSM303C_ACC_XH_REFERENCE, valueH) )
    return MEMS_ERROR;
	
  valueL=buff.AXIS_Y&0x00FF;
  valueH=buff.AXIS_Y&0xFF00>>8;
  
  if( !LSM303C_ACC_WriteReg(LSM303C_ACC_YL_REFERENCE, valueL) )
    return MEMS_ERROR;
  
  if( !LSM303C_ACC_WriteReg(LSM303C_ACC_YH_REFERENCE, valueH) )
    return MEMS_ERROR;

  valueL=buff.AXIS_Z&0x00FF;
  valueH=buff.AXIS_Z&0xFF00>>8;
  
  if( !LSM303C_ACC_WriteReg(LSM303C_ACC_ZL_REFERENCE, valueL) )
    return MEMS_ERROR;
  
  if( !LSM303C_ACC_WriteReg(LSM303C_ACC_ZH_REFERENCE, valueH) )
    return MEMS_ERROR;	
	  
  return MEMS_SUCCESS;  
}

/*******************************************************************************
* Function Name  : LSM303C_ACC_GetReferenceValue
* Description    : Get Reference data row
* Input          : Reference row value [AxesRaw_t]
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t LSM303C_ACC_GetReferenceValue(AxesRaw_t *buff) {
  u8_t valueL;
  u8_t valueH;
  
  if( !LSM303C_ACC_ReadReg(LSM303C_ACC_XL_REFERENCE, &valueL) )
	  return MEMS_ERROR;
  
  if( !LSM303C_ACC_ReadReg(LSM303C_ACC_XH_REFERENCE, &valueH) )
	  return MEMS_ERROR;
  
  buff->AXIS_X = (i16_t)( (valueH << 8) | valueL );
  
  if( !LSM303C_ACC_ReadReg(LSM303C_ACC_YL_REFERENCE, &valueL) )
	  return MEMS_ERROR;
  
  if( !LSM303C_ACC_ReadReg(LSM303C_ACC_YH_REFERENCE, &valueH) )
	  return MEMS_ERROR;
  
  buff->AXIS_Y = (i16_t)( (valueH << 8) | valueL );

  if( !LSM303C_ACC_ReadReg(LSM303C_ACC_ZL_REFERENCE, &valueL) )
	  return MEMS_ERROR;
  
  if( !LSM303C_ACC_ReadReg(LSM303C_ACC_ZH_REFERENCE, &valueH) )
	  return MEMS_ERROR;
  
  buff->AXIS_Z = (i16_t)( (valueH << 8) | valueL );  
	  
  return MEMS_SUCCESS;  
}
/*******************************************************************************
* Function Name  : LSM303C_ACC_SetActivity_Threshold
* Description    : Set Interrupt Threshold data row
* Input          : threshold row value [u8_t]
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t LSM303C_ACC_SetActivity_Threshold(u8_t buff) {
  
  if( !LSM303C_ACC_WriteReg(LSM303C_ACC_ACT_TSH, buff) )
	  return MEMS_ERROR;	  
  return MEMS_SUCCESS;  
}

/*******************************************************************************
* Function Name  : LSM303C_ACC_SetActivity_Duration
* Description    : Set Interrupt Threshold data row
* Input          : threshold row value [u8_t]
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t LSM303C_ACC_SetActivity_Duration(u8_t buff) {
  
  if( !LSM303C_ACC_WriteReg(LSM303C_ACC_ACT_DUR, buff) )
	  return MEMS_ERROR;	  
  return MEMS_SUCCESS;  
}

/*******************************************************************************
* Function Name  : LSM303C_ACC_SetFIFO_threshold
* Description    : FIFO Threshold data row
* Input          : threshold row value [u8_t]
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
//WARNING: value from 0x00->0x1F
status_t LSM303C_ACC_SetFIFO_threshold(u8_t buff) {

  u8_t value;
  
  buff &= 0x1F; //coerce

  if (!LSM303C_ACC_ReadReg(LSM303C_ACC_FIFO_CTRL, &value) )
    return MEMS_ERROR;

  value &= ~0x1F; //mask
  value |= buff;

  if( !LSM303C_ACC_WriteReg(LSM303C_ACC_FIFO_CTRL, value) )
    return MEMS_ERROR;

  return MEMS_SUCCESS;  
}


/*******************************************************************************
* Function Name  : LSM303C_ACC_GetFIFO_StoredData
* Description    : FIFO Stored data row
* Input          : Stored data row value [u8_t]
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
//WARNING: value from 0x00->0x1F
status_t LSM303C_ACC_GetFIFO_StoredData(u8_t buff) {

  if (!LSM303C_ACC_ReadReg(LSM303C_ACC_FIFO_SRC, &buff) )
    return MEMS_ERROR;
	
  buff &= 0x1F; //coerce	

  return MEMS_SUCCESS;  
}



