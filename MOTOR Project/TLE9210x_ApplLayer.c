/**
 *	@file TLE9210x_ApplLayer.c
 *	@author Infineon Technologies AG
 *	@date 11.09.2023
 *	@brief TLE9210x specific device driver implementation from Infineon Technologies AG.
 *
 ************************************************************************
 *
 * Copyright (c) 2023, Infineon Technologies AG
 * All rights reserved.
 *
 ************************************************************************
 */

/** \addtogroup TLE9210x_APPLICATION_LAYER
 *  @{
 */

/************************************************************************
**                             Includes                                **
************************************************************************/

#include "TLE9210x_ApplLayer.h"
#include "TLE9210x_RegLayer.h"
#include "TLE9210x_defines.h"
#include "TLE9210x_ServLayer.h"
#include "TLE9210x_HAL.h"

/************************************************************************
**                      Local Macro Definitions                        **
************************************************************************/

/************************************************************************
**                      Local Type Definitions                         **
************************************************************************/

/************************************************************************
**                    Local Function Definitions                       **
************************************************************************/

static void TLE9210x_ApplLayer_update_Status_GENSTAT(uint16_t u16_data);
static void TLE9210x_ApplLayer_update_Status_DSOV(uint16_t u16_data);
static void TLE9210x_ApplLayer_update_Status_HBVOUT_PWMERR(uint16_t u16_data);
static void TLE9210x_ApplLayer_update_Status_EFF_TDON_OFF1(uint16_t u16_data);
static void TLE9210x_ApplLayer_update_Status_EFF_TDON_OFF2(uint16_t u16_data);
static void TLE9210x_ApplLayer_update_Status_EFF_TDON_OFF3(uint16_t u16_data);
static void TLE9210x_ApplLayer_update_Status_TRISE_FALL1(uint16_t u16_data);
static void TLE9210x_ApplLayer_update_Status_TRISE_FALL2(uint16_t u16_data);
static void TLE9210x_ApplLayer_update_Status_TRISE_FALL3(uint16_t u16_data);
static void TLE9210x_ApplLayer_update_Status_DEVID(uint16_t u16_data);

/************************************************************************
**                    Local Variable Definitions                       **
************************************************************************/

static TLE9210x_fpDEVICE_getDeviceReg TLE9210x_fp_cyclicStatGetFuncs[] = {  &TLE9210x_RegLayer_get_Status_GENSTAT,
                                                                            &TLE9210x_RegLayer_get_Status_DSOV,
                                                                            &TLE9210x_RegLayer_get_Status_HBVOUT_PWMERR,
                                                                            &TLE9210x_RegLayer_get_Status_EFF_TDON_OFF1,
                                                                            &TLE9210x_RegLayer_get_Status_EFF_TDON_OFF2,
                                                                            &TLE9210x_RegLayer_get_Status_EFF_TDON_OFF3,
                                                                            &TLE9210x_RegLayer_get_Status_TRISE_FALL1,
                                                                            &TLE9210x_RegLayer_get_Status_TRISE_FALL2,
                                                                            &TLE9210x_RegLayer_get_Status_TRISE_FALL3,
                                                                            &TLE9210x_RegLayer_get_Status_DEVID
                                                                            };

static TLE9210x_fpDEVICE_updateRamReg TLE9210x_fp_cyclicStatUpdateFuncs[] = { &TLE9210x_ApplLayer_update_Status_GENSTAT,
                                                                              &TLE9210x_ApplLayer_update_Status_DSOV,
                                                                              &TLE9210x_ApplLayer_update_Status_HBVOUT_PWMERR,
                                                                              &TLE9210x_ApplLayer_update_Status_EFF_TDON_OFF1,
                                                                              &TLE9210x_ApplLayer_update_Status_EFF_TDON_OFF2,
                                                                              &TLE9210x_ApplLayer_update_Status_EFF_TDON_OFF3,
                                                                              &TLE9210x_ApplLayer_update_Status_TRISE_FALL1,
                                                                              &TLE9210x_ApplLayer_update_Status_TRISE_FALL2,
                                                                              &TLE9210x_ApplLayer_update_Status_TRISE_FALL3,
                                                                              &TLE9210x_ApplLayer_update_Status_DEVID
                                                                              };

/************************************************************************
**                    Global Variable Definitions                      **
************************************************************************/

