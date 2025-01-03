/*******************************************************************************
  Main Source File

  Company:
    Microchip Technology Inc.

  File Name:
    main.c

  Summary:
    This file contains the "main" function for a project.

  Description:
    This file contains the "main" function for a project.  The
    "main" function calls the "SYS_Initialize" function to initialize the state
    machines of all modules in the system
 *******************************************************************************/

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include <stddef.h>                     // Defines NULL
#include <stdbool.h>                    // Defines true
#include <stdlib.h>                     // Defines EXIT_FAILURE
#include <string.h>                     // Defines memcpy and memcmp
#include "definitions.h"                // SYS function prototypes

#include "config/mtch2120_evk_host_firmware/touch_host_interface/touchTune.h"
#include "config/mtch2120_evk_host_firmware/touch_host_interface/mtch2120.h"
#include "config/mtch2120_evk_host_firmware/touch_host_interface/mtch2120_host_example.h"
#include "mtch2120/TimerHandler/Timer_Handler.h"
#include "led_driver.h"

#include "app.h"

#define QUERY_TIME              35u
#define SEND_DATA_TO_DV_TIME    50u
#define QUERY_SLAVE_ADDRESS     1000u
#define BUTTON11_BLINK          1000u
#define COMM_STATUS_LED_PERIOD  500u

#define DEFAULT_CONFIG_CRC      0x1040u

#define QTM_KEY_STATE_DETECT    0x85u

// Touch module inputs
#define INT                     (1u << 0u)      // INTERRUPT    [PA0]
#define CLIENT_RESET            (1u << 1u)      // CLIENT RESET [PA1]

// status LED's
#define DEFAULT_CONFIG          (1u << 10u)     // INT_HOST_STATUS_1 [PA10] / D1
#define STATUS_2                (1u << 11u)     // INT_HOST_STATUS_2 [PA11] / D3
#define STATUS_3                (1u << 14u)     // INT_HOST_STATUS_3 [PA14] / D5
#define STATUS_4                (1u << 15u)     // INT_HOST_STATUS_4 [PA15] / D6

static uint8_t previousSensorState[DEF_NUM_SENSORS];
static uint8_t portaLed = 0u, portbLed = 0u;
static uint8_t blinkValue = 0u;

static volatile uint8_t updateTouchMemoryFlag = 0u;
static volatile uint8_t updateblinkCommStatusLedFlag = 0u;
static void updateTouchMemoryHandler(void);
static void blinkCommStatusLedHandler(void);

static void updateTouchMemory(void);
static void updateStatusLeds(void);
static void blinkButton11Led(void);
static void hostVersionLedStatus(void);
static void blinkCommStatusLed(void);
static void initStatusPins(void);
static void resetClientDevice(void);

static void scanAndCopyDataFromUSB(void);

extern uint8_t read_buffer[UART_RX_BUF_LENGTH];
extern volatile uint8_t read_buf_write_ptr;
extern volatile uint8_t uart_tx_in_progress;
extern void touchUartTxComplete(uintptr_t touchUart);

extern bool triggerUSBStatusLed;
// *****************************************************************************
// *****************************************************************************
// Section: Main Entry Point
// *****************************************************************************
// *****************************************************************************
int main ( void )
{
    /* Initialize all modules */
    SYS_Initialize ( NULL );
      
    initStatusPins();

    // resetting 2120 
    resetClientDevice();
    
    hostVersionLedStatus();
    
    // data visualizer initialization
    touch_init_example();
    
    // initializing button led driver
    initLeds();
    
    timer_setHandler(SEND_DATA_TO_DV_TIME, touchTuneNewDataAvailable);
    
    // below function will query the touch module 4 times per sec.
    timer_setHandler(QUERY_TIME, updateTouchMemoryHandler);
    
    timer_setHandler(BUTTON11_BLINK, blinkButton11Led);
    
    timer_setHandler(COMM_STATUS_LED_PERIOD, blinkCommStatusLedHandler);
    
    mtch2120_updateShieldStatus();
    
    while ( true )
    {
        // 2120 will pull the INT pin to low until host read the touch status.
        if((PORT_REGS->GROUP[0u].PORT_IN & (uint32_t)INT) == (uint32_t)0u)
        {
            mtch2120_getAllSensorState();
            updateStatusLeds();
        }
            
        timer_run();
  
        scanAndCopyDataFromUSB();
        
        if(updateTouchMemoryFlag == 1u)
        {
            updateTouchMemoryFlag = 0;
            updateTouchMemory();
        }
        
        if(updateblinkCommStatusLedFlag == 1u)
        {
            updateblinkCommStatusLedFlag = 0u;
            blinkCommStatusLed();
        }
        
        // run touch tune module
        touch_mainloop_example();
        
        /* Maintain state machines of all polled MPLAB Harmony modules. */
        SYS_Tasks ( );
        
        if(uart_tx_in_progress == 1u)
        {
            touchUartTxComplete(0u);
        }
    }

    /* Execution should not come here during normal operation */
    return ( EXIT_FAILURE );
}

