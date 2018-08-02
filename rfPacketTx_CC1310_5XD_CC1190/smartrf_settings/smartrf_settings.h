#ifndef _SMARTRF_SETTINGS_H_
#define _SMARTRF_SETTINGS_H_

//*********************************************************************************
// Generated by SmartRF Studio version 2.8.0 ( build #41)
// Compatible with SimpleLink SDK version: CC13x0 SDK 1.30.xx.xx
// Device: CC1310 Rev. 2.1
// 
//*********************************************************************************

#include <driverlib/rf_mailbox.h>
#include <driverlib/rf_common_cmd.h>
#include <driverlib/rf_prop_cmd.h>
#include <ti/drivers/rf/RF.h>


// TI-RTOS RF Mode Object
extern RF_Mode RF_prop;


// RF Core API commands
extern rfc_CMD_PROP_RADIO_DIV_SETUP_t RF_cmdPropRadioDivSetup;
extern rfc_CMD_FS_t RF_cmdFs;
extern rfc_CMD_PROP_TX_t RF_cmdPropTx;
extern rfc_CMD_PROP_RX_t RF_cmdPropRx;


#endif // _SMARTRF_SETTINGS_H_
