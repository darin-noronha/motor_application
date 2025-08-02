

#ifndef SPI_CPU_H_
#define SPI_CPU_H_


/*********************************************************************************************************************/
/*-----------------------------------------------------Includes------------------------------------------------------*/
/*********************************************************************************************************************/
#include "Ifx_Types.h"
#include "IfxQspi_SpiMaster.h"
#include "IfxQspi_SpiSlave.h"

/*********************************************************************************************************************/
/*------------------------------------------------------Macros-------------------------------------------------------*/
/*********************************************************************************************************************/
#define DATA_TO_TRANSFER     1                       /* Buffers size 5 -> 3
' */

/*********************************************************************************************************************/
/*-------------------------------------------------Data Structures---------------------------------------------------*/
/*********************************************************************************************************************/
typedef struct
{
    uint32 spiMasterTxBuffer[DATA_TO_TRANSFER];       /* QSPI Master Transmit buffer   */
    uint32 spiMasterRxBuffer[DATA_TO_TRANSFER];       /* QSPI Master Receive buffer    */
    uint32 spiSlaveTxBuffer[DATA_TO_TRANSFER];        /* QSPI Slave Transmit buffer    */
    uint32 spiSlaveRxBuffer[DATA_TO_TRANSFER];        /* QSPI Slave Receive buffer     */
} qspiBuffers;

typedef struct
{
    qspiBuffers               spiBuffers;           /* Buffers instance              */
    IfxQspi_SpiMaster         spiMaster;            /* QSPI Master handle            */
    IfxQspi_SpiMaster_Channel spiMasterChannel;     /* QSPI Master Channel handle    */
    IfxQspi_SpiSlave          spiSlave;             /* QSPI Slave handle             */
} qspiComm;

/*********************************************************************************************************************/
/*------------------------------------------------Function Prototypes------------------------------------------------*/
/*********************************************************************************************************************/
void initPeripherals(void);
void transferData(void);
uint32 verifyData(void);
void initQSPI(void);


#endif /* SPI_CPU_H_ */
