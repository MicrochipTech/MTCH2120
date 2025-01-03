/* ************************************************************************** */
/** Descriptive File Name

  @Company
    Company Name

  @File Name
    filename.c

  @Summary
    Brief description of the file.

  @Description
    Describe the purpose of this file.
 */
/*----------------------------------------------------------------------------
 *     include files
 *----------------------------------------------------------------------------*/
#include <stddef.h>      // Defines NULL
#include <stdbool.h>     // Defines true
#include <stdlib.h>      // Defines EXIT_FAILURE
#include "definitions.h" // SYS function prototypes
// #include <string.h>
#include "led_driver.h"

void initLeds(void) {
    static uint8_t data[2] = {0u,0u};
        
    data[0] = MCP23017_IODIRA_ADDR;
    data[1] = 0x00;
	
	SERCOM2_I2C_Write(APP_MCP23017_DEVICE_ADDR, data, 2u);
    while(SERCOM2_I2C_IsBusy());

    data[0] = MCP23017_IODIRB_ADDR;
    SERCOM2_I2C_Write(APP_MCP23017_DEVICE_ADDR, data, 2u);
    while(SERCOM2_I2C_IsBusy());
    
    updateLeds(0,0);
}

void updateLeds(uint8_t porta, uint8_t portb) {
   
   static uint8_t data[2] = {0u,0u};
   
    data[0] = MCP23017_OLATA_ADDR;
	data[1] = porta;
    SERCOM2_I2C_Write(APP_MCP23017_DEVICE_ADDR, data, 2u);
    while(SERCOM2_I2C_IsBusy());
    
    data[0] = MCP23017_OLATB_ADDR;
	data[1] = portb;
    SERCOM2_I2C_Write(APP_MCP23017_DEVICE_ADDR, data, 2u);
    while(SERCOM2_I2C_IsBusy());
}