#include <xdc/std.h>
#include <xdc/runtime/System.h>
#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Task.h>
#include <ti/drivers/UART.h>
#include <ti/drivers/uart/UARTCC26XX.h>
#include <ti/drivers/ADC.h>
#include <ti/drivers/PIN.h>
#include <ti/drivers/GPIO.h>
#include <ti/drivers/Power.h>
#include <ti/drivers/power/PowerCC26XX.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Board.h"

/* ADC conversion result variables */
uint16_t adcValue;

UARTCC26XX_Object uartCC26XXObjects[CC1310_LAUNCHXL_UARTCOUNT];
unsigned char uartCC26XXRingBuffer[CC1310_LAUNCHXL_UARTCOUNT][32];

const UARTCC26XX_HWAttrsV2 uartCC26XXHWAttrs[CC1310_LAUNCHXL_UARTCOUNT] = {
    {
        .baseAddr       = UART0_BASE,
        .powerMngrId    = PowerCC26XX_PERIPH_UART0,
        .intNum         = INT_UART0_COMB,
        .intPriority    = ~0,
        .swiPriority    = 0,
        .txPin          = IOID_5,
        .rxPin          = IOID_4,
        .ctsPin         = PIN_UNASSIGNED,
        .rtsPin         = PIN_UNASSIGNED,
        .ringBufPtr     = uartCC26XXRingBuffer[0],
        .ringBufSize    = sizeof(uartCC26XXRingBuffer[0])
    }
};
const UART_Config UART_config[] = {
    {
        .fxnTablePtr = &UARTCC26XX_fxnTable,
        .object      = &uartCC26XXObjects[0],
        .hwAttrs     = &uartCC26XXHWAttrs[0]
    },
    {NULL, NULL, NULL}
};
const uint_least8_t UART_count = CC1310_LAUNCHXL_UARTCOUNT;

void PinOutPutConfig(int pin);

int main(void)
{
    /* Call board init functions */
    char buffer[10];
    char enter[2]="  ";
    Board_initGeneral();
    Board_initADC();

    PinOutPutConfig(IOID_6);

    UART_Handle uart;
    UART_Params uartParams;

    UART_init();
    UART_Params_init(&uartParams);

    uartParams.readMode = UART_MODE_CALLBACK;
    uartParams.writeMode = UART_MODE_CALLBACK;
    uartParams.readTimeout = UART_WAIT_FOREVER;
    uartParams.writeTimeout = UART_WAIT_FOREVER;
    uartParams.readCallback = NULL;
    uartParams.writeCallback = NULL;
    uartParams.readReturnMode = UART_RETURN_NEWLINE;
    uartParams.readDataMode = UART_DATA_TEXT;
    uartParams.writeDataMode = UART_DATA_TEXT;
    uartParams.readEcho = UART_ECHO_ON;
    uartParams.baudRate = 115200;
    uartParams.dataLength = UART_LEN_8;
    uartParams.stopBits = UART_STOP_ONE;
    uartParams.parityType = UART_PAR_NONE;

    uart = UART_open(Board_UART0, &uartParams);

    if (uart == NULL) {
        while (1);
    }

    ADC_Handle   adc;
    ADC_Params   params;
    ADC_Params_init(&params);
    adc = ADC_open(CC1310_LAUNCHXL_ADC0, &params);
    if (adc == NULL) {
        while(1);
    }

   while(1)
   {
        ADC_convert(adc, &adcValue);
        //snprintf(buffer, "%d", adcValue);
        ltoa(adcValue, buffer);
        UART_write(uart,buffer,sizeof(buffer));
        UART_write(uart,enter,sizeof(enter));
        CPUdelay(2000000);

    }

}
void PinOutPutConfig(int pin)
{
    IOCIODrvStrengthSet(pin,IOC_CURRENT_2MA,IOC_STRENGTH_AUTO);
    IOCIOHystSet(pin,IOC_HYST_DISABLE);
    IOCIOSlewCtrlSet(pin,IOC_SLEW_DISABLE);
    IOCIOPortPullSet(pin,IOC_NO_IOPULL);
    IOCIOIntSet(pin,IOC_INT_DISABLE,IOC_NO_EDGE);
    IOCIOModeSet(pin,IOC_IOMODE_NORMAL);
    IOCIOShutdownSet(pin,IOC_NO_WAKE_UP);
    IOCIOInputSet(pin,IOC_INPUT_DISABLE);
    IOCPinTypeGpioOutput(pin);
}
