#include <xdc/std.h>
#include <xdc/runtime/System.h>
#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Clock.h>
#include <ti/drivers/PIN.h>
#include "Board.h"

void PinOutPutConfig(int pin);

int main(void)
{
    Board_initGeneral();
    PinOutPutConfig(IOID_27);
    PinOutPutConfig(IOID_28);
    PinOutPutConfig(IOID_29);

    while(1)
    {
        GPIO_writeDio(IOID_27, 1);
        GPIO_writeDio(IOID_28, 1);
        GPIO_writeDio(IOID_29, 1);
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

