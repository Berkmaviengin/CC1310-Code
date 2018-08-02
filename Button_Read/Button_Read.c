#include <xdc/std.h>
#include <xdc/runtime/System.h>
#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Clock.h>
#include <ti/drivers/PIN.h>
#include "Board.h"

int main(void)
{
    uint32_t durum; //durum kontrolu icin

    Board_initGeneral(); //Board ayarlari

    /**************** input pin yapilandirmalari ******************/
    IOCIODrvStrengthSet(IOID_9,IOC_CURRENT_2MA,IOC_STRENGTH_AUTO);
    IOCIOModeSet(IOID_9,IOC_IOMODE_NORMAL);
    IOCIOPortPullSet(IOID_9,IOC_IOPULL_UP);
    IOCIOSlewCtrlSet(IOID_9,IOC_SLEW_DISABLE);
    IOCIOHystSet(IOID_9,IOC_HYST_DISABLE);
    IOCIOInputSet(IOID_9,IOC_INPUT_ENABLE);

    /************** input cikisina lojik 1 verildi *****************/
    GPIO_writeDio(IOID_9,1);

    while(1)
    {
        durum=GPIO_readDio(IOID_9); // input pinini oku
        if(durum==0)// lojik 0 ise
        {
            GPIO_writeDio(IOID_6,1);  //led yak
        }
        else // lojik 1 ise
        {
            GPIO_writeDio(IOID_6,0); //led sondur
        }
    }
}