extern TLE9210x_sDEVICE_deviceDriver TLE9210x_s_deviceDriver = {  TLE9210x_DEVICEDRIVER_STATUS_INIT,
                                                                  TLE9210x_DEVICEDRIVER_ERRORLOG_INIT,
                                                                  0u,
                                                                  {0, 0, 0},
                                                                  0u,
                                                                  NULL,
                                                                  NULL,
                                                                  NULL,
                                                                  0u,
                                                                };/* polyspace MISRA2012:8.5 [Justified: Low] "Necessary due to double declaration issue" */

typedef uint8_t (*fpDEVICE_initDeviceReg)(uint16_t initValue); 

/************************************************************************
**                    Global Function Definitions                      **
************************************************************************/
/**
 * \brief Executes the device driver cyclic task for the TLE9210x device driver
 * 
 * \return The status of the device driver cyclic task as an 8-bit unsigned integer.
 */
uint8_t TLE9210x_ApplLayer_deviceDriverCyclicTask(void) /* polyspace MISRA2012:8.7 [Justified: Low] "Will be called by the main function. Has to be implemented in later stages." */
{ 
  static uint8_t u8_cyclicStatUpdateCount = 0;
  uint16_t u16_tempRegData;
  
  switch(TLE9210x_s_deviceDriver.u8_deviceDriverStatus)
  {
  case TLE9210x_DEVICEDRIVER_STATUS_INIT:
    /* dynamic set register requests to write to control registers have highest priority */
    if(TLE9210x_s_deviceDriver.fp_setReg != NULL)
    {
      TLE9210x_s_deviceDriver.u8_deviceDriverStatus = TLE9210x_DEVICEDRIVER_STATUS_CYCLIC_UPDATE;
      TLE9210x_s_deviceDriver.fp_setReg(TLE9210x_s_deviceDriver.u16_setBitValue);
      TLE9210x_s_deviceDriver.fp_setReg = NULL;
      TLE9210x_s_deviceDriver.u8_deviceDriverErrorLog = TLE9210x_DEVICEDRIVER_ERRORLOG_STARTED;
      break;
    }
    /* cyclic get register requests to update status registers have lowest priority */
    else if(TLE9210x_s_deviceDriver.fp_getReg != NULL)
    {
      TLE9210x_s_deviceDriver.u8_deviceDriverStatus = TLE9210x_DEVICEDRIVER_STATUS_CYCLIC_UPDATE;
      TLE9210x_s_deviceDriver.fp_getReg = TLE9210x_fp_cyclicStatGetFuncs[u8_cyclicStatUpdateCount];
      TLE9210x_s_deviceDriver.fp_updateRamReg = TLE9210x_fp_cyclicStatUpdateFuncs[u8_cyclicStatUpdateCount];
      TLE9210x_s_deviceDriver.fp_getReg();
      if(u8_cyclicStatUpdateCount < TLE9210x_DEVICEDRIVER_CYCLICSTATUPDATENUM)
      {
        u8_cyclicStatUpdateCount++;
      }
      else
      {
        u8_cyclicStatUpdateCount = 0;
      }
      TLE9210x_s_deviceDriver.u8_deviceDriverErrorLog = TLE9210x_DEVICEDRIVER_ERRORLOG_STARTED;
      break;
    }
    else
    {
      TLE9210x_s_deviceDriver.u8_deviceDriverErrorLog = TLE9210x_DEVICEDRIVER_ERRORLOG_INIT;
      TLE9210x_s_deviceDriver.u8_deviceDriverStatus = TLE9210x_DEVICEDRIVER_STATUS_INIT;
      break;
    }
    break;
    
  case TLE9210x_DEVICEDRIVER_STATUS_CYCLIC_UPDATE:
    /* update SPI status */
    TLE9210x_ServLayer_SpiInterfaceStatus(); //does not return a TLE type var?
    if(TLE9210x_s_deviceDriver.u8_statusSPI == TLE9210x_DEVICEDRIVER_SPI_NEW_DATA)
    {
      /* get SPI Data */
      TLE9210x_ServLayer_getSpiData();
      /* clear device driver error log */
      TLE9210x_s_deviceDriver.u8_deviceDriverErrorLog = TLE9210x_DEVICEDRIVER_ERRORLOG_INIT;
      /* update GSB */
      TLE9210x->GSB.byte = TLE9210x_s_deviceDriver.u8_SpiRx[0];
      /* update status register, if provided */
      if(TLE9210x_s_deviceDriver.fp_updateRamReg != NULL)
      {
        u16_tempRegData = (uint16_t)(((uint16_t)TLE9210x_s_deviceDriver.u8_SpiRx[2] << 8) | ((uint16_t)TLE9210x_s_deviceDriver.u8_SpiRx[1]));
        TLE9210x_s_deviceDriver.fp_updateRamReg(u16_tempRegData);
        TLE9210x_s_deviceDriver.fp_updateRamReg = NULL;
      }

      /* Start next SPI command */
      /* dynamic set register requests to write to control registers have highest priority */
      if(TLE9210x_s_deviceDriver.fp_setReg != NULL)
      {
        TLE9210x_s_deviceDriver.u8_deviceDriverStatus = TLE9210x_DEVICEDRIVER_STATUS_CYCLIC_UPDATE;
        TLE9210x_s_deviceDriver.fp_setReg(TLE9210x_s_deviceDriver.u16_setBitValue);
        TLE9210x_s_deviceDriver.fp_setReg = NULL;
        TLE9210x_s_deviceDriver.u8_deviceDriverErrorLog = TLE9210x_s_deviceDriver.u8_deviceDriverErrorLog | TLE9210x_DEVICEDRIVER_ERRORLOG_STARTED;
        break;
      }
      /* cyclic get register requests to update status registers have lowest priority */
      else if(TLE9210x_s_deviceDriver.fp_getReg != NULL)
      {
        TLE9210x_s_deviceDriver.u8_deviceDriverStatus = TLE9210x_DEVICEDRIVER_STATUS_CYCLIC_UPDATE;
        TLE9210x_s_deviceDriver.fp_getReg = TLE9210x_fp_cyclicStatGetFuncs[u8_cyclicStatUpdateCount];
        TLE9210x_s_deviceDriver.fp_updateRamReg = TLE9210x_fp_cyclicStatUpdateFuncs[u8_cyclicStatUpdateCount];
        TLE9210x_s_deviceDriver.fp_getReg();
        if(u8_cyclicStatUpdateCount < TLE9210x_DEVICEDRIVER_CYCLICSTATUPDATENUM)
        {
          u8_cyclicStatUpdateCount ++;
        }
        else
        {
          u8_cyclicStatUpdateCount = 0;
        }
        TLE9210x_s_deviceDriver.u8_deviceDriverErrorLog = TLE9210x_s_deviceDriver.u8_deviceDriverErrorLog | TLE9210x_DEVICEDRIVER_ERRORLOG_STARTED;
        break;
      }
      else
      {
        TLE9210x_s_deviceDriver.u8_deviceDriverStatus = TLE9210x_DEVICEDRIVER_STATUS_INIT;
        break;
      }
    }
    else
    {
      TLE9210x_s_deviceDriver.u8_deviceDriverErrorLog = TLE9210x_DEVICEDRIVER_ERRORLOG_BUSY;
    }
    break;
    
  default:
    /* invalid device driver status */
    break;
  }
  
  return TLE9210x_s_deviceDriver.u8_deviceDriverErrorLog;
}