static void updateTouchMemoryHandler(void) 
{
    updateTouchMemoryFlag = 1u;
}

static void blinkCommStatusLedHandler(void)
{
    updateblinkCommStatusLedFlag = 1u;
}

static void updateStatusLeds(void)
{
    if(memcmp(previousSensorState, mtch2120_sensorState, DEF_NUM_SENSORS) != 0u)
    {
        (void)memcpy(previousSensorState, mtch2120_sensorState, DEF_NUM_SENSORS);
        for(uint8_t i = 0u; i < DEF_NUM_SENSORS; i++)
        {
            if(mtch2120_sensorState[i] == QTM_KEY_STATE_DETECT)
            {
                if(i < 8u)
                {
                    portaLed |= (1u << i);
                }
                else
                {
                    portbLed |= (1u << (i - 8u));
                }
            }
            else
            {
                if(i < 8u)
                {
                    portaLed &= ~(1u << i);
                }
                else
                {
                    portbLed &= ~(1u << (i - 8u));
                }
            }
        }
        
        // make DS indication consistent when touch is detected
        portbLed |= (blinkValue << 3u);
        
        updateLeds(portaLed, portbLed);
    }
}

static void blinkCommStatusLed(void)
{
    static uint8_t led = 0u;
    static uint8_t i2c_timeout = 0u;
    static uint8_t usb_timeout = 0u;
    
    if(triggerI2CStatusLed)
    {
        triggerI2CStatusLed = false;
        
        if(led == 0u)
        {
            // turn on D3 led to indicate i2c transaction in progress
            PORT_REGS->GROUP[0u].PORT_OUT &= ~STATUS_2;
        }
        else
        {
            // turn on D3 led to indicate i2c transaction in progress
            PORT_REGS->GROUP[0u].PORT_OUT |= STATUS_2;
        }          
    }
    else
    {
        i2c_timeout++;
        if(i2c_timeout > 4) 
        {
            PORT_REGS->GROUP[0u].PORT_OUT |= STATUS_2;
        }
    }
    if(triggerUSBStatusLed)
    {
        triggerUSBStatusLed = false;
        
        if(led == 0u)
        {
            // turn on D3 led to indicate i2c transaction in progress
            PORT_REGS->GROUP[0u].PORT_OUT &= ~STATUS_3;
        }
        else
        {
            // turn on D3 led to indicate i2c transaction in progress
            PORT_REGS->GROUP[0u].PORT_OUT |= STATUS_3;
        }
    }
    else
    {
        usb_timeout++;
        if(usb_timeout > 4)
        {
            PORT_REGS->GROUP[0u].PORT_OUT |= STATUS_3;
        }
    }
    
    if(led)
    {
        led = 0u;
    }
    else
    {
        led = 1u;
    }
}

static void blinkButton11Led(void)
{
    // blink button 11 led to indicate that DS is enabled.
    if(mtch2120_isDS_Activated)
    {
        if(blinkValue == 0u)
        {
            portbLed |= (1u << 3u);
            blinkValue = 1u;
        }
        else
        {
            portbLed &= ~(1u << 3u);
            blinkValue = 0u;
        }
        
        updateLeds(portaLed, portbLed);
    }
}

static void updateTouchMemory(void)
{
    if(isStopToReadDebugData() == 0u)
    {
        mtch2120_touchDeviceReadDebugData();

        if(mtch2120_deviceInformation.crc == DEFAULT_CONFIG_CRC)
        {
            PORT_REGS->GROUP[0].PORT_OUT &= ~DEFAULT_CONFIG;    // ON D1 by pulling gpio pin to low
        }
        else
        {
            PORT_REGS->GROUP[0].PORT_OUT |= DEFAULT_CONFIG;     // OFF D1 by pulling gpio pin to high
        }
    }
    
    // turn on D7 led if CRC corrupted...
    // CRC validation will happen on every save and load
    if((mtch2120_Status.deviceStatus.bits.loadFailure == 1u) || (mtch2120_Status.deviceStatus.bits.saveFailure == 1u))
    {
        // on d6 led to intimate load or save failure
        PORT_REGS->GROUP[0u].PORT_OUT &= ~STATUS_4;
    }
    else
    {
        // off d6 led to intimate no failure
        PORT_REGS->GROUP[0u].PORT_OUT |= STATUS_4;
    }
}

