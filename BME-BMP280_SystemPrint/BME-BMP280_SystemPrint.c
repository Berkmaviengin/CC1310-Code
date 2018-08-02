#include <string.h>
#include <stdio.h>
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
#include "bme280.h"

static char ubuf[256];
#define TASKSTACKSIZE     768
Task_Struct task0Struct;
Char task0Stack[TASKSTACKSIZE];

void bme280_Example_Task(UArg arg0, UArg arg1)
{
    I2C_Params i2cParam;
    I2C_Handle i2c;

    /* Open I2C bus driver */
    I2C_Params_init(&i2cParam);
    i2cParam.transferMode = I2C_MODE_BLOCKING;
    i2cParam.transferCallbackFxn = NULL;
    i2cParam.bitRate = I2C_400kHz;
    i2c = I2C_open(0, &i2cParam);  // Open I2C instance #0 with 400KHz bus speed
    if (i2c == NULL) {
        System_abort("I2C0: Failure opening port\r\n");
    }

    System_printf("Successfully initialized I2C driver.\r\n");
    System_flush();

    // Init the BME280 API
    BME280_init(i2c, BOSCH_SENSORTEC_BME280_I2CSLAVE_DEFAULT);
    if (!BME280_open()) {
        System_printf("ERROR opening BME280_open()\r\n");
        System_flush();
    } else {
        System_printf("BME280 opened; polling every 500ms\r\n");
        System_flush();
    }

    while(1) {

        // Read & interpret results, spitting to CIO console
        BME280_RawData *rd = BME280_read();
        Int32 tempC = BME280_compensated_Temperature(rd);
        sprintf(ubuf, "Temp: %d C (%d F), humidity: %d%%%%, Pressure: %u hPa\r\n", \
                        tempC / 100,
                        ((tempC * 9) / 5) / 100 + 32,
                        BME280_compensated_Humidity(rd) / 1024,
                        (BME280_compensated_Pressure(rd) / 256) / 1000);
        System_printf(ubuf);
        System_flush();

        // Wait 500ms and poll again
        Task_sleep(500);

    }
}

int main(void)
{
    Task_Params taskParams;
    /* Board ayarlari */
    Board_initGeneral();

    Task_Params_init(&taskParams);
    taskParams.stackSize = TASKSTACKSIZE;
    taskParams.stack = &task0Stack;
    Task_construct(&task0Struct, (Task_FuncPtr)bme280_Example_Task, &taskParams, NULL);
    /* Bios Baslatilir*/
    BIOS_start();
    return (0);
}