/**
 * \brief Initializes the TLE9210x application layer.
 *
 * \param disableWatchdog True to disable the watchdog timer, false otherwise.
 */
void TLE9210x_ApplLayer_Init(uint8_t disableWatchdog)
{
    fpDEVICE_initDeviceReg fp_initRegTemp;
    uint8_t u8_initRegisterCounter;
    uint8_t u8_initRegisterNum;
    uint8_t u8_initStatUpdateCount = 0;
    uint16_t u16_initRegValueTemp;

    fpDEVICE_initDeviceReg fp_initRegAll[] = {&TLE9210x_FuncLayer_set_Control_VDS1,
                                              #if TLE9210x_Variant == TLE92108
                                              &TLE9210x_FuncLayer_set_Control_VDS2,
                                              #endif
                                              &TLE9210x_FuncLayer_set_Control_CCP_BLK1, &TLE9210x_FuncLayer_set_Control_CCP_BLK2_ACT, &TLE9210x_FuncLayer_set_Control_CCP_BLK2_FW, &TLE9210x_FuncLayer_set_Control_HBMODE,
                                              &TLE9210x_FuncLayer_set_Control_PWMSET, &TLE9210x_FuncLayer_set_Control_TPRECHG, &TLE9210x_FuncLayer_set_Control_HBIDIAG, &TLE9210x_FuncLayer_set_Control_ST_ICHG, &TLE9210x_FuncLayer_set_Control_PWM_PCHG_INIT,
                                              &TLE9210x_FuncLayer_set_Control_PWM_ICHG_ACT, &TLE9210x_FuncLayer_set_Control_PWM_ICHG_FW, &TLE9210x_FuncLayer_set_Control_PWM_IDCHG_ACT, &TLE9210x_FuncLayer_set_Control_PWM_PDCHG_INIT, &TLE9210x_FuncLayer_set_Control_PWM_ICHGMAX_CCP_BLK3_ACT, &TLE9210x_FuncLayer_set_Control_PWM_ICHGMAX_CCP_BLK3_FW,
                                              &TLE9210x_FuncLayer_set_Control_TDON_OFF1, &TLE9210x_FuncLayer_set_Control_TDON_OFF2, &TLE9210x_FuncLayer_set_Control_TDON_OFF3};
    
     uint16_t u16_initRegValueAll[] = {       TLE9210x_Control_VDS1_INIT,
                                              #if TLE9210x_Variant == TLE92108
                                              TLE9210x_Control_VDS2_INIT,
                                              #endif
                                              TLE9210x_Control_CCP_BLK1_INIT, TLE9210x_Control_CCP_BLK2_ACT_INIT, TLE9210x_Control_CCP_BLK2_FW_INIT, TLE9210x_Control_HBMODE_INIT,
                                              TLE9210x_Control_PWMSET_INIT, TLE9210x_Control_TPRECHG_INIT, TLE9210x_Control_HBIDIAG_INIT, TLE9210x_Control_ST_ICHG_INIT, TLE9210x_Control_PWM_PCHG_INIT_INIT,
                                              TLE9210x_Control_PWM_ICHG_ACT_INIT, TLE9210x_Control_PWM_ICHG_FW_INIT, TLE9210x_Control_PWM_IDCHG_ACT_INIT, TLE9210x_Control_PWM_PDCHG_INIT_INIT, TLE9210x_Control_PWM_ICHGMAX_CCP_BLK3_ACT_INIT, TLE9210x_Control_PWM_ICHGMAX_CCP_BLK3_FW_INIT,
                                              TLE9210x_Control_TDON_OFF1_INIT, TLE9210x_Control_TDON_OFF2_INIT, TLE9210x_Control_TDON_OFF3_INIT};
    
    
    TLE9210x_ServLayer_InitSpiInterface();

    //Trigger the Watchdog
    (void)TLE9210x_FuncLayer_set_Control_GENCTRL1_WDTRIG(0x1u); /* polyspace MISRA2012:8.7 [Justified: Low] "Given due architecture" */
    (void)TLE9210x_ApplLayer_deviceDriverCyclicTask();
    while(TLE9210x_ApplLayer_deviceDriverCyclicTask() == TLE9210x_DEVICEDRIVER_ERRORLOG_BUSY)
    {
      // MISRA
    }
    TLE9210x_WaitFor_us_HAL(300);

    /* Disable/Enable watchdog depending on parameter */             //CHECK
    if (disableWatchdog == TLE9210x_WATCHDOG_DISABLE) {
        //Set Unlock-PIN to disable the Watchdog
        TLE9210x_RegLayer_set_Control_GENCTRL1(0x80u);
        TLE9210x_WaitFor_us_HAL(10000);
        //Disable the Watchdog
        TLE9210x_RegLayer_set_Control_GENCTRL2(0x4200u);
        TLE9210x_WaitFor_us_HAL(10000);
    }

    /* Initialize all control registers */
    u8_initRegisterNum = (uint8_t)(sizeof(u16_initRegValueAll)/sizeof(u16_initRegValueAll[0]));
    for(u8_initRegisterCounter = 0; u8_initRegisterCounter < u8_initRegisterNum; u8_initRegisterCounter++)
    {
      fp_initRegTemp = fp_initRegAll[u8_initRegisterCounter];
      u16_initRegValueTemp = u16_initRegValueAll[u8_initRegisterCounter];
      fp_initRegTemp(u16_initRegValueTemp);
      (void)TLE9210x_ApplLayer_deviceDriverCyclicTask();
      while(TLE9210x_ApplLayer_deviceDriverCyclicTask() == TLE9210x_DEVICEDRIVER_ERRORLOG_BUSY)
      {
        // MISRA
      }
    }
    /* Initialize first get-register function pointer */
    TLE9210x_s_deviceDriver.fp_getReg = &TLE9210x_RegLayer_get_Status_GENSTAT;
    TLE9210x_s_deviceDriver.fp_updateRamReg = &TLE9210x_ApplLayer_update_Status_GENSTAT;
    /* Read out all status registers */
    for(u8_initStatUpdateCount = 0; u8_initStatUpdateCount < TLE9210x_DEVICEDRIVER_CYCLICSTATUPDATENUM; u8_initStatUpdateCount++)
    {
      (void)TLE9210x_ApplLayer_deviceDriverCyclicTask();
      while(TLE9210x_ApplLayer_deviceDriverCyclicTask() == TLE9210x_DEVICEDRIVER_ERRORLOG_BUSY)
      {
        // MISRA
      }
    }

    return;
}