static void resetClientDevice(void)
{
    // reset 2120 device - appox 80msec
    PORT_REGS->GROUP[0u].PORT_OUTCLR = CLIENT_RESET;      // Reset the client device
    
    /* delay need to be fine tuned with reset circuit */
    for(uint16_t cnt1 = 0u; cnt1 < 10u; cnt1++)
    {
        for(uint16_t cnt = 0u; cnt < 65500u; cnt++)
        {
            asm("nop");
        }
    }

    // release reset - appox 800msec
    PORT_REGS->GROUP[0u].PORT_OUTSET = CLIENT_RESET;
    
    /* delay need to be fine tuned with reset circuit */
    for(uint16_t cnt1 = 0u; cnt1 < 100u; cnt1++)
    {
        for(uint16_t cnt = 0u; cnt < 65500u; cnt++)
        {
            asm("nop");
        }
    }
}

static void initStatusPins(void)
{
    // configuring translator direction as B->A for reading HB and INT status.
    PORT_REGS->GROUP[0u].PORT_DIRSET = CLIENT_RESET;      // CLIENT RESET
    PORT_REGS->GROUP[0u].PORT_DIRSET = DEFAULT_CONFIG;    // DEFAULT CONFIGURATION LED
    
    // configuring INT as input
    PORT_REGS->GROUP[0u].PORT_DIRCLR    = INT;            // INT
    PORT_REGS->GROUP[0u].PORT_PINCFG[0] = 0x2U;           // ENABLING INPUT(PA0)
    
    // configuring STATUS_2,3,4 pin as output
    PORT_REGS->GROUP[0u].PORT_DIRSET = STATUS_2;
    PORT_REGS->GROUP[0u].PORT_DIRSET = STATUS_3;
    PORT_REGS->GROUP[0u].PORT_DIRSET = STATUS_4;
    
    PORT_REGS->GROUP[0u].PORT_OUTSET =  DEFAULT_CONFIG;  // TURNING off D1
    PORT_REGS->GROUP[0u].PORT_OUTSET =  STATUS_2;        // TURNING off D3
    PORT_REGS->GROUP[0u].PORT_OUTSET =  STATUS_3;        // TURNING off D5
    PORT_REGS->GROUP[0u].PORT_OUTSET =  STATUS_4;        // TURNING off D6
}

static void scanAndCopyDataFromUSB(void)
{
    if(appData.isReadComplete)
    {
        readFrame();
        for(uint8_t i = 0u; i < appData.numBytesRead; i++)
        {
            read_buffer[read_buf_write_ptr] = appData.cdcReadBuffer[i];
            read_buf_write_ptr++;
            if (read_buf_write_ptr == UART_RX_BUF_LENGTH)
            {
                read_buf_write_ptr = 0;
            }
        }
    }
}

static void hostVersionLedStatus(void)
{
    static uint8_t status = 0u;
    
    /********************************************************************
     * Note: 
     * version 1.1 led status.
     *  2 times on 2 times off; therefore i is compared with 4.
    ********************************************************************/
    
    for(uint8_t i = 0u; i < 4u; i++)
    {
        switch(status)
        {
            case 0u:
                PORT_REGS->GROUP[0u].PORT_OUT &= ~DEFAULT_CONFIG;
                PORT_REGS->GROUP[0u].PORT_OUT &= ~STATUS_2;
                PORT_REGS->GROUP[0u].PORT_OUT &= ~STATUS_3;
                PORT_REGS->GROUP[0u].PORT_OUT &= ~STATUS_4;
                status++;
                break;
            case 1u:
                PORT_REGS->GROUP[0u].PORT_OUT |= DEFAULT_CONFIG;
                PORT_REGS->GROUP[0u].PORT_OUT |= STATUS_2;
                PORT_REGS->GROUP[0u].PORT_OUT |= STATUS_3;
                PORT_REGS->GROUP[0u].PORT_OUT |= STATUS_4;
                status = 0u;
                break;
            default:
                status = 0u;
                break;
        }
        
        // delay 
        for(unsigned int i = 0u; i < 1000000u; i++);
        for(unsigned int i = 0u; i < 1000000u; i++);
        for(unsigned int i = 0u; i < 1000000u; i++);
    }
}

/*******************************************************************************
 End of File
*/
