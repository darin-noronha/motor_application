
#include "Ifx_Types.h"
#include "IfxCpu.h"
#include "IfxScuWdt.h"
#include "Bsp.h"
#include "IfxPort.h"
#include "IfxGtm_Tom_Pwm.h"          //GTM Module
#include "GTM_TOM_PWM.h"            //GTM PWM
#include "TLE9210x_ApplLayer.h"    //TLE API
#include "TLE9210x_FuncLayer.h"   //For Normal Mode Operation
#include "SPI.h"                 //For QSPI Module


#define BUTTON   &MODULE_P00, 7   /* Port pin for the button */

bool press = true;
IFX_ALIGN(4) IfxCpu_syncEvent g_cpuSyncEvent = 0;


void initButton(void){

    IfxPort_setPinMode(BUTTON, IfxPort_Mode_inputPullUp);

}
void TLE9210x_Auto_Cycle_Tasks(void)
{
    //Processing of the cyclic tasks
    TLE9210x_ApplLayer_deviceDriverCyclicTask();
    while(TLE9210x_ApplLayer_deviceDriverCyclicTask() == TLE9210x_DEVICEDRIVER_ERRORLOG_BUSY)
    {
      // MISRA
    }
}
void TLE9210x_Activate_Motor_One_PWM(bool set)
{
    //Enable PWM channel 1
    TLE9210x_FuncLayer_set_Control_PWMSET_PWM1_EN(1u);
    TLE9210x_Auto_Cycle_Tasks();

    //Set Allocation of the PWM channel 1 to half-bridge 1
    TLE9210x_FuncLayer_set_Control_PWMSET_PWM1_HB(0u);
    TLE9210x_Auto_Cycle_Tasks();

    //Set half-bridge 1 drain-source overvoltage threshold to 0.50V
    TLE9210x_FuncLayer_set_Control_VDS1_HB1VDSTH(0x5u);
    TLE9210x_Auto_Cycle_Tasks();

    //Set half-bridge 2 drain-source overvoltage threshold to 0.50V
    TLE9210x_FuncLayer_set_Control_VDS1_HB2VDSTH(0x5u);
    TLE9210x_Auto_Cycle_Tasks();

    //Set Drain-source monitoring of half-bridge 1 to CSIN1 - VSH1
    TLE9210x_FuncLayer_set_Control_VDS1_HB1D(1u);
    TLE9210x_Auto_Cycle_Tasks();

    //Set Drain-source monitoring of half-bridge 2 to CSIN1 - VSH1
    TLE9210x_FuncLayer_set_Control_VDS1_HB2D(1u);
    TLE9210x_Auto_Cycle_Tasks();

    //Set Drain-source monitoring of half-bridge 3 to CSIN1 - VSH1
    TLE9210x_FuncLayer_set_Control_VDS1_HB3D(1u);
    TLE9210x_Auto_Cycle_Tasks();

    //Set Drain-source monitoring of half-bridge 4 to CSIN1 - VSH1
    TLE9210x_FuncLayer_set_Control_VDS1_HB4D(1u);
    TLE9210x_Auto_Cycle_Tasks();

    //Set gate driver charge current of PWM Channel 1 (Active MOSFET): Nom. charge current to 12.5[mA] & Max. deviation to nominal values to +/-38%
    TLE9210x_FuncLayer_set_Control_PWM_ICHG_ACT_ICHG1(0x8u);
    TLE9210x_Auto_Cycle_Tasks();

    //Set static gate driver charge and discharge currents 2: Nom. charge current to 37.1[mA] & Nom. discharge current to 39.5[mA] & Max. deviation to typ. values to +/-28%
    TLE9210x_FuncLayer_set_Control_ST_ICHG_ICHGST2(8u);
    TLE9210x_Auto_Cycle_Tasks();

    //Set static gate driver charge and discharge currents 1: Nom. charge current to 37.1[mA] & Nom. discharge current to 39.5[mA] & Max. deviation to typ. values to +/-28%
    TLE9210x_FuncLayer_set_Control_ST_ICHG_ICHGST1(8u);
    TLE9210x_Auto_Cycle_Tasks();


    if(set){
    //Set half-bridge output 1 to highside
    TLE9210x_FuncLayer_set_Control_HBMODE_HB1MODE(2u);
    TLE9210x_Auto_Cycle_Tasks();

    //Set half-bridge output 2 to lowside
    TLE9210x_FuncLayer_set_Control_HBMODE_HB2MODE(1u);
    TLE9210x_Auto_Cycle_Tasks();
    }else{
        //Set half-bridge output 1 to highside
            TLE9210x_FuncLayer_set_Control_HBMODE_HB1MODE(1u);
            TLE9210x_Auto_Cycle_Tasks();

            //Set half-bridge output 2 to lowside
            TLE9210x_FuncLayer_set_Control_HBMODE_HB2MODE(2u);
            TLE9210x_Auto_Cycle_Tasks();
    }


    //Set bridge-driver to active mode
    TLE9210x_FuncLayer_set_Control_GENCTRL2_BD_PASS(0u);
    TLE9210x_Auto_Cycle_Tasks();
}
void TLE9210x_Deactivate_Motors(void) {
    //Deactivate all half-bridges
    TLE9210x_RegLayer_set_Control_HBMODE(0x0u);
}
void checkButton(void){
    if (IfxPort_getPinState(BUTTON) == 0) //button is pressed
        {
        waitTime(IfxStm_getTicksFromMicroseconds(&MODULE_STM0, 1000000));
        press = !press;
        TLE9210x_Deactivate_Motors();
        waitTime(IfxStm_getTicksFromMicroseconds(&MODULE_STM0, 100000));
        TLE9210x_Activate_Motor_One_PWM(press);
        }
}