/**
 * \brief Returns the GSB (Global Status Byte) of the TLE9210x device.
 *
 * \return The GSB of the device as a 16-bit unsigned integer.
 */
uint16_t TLE9210x_getGSB(void)
{
  return TLE9210x->GSB.byte;
}

/** \brief Update Register Shadow of Status_GENSTAT
*
*   \param u16_data : value to be written
*
*   \return void
*/
static void TLE9210x_ApplLayer_update_Status_GENSTAT(uint16_t u16_data)
{
  TLE9210x->Status_GENSTAT.reg = u16_data;
}

/** \brief Update Register Shadow of Status_DSOV
*
*   \param u16_data : value to be written
*
*   \return void
*/
static void TLE9210x_ApplLayer_update_Status_DSOV(uint16_t u16_data)
{
  TLE9210x->Status_DSOV.reg = u16_data;
}

/** \brief Update Register Shadow of Status_HBVOUT_PWMERR
*
*   \param u16_data : value to be written
*
*   \return void
*/
static void TLE9210x_ApplLayer_update_Status_HBVOUT_PWMERR(uint16_t u16_data)
{
  TLE9210x->Status_HBVOUT_PWMERR.reg = u16_data;
}

/** \brief Update Register Shadow of Status_EFF_TDON_OFF1
*
*   \param u16_data : value to be written
*
*   \return void
*/
static void TLE9210x_ApplLayer_update_Status_EFF_TDON_OFF1(uint16_t u16_data)
{
  TLE9210x->Status_EFF_TDON_OFF1.reg = u16_data;
}

