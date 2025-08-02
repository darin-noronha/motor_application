/**
 *	@file TLE9210x_ApplLayer.h
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

#ifndef TLE9210x_APPLLAYER_H
#define TLE9210x_APPLLAYER_H

/************************************************************************
**                             Includes                                **
************************************************************************/

#include "TLE9210x_FuncLayer.h"

/************************************************************************
**                      Local Macro Definitions                        **
************************************************************************/

/** \brief Indication that the initialization process is currently ongoing.
*/
#define TLE9210x_DEVICEDRIVER_STATUS_INIT                (0u)

/** \brief Indication that the cyclic update is currently ongoing.
*/
#define TLE9210x_DEVICEDRIVER_STATUS_CYCLIC_UPDATE       (1u)

/** \brief Indication that the error log is initialized.
*/
#define TLE9210x_DEVICEDRIVER_ERRORLOG_INIT              (0u)

/** \brief Indication that the error log is started.
*/
#define TLE9210x_DEVICEDRIVER_ERRORLOG_STARTED           (1u)

/** \brief Indication that the device driver is still waiting for new data.
*/
#define TLE9210x_DEVICEDRIVER_ERRORLOG_BUSY              (2u)

/** \brief Indication that an SPI error occured.
*/
#define TLE9210x_DEVICEDRIVER_ERRORLOG_SPI_RECEIVE_ERR   (3u)

/** \brief Number of status registers to be updated by the cyclic function.
*/
#define TLE9210x_DEVICEDRIVER_CYCLICSTATUPDATENUM       (9u)

/** \brief Indication that new SPI data are available.
*/
#define TLE9210x_DEVICEDRIVER_SPI_NEW_DATA               (1u)

/** \brief enables Watchdog
*/
#define TLE9210x_WATCHDOG_ENABLE                          (0u)

/** \brief disables Watchdog
*/
#define TLE9210x_WATCHDOG_DISABLE                         (1u)

/************************************************************************
**                      Local Type Definitions                         **
************************************************************************/

/************************************************************************
**                    Local Variable Definitions                       **
************************************************************************/

/************************************************************************
**                    Local Function Definitions                       **
************************************************************************/

/************************************************************************
**                    Global Variable Definitions                      **
************************************************************************/

/** \brief function pointer TLE9210x_fpDEVICE_setDeviceReg
*/
typedef void (*TLE9210x_fpDEVICE_setDeviceReg)(uint16_t);

/** \brief function pointer TLE9210x_fpDEVICE_getDeviceReg
*/
typedef void (*TLE9210x_fpDEVICE_getDeviceReg)(void);

/** \brief function pointer TLE9210x_fpDEVICE_updateRamReg
*/
typedef void (*TLE9210x_fpDEVICE_updateRamReg)(uint16_t);

/** \struct TLE9210x_sDEVICE_deviceDriver
 *  \brief Struct for Device Driver cyclic task
 */
typedef struct _TLE9210x_sDEVICE_deviceDriver
{
  uint8_t u8_deviceDriverStatus;                              /*!<  device driver status */
  uint8_t u8_deviceDriverErrorLog;                            /*!<  device driver error log */
  uint8_t u8_statusSPI;                                       /*!<  status of the SPI interface */
  uint8_t u8_SpiRx[3];                                        /*!<  content of last received SPI message */
  uint8_t u8_CrcResult;                                       /*!<  expected result for crc of last received SPI message */
  TLE9210x_fpDEVICE_setDeviceReg fp_setReg;                   /*!<  function pointer to next set register function */
  TLE9210x_fpDEVICE_getDeviceReg fp_getReg;                   /*!<  function pointer to next get register function */
  TLE9210x_fpDEVICE_updateRamReg fp_updateRamReg;             /*!<  function pointer to next update RAM register function */
  uint16_t u16_setBitValue;                                   /*!<  value to be used in next set register function */
} TLE9210x_sDEVICE_deviceDriver;

extern TLE9210x_sDEVICE_deviceDriver TLE9210x_s_deviceDriver;

/************************************************************************
**                    Global Function Definitions                      **
************************************************************************/
void TLE9210x_ApplLayer_Init(uint8_t disableWatchdog);
uint8_t TLE9210x_ApplLayer_deviceDriverCyclicTask(void);
uint16_t TLE9210x_getGSB(void);

/** @}*/ /* End of group TLE9210x_APPLICATION_LAYER */

#endif /* TLE9210x_APPLLAYER_H */