void TLE9210x_Activate_Motor_One_Static(bool set)
{
    //Set static gate driver charge and discharge currents 2: Nom. charge current to 37.1[mA] & Nom. discharge current to 39.5[mA] & Max. deviation to typ. values to +/-28%
    TLE9210x_FuncLayer_set_Control_ST_ICHG_ICHGST2(8u);
    TLE9210x_Auto_Cycle_Tasks();

    //Set static gate driver charge and discharge currents 1: Nom. charge current to 37.1[mA] & Nom. discharge current to 39.5[mA] & Max. deviation to typ. values to +/-28%
    TLE9210x_FuncLayer_set_Control_ST_ICHG_ICHGST1(8u);
    TLE9210x_Auto_Cycle_Tasks();


    if(set){
        //Set half-bridge output 1 to highside
            TLE9210x_FuncLayer_set_Control_HBMODE_HB1MODE(1u);
            TLE9210x_Auto_Cycle_Tasks();

            //Set half-bridge output 2 to lowside
            TLE9210x_FuncLayer_set_Control_HBMODE_HB2MODE(2u);
            TLE9210x_Auto_Cycle_Tasks();
    }else{
        //Set half-bridge output 1 to highside
            TLE9210x_FuncLayer_set_Control_HBMODE_HB1MODE(2u);
            TLE9210x_Auto_Cycle_Tasks();

            //Set half-bridge output 2 to lowside
            TLE9210x_FuncLayer_set_Control_HBMODE_HB2MODE(1u);
            TLE9210x_Auto_Cycle_Tasks();
    }


    //Set bridge-driver to active mode
    TLE9210x_FuncLayer_set_Control_GENCTRL2_BD_PASS(0u);
    TLE9210x_Auto_Cycle_Tasks();
}

void core0_main(void)
{
    IfxCpu_enableInterrupts();

    /* !!WATCHDOG0 AND SAFETY WATCHDOG ARE DISABLED HERE!!
     * Enable the watchdogs and service them periodically if it is required
     */
    IfxScuWdt_disableCpuWatchdog(IfxScuWdt_getCpuWatchdogPassword());
    IfxScuWdt_disableSafetyWatchdog(IfxScuWdt_getSafetyWatchdogPassword());
    
    /* Wait for CPU sync event */
    IfxCpu_emitEvent(&g_cpuSyncEvent);
    IfxCpu_waitEvent(&g_cpuSyncEvent, 1);
    
    initGtmTomPwm(); //init & start PWM
    initButton();
    //Initialize TLE9210x
    TLE9210x_ApplLayer_Init(true); // use false to leave the watchdog enabled / use true to disable the watchdog
    waitTime(IfxStm_getTicksFromMicroseconds(&MODULE_STM0, 300));

    //Reset VDSE-bit to 0
    TLE9210x_RegLayer_set_Status_DSOV(0u);
    waitTime(IfxStm_getTicksFromMicroseconds(&MODULE_STM0, 700000));

    //Reset NPOR-bit to 1
    TLE9210x_RegLayer_set_Status_GENSTAT(0u);
    waitTime(IfxStm_getTicksFromMicroseconds(&MODULE_STM0, 700000));

    /* Activate Motors staticly */
    //TLE9210x_Activate_Motor_One_Static();


    while(1){
        TLE9210x_ApplLayer_deviceDriverCyclicTask();
        waitTime(IfxStm_getTicksFromMicroseconds(&MODULE_STM0, 20));
        checkButton();

    }
 return;

};