/** \brief Update Register Shadow of Status_EFF_TDON_OFF2
*
*   \param u16_data : value to be written
*
*   \return void
*/
static void TLE9210x_ApplLayer_update_Status_EFF_TDON_OFF2(uint16_t u16_data)
{
  TLE9210x->Status_EFF_TDON_OFF2.reg = u16_data;
}

/** \brief Update Register Shadow of Status_EFF_TDON_OFF3
*
*   \param u16_data : value to be written
*
*   \return void
*/
static void TLE9210x_ApplLayer_update_Status_EFF_TDON_OFF3(uint16_t u16_data)
{
  TLE9210x->Status_EFF_TDON_OFF3.reg = u16_data;
}

/** \brief Update Register Shadow of Status_TRISE_FALL1
*
*   \param u16_data : value to be written
*
*   \return void
*/
static void TLE9210x_ApplLayer_update_Status_TRISE_FALL1(uint16_t u16_data)
{
  TLE9210x->Status_TRISE_FALL1.reg = u16_data;
}

/** \brief Update Register Shadow of Status_TRISE_FALL2
*
*   \param u16_data : value to be written
*
*   \return void
*/
static void TLE9210x_ApplLayer_update_Status_TRISE_FALL2(uint16_t u16_data)
{
  TLE9210x->Status_TRISE_FALL2.reg = u16_data;
}

/** \brief Update Register Shadow of Status_TRISE_FALL3
*
*   \param u16_data : value to be written
*
*   \return void
*/
static void TLE9210x_ApplLayer_update_Status_TRISE_FALL3(uint16_t u16_data)
{
  TLE9210x->Status_TRISE_FALL3.reg = u16_data;
}

/** \brief Update Register Shadow of Status_DEVID
*
*   \param u16_data : value to be written
*
*   \return void
*/
static void TLE9210x_ApplLayer_update_Status_DEVID(uint16_t u16_data)
{
  TLE9210x->Status_DEVID.reg = u16_data;
}

/** @}*/ /* End of group TLE9210x_APPLICATION_LAYER */
