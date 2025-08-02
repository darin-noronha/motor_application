/**
 *  @file TLE9210x_HAL.c
 *  @author Infineon Technologies AG
 *	@date 11.09.2023
 *	@brief TLE9210x specific device driver implementation from Infineon Technologies AG.
 *
 ***********************************************************************************************************************
 *
 * Copyright (c) 2023, Infineon Technologies AG
 * All rights reserved.
 *
 **********************************************************************************************************************
 */

/** \addtogroup TLE9210x_HAL
 *  @{
 */

/*******************************************************************************
**                                  Includes                                  **
*******************************************************************************/
#include "TLE9210x_HAL.h"
#include "IfxQspi_SpiMaster.h"
#include "SPI.h"
#include "TLE9210x_defines.h"
#include "Bsp.h"


/*******************************************************************************
**                        Global Variable Definitions                         **
*******************************************************************************/
static uint32_t rxBuffer;

static uint32_t txBuffer;
extern qspiComm g_qspi;

/******************************************************************************/
/**                        Local Function Definitions                        **/
/******************************************************************************/

/** \brief Wait function
 *
 *  \note Within this function the microcontroller specific wait routine must be called
 * 
 *  \param u32_waitTime wait time in microseconds
 * 
 *  \return void
 */
void TLE9210x_WaitFor_us_HAL(uint32_t u32_waitTime)
{
  /* Place here the code for waiting */
    waitTime(IfxStm_getTicksFromMicroseconds(&MODULE_STM0, u32_waitTime));
    //IfxStm_waitUs();
}

/** \brief Initialize the SPI peripheral
 *
 *  \note Within this function the microcontroller specific SPI initialization routine must be called
 * 
 *  \return void
 */
void TLE9210x_InitSpiInterface_HAL(void)
{
  /* Call here micro controller dependent SPI initialization function */
    //SPI is initialized in initPeripherals via initQSPI
    initQSPI();
}

/** \brief Perform an SPI transfer
 *
 *  \note Within this function the microcontroller specific SPI initialization routine must be called
 * 
 *  \param u32_sendBuffer data to be transmitted
 * 
 *  \return void
 */
uint32 sendCounter = 0;
void TLE9210x_SendSpiData_HAL(uint32_t u32_sendBuffer)
{
        txBuffer = u32_sendBuffer;
        /* Instruct the SPI Slave to receive a data stream of defined length */
       /* Send a data stream through the SPI Master to the TLE board */

        IfxQspi_SpiMaster_exchange(&g_qspi.spiMasterChannel, &txBuffer, &rxBuffer, 1);
        sendCounter++;
}

/** \brief Read out the SPI RX buffer of the microcontroller
 *  
 *  \return void
 */

uint32_t TLE9210x_ReadSpiRxBuffer_HAL(void)
{
  /* Call SPI Read function here and write into rxBuffer */

        return rxBuffer;

}

/** \brief Get the Status of the SPI interface
 * 
 *  Within this function the microcontroller specific SPI status function has to be implemented or called.
 *  The result has to be returned by this function.
 *  
 *  \return uint8_t status of SPI transfer
 */


uint8_t TLE9210x_SpiInterfaceStatus_HAL(void)
{

  return !IfxQspi_SpiMaster_getStatus(&g_qspi.spiMasterChannel);
}


/** @}*/ /* End of group TLE9210x_HAL_C */
