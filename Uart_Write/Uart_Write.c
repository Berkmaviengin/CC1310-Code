#include <xdc/std.h>
#include <xdc/runtime/System.h>
#include <ti/sysbios/BIOS.h>
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
    Board_initGeneral();
    const char  string[] = "Deneme\r\n";
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
    while (1) {
        UART_write(uart, string, sizeof(string));
        CPUdelay(5000000);
    }


}
