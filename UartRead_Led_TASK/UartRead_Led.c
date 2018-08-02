#include <xdc/std.h>
#include <xdc/runtime/System.h>
#include <ti/sysbios/BIOS.h>
#include <ti/drivers/UART.h>
#include <ti/drivers/uart/UARTCC26XX.h>
#include <ti/drivers/PIN.h>
#include <ti/drivers/GPIO.h>
#include <ti/drivers/Power.h>
#include <ti/drivers/power/PowerCC26XX.h>
#include <xdc/runtime/System.h>
#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Task.h>
#include <ti/drivers/PIN.h>
#include <ti/drivers/UART.h>
#include "Board.h"
#include <stdint.h>
#include <xdc/std.h>

#define TASKSTACKSIZE     768
Task_Struct task0Struct;
Char task0Stack[TASKSTACKSIZE];

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

void UartLed(UArg arg0, UArg arg1)
{
    char input;
    const char cevap1[]="\r\nKirmizi Led Acik\r\n";
    const char cevap2[]="\r\nKirmizi Led Kapali\r\n";
    const char cevap3[]="\r\nYesil Led Acik\r\n";
    const char cevap4[]="\r\nYesil Led Kapali\r\n";
    UART_Handle uart;
    UART_Params uartParams;
    UART_Params_init(&uartParams);
    /* Uart Ayarlari */
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
    /* Uart Baslatilir */
    uart = UART_open(Board_UART0, &uartParams);

    if (uart == NULL)
    {
        System_abort("Uart Duzgun Baslatilamadi !");
    }

    while (1)
    {
        UART_read(uart, &input,1);

        if(input=='1')
        {
            GPIO_writeDio(IOID_6,1);
            UART_write(uart, cevap1, sizeof(cevap1));
        }
        if(input=='2')
        {
            GPIO_writeDio(IOID_6,0);
            UART_write(uart, cevap2, sizeof(cevap2));
        }
        if(input=='3')
        {
            GPIO_writeDio(IOID_7,1);
            UART_write(uart, cevap3, sizeof(cevap3));
        }
        if(input=='4')
        {
            GPIO_writeDio(IOID_7,0);
            UART_write(uart, cevap4, sizeof(cevap4));
        }
    }
}
int main(void)
{
    Task_Params taskParams;
    /* Board ayarlari */
    Board_initGeneral();
    Board_initUART();
    /* Pinler Output Ayarlanir */
    PinOutPutConfig(IOID_6);
    PinOutPutConfig(IOID_7);
    /* Task Acilir */
    Task_Params_init(&taskParams);
    taskParams.stackSize = TASKSTACKSIZE;
    taskParams.stack = &task0Stack;
    Task_construct(&task0Struct, (Task_FuncPtr)UartLed, &taskParams, NULL);
    IOCPinTypeGpioOutput(IOID_6);
    IOCPinTypeGpioOutput(IOID_7);
    /* Bios Baslatilir*/
    BIOS_start();
    return (0);
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
