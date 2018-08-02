#include <xdc/std.h>
#include <xdc/runtime/System.h>
#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Clock.h>
#include <ti/drivers/PIN.h>
#include "Board.h"

void PinOutputConfig(int pin);
void Pin›nputConfig(int pin);

int main(void)
{
    uint32_t durum;
    int sayac=0;
    int mod;

    Board_initGeneral();

    PinOutputConfig(IOID_27);
    PinOutputConfig(IOID_28);
    PinOutputConfig(IOID_29);

    Pin›nputConfig(IOID_9);

    GPIO_writeDio(IOID_9,1);

    while(1)
    {
        durum=GPIO_readDio(IOID_9);
        CPUdelay(800000);
        if(durum==0)
        {
            sayac++;
            mod=sayac%2;
        }

        switch(mod)
        {
            case 0:
                CPUdelay(350000);
                GPIO_writeDio(IOID_27,1);
                GPIO_writeDio(IOID_28,1);
                GPIO_writeDio(IOID_29,1);
                break;
            case 1:
                CPUdelay(350000);
                GPIO_writeDio(IOID_27,0);
                GPIO_writeDio(IOID_28,0);
                GPIO_writeDio(IOID_29,0);
                break;

        }

    }
}

void Pin›nputConfig(int pin)
{
    IOCIODrvStrengthSet(pin,IOC_CURRENT_2MA,IOC_STRENGTH_AUTO);
    IOCIOModeSet(pin,IOC_IOMODE_NORMAL);
    IOCIOPortPullSet(pin,IOC_IOPULL_UP);
    IOCIOSlewCtrlSet(pin,IOC_SLEW_DISABLE);
    IOCIOHystSet(pin,IOC_HYST_DISABLE);
    IOCIOInputSet(pin,IOC_INPUT_ENABLE);
}
void PinOutputConfig(int pin)
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
