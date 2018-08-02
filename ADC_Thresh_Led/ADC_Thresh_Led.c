/*
 * Copyright (c) 2016, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/*
 *  ======== adcsinglechannel.c ========
 */
/* XDCtools Header files */
#include <xdc/std.h>
#include <xdc/runtime/System.h>

/* BIOS Header files */
#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Task.h>

/* Driver Header files */
#include <ti/drivers/ADC.h>
#if defined(CC2650DK_7ID) || defined(CC1310DK_7XD)
#include <ti/drivers/PIN.h>
#endif

/* Example/Board Header files */
#include "Board.h"

/* ADC conversion result variables */
uint16_t adcValue;

void PinOutPutConfig(int pin);

/*
 *  ======== main ========
 */
int main(void)
{
    /* Call board init functions */
    Board_initGeneral();
    Board_initADC();

    PinOutPutConfig(IOID_6);

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

        if (adcValue <= 1500)
        {
            GPIO_writeDio(IOID_6,1);
        }
        else
        {
            GPIO_writeDio(IOID_6,0);
        }
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
