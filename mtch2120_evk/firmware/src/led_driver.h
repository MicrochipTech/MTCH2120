/* ************************************************************************** */
/** Descriptive File Name

  @Company
    Microchip Technology Inc.

  @File Name
    led_driver.h

  @Summary
    Brief description of the file.

  @Description
    DConfiguration macros for LED driver

*******************************************************************************/

/*******************************************************************************
Copyright (c) 2021 released Microchip Technology Inc.  All rights reserved.

Microchip licenses to you the right to use, modify, copy and distribute
Software only when embedded on a Microchip microcontroller or digital signal
controller that is integrated into your product or third party product
(pursuant to the sublicense terms in the accompanying license agreement).

You should refer to the license agreement accompanying this Software for
additional information regarding your rights and obligations.

SOFTWARE AND DOCUMENTATION ARE PROVIDED AS IS  WITHOUT  WARRANTY  OF  ANY  KIND,
EITHER EXPRESS  OR  IMPLIED,  INCLUDING  WITHOUT  LIMITATION,  ANY  WARRANTY  OF
MERCHANTABILITY, TITLE, NON-INFRINGEMENT AND FITNESS FOR A  PARTICULAR  PURPOSE.
IN NO EVENT SHALL MICROCHIP OR  ITS  LICENSORS  BE  LIABLE  OR  OBLIGATED  UNDER
CONTRACT, NEGLIGENCE, STRICT LIABILITY, CONTRIBUTION,  BREACH  OF  WARRANTY,  OR
OTHER LEGAL  EQUITABLE  THEORY  ANY  DIRECT  OR  INDIRECT  DAMAGES  OR  EXPENSES
INCLUDING BUT NOT LIMITED TO ANY  INCIDENTAL,  SPECIAL,  INDIRECT,  PUNITIVE  OR
CONSEQUENTIAL DAMAGES, LOST  PROFITS  OR  LOST  DATA,  COST  OF  PROCUREMENT  OF
SUBSTITUTE  GOODS,  TECHNOLOGY,  SERVICES,  OR  ANY  CLAIMS  BY  THIRD   PARTIES
(INCLUDING BUT NOT LIMITED TO ANY DEFENSE  THEREOF),  OR  OTHER  SIMILAR  COSTS.
*******************************************************************************/
#ifndef LED_DRIVER_H
#define LED_DRIVER_H
#include "device.h"


// *****************************************************************************
// Section: Macros & definitions 
// *****************************************************************************
#define APP_MCP23017_DEVICE_ADDR            0x0027
#define APP_TRANSMIT_DATA_LENGTH            2
#define APP_ACK_DATA_LENGTH                 1
#define MCP23017_IODIRA_ADDR                0x00
#define MCP23017_IODIRB_ADDR                0x01
#define MCP23017_OLATA_ADDR                 0x14
#define MCP23017_OLATB_ADDR                 0x15

void updateLeds(uint8_t porta, uint8_t portb);
void initLeds(void);

#endif /* LED_DRIVER.H */