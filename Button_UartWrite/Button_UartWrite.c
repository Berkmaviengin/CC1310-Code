#include <xdc/std.h>
#include <xdc/runtime/System.h>
#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Clock.h>
#include <ti/drivers/UART.h>
#include <ti/drivers/uart/UARTCC26XX.h>
#include <ti/drivers/PIN.h>
#include <ti/drivers/GPIO.h>
#include <ti/drivers/Power.h>
#include <ti/drivers/power/PowerCC26XX.h>
#include "Board.h"


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

int main(void)
{
    const char  string[] = "Deneme\r\n";
    uint32_t durum;
    UART_Handle uart;
    UART_Params uartParams;

    Board_initGeneral(); //Board ayarlari

    /**************** input pin yapilandirmalari ******************/
    IOCIODrvStrengthSet(IOID_9,IOC_CURRENT_2MA,IOC_STRENGTH_AUTO);
    IOCIOModeSet(IOID_9,IOC_IOMODE_NORMAL);
    IOCIOPortPullSet(IOID_9,IOC_IOPULL_UP);
    IOCIOSlewCtrlSet(IOID_9,IOC_SLEW_DISABLE);
    IOCIOHystSet(IOID_9,IOC_HYST_DISABLE);
    IOCIOInputSet(IOID_9,IOC_INPUT_ENABLE);
    GPIO_writeDio(IOID_9,1);

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

    while(1)
    {
        durum=GPIO_readDio(IOID_9); // input pinini oku
        if(durum==0)// lojik 0 ise
        {
            UART_write(uart, string, sizeof(string));
            GPIO_writeDio(IOID_6,1);  //led yak
            CPUdelay(2000000);
        }
        else // lojik 1 ise
        {
            GPIO_writeDio(IOID_6,0); //led sondur
        }
    }
}
