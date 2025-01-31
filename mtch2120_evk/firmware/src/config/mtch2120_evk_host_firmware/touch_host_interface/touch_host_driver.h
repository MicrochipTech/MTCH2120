/*******************************************************************************
  MPLAB Harmony Touch Host Interface v1.1.0 Release

  Company:
	Microchip Technology Inc.

  File Name:
	touch_host_driver.h

  Summary:
	This header file provides prototypes and definitions for the application.

  Description:
	This header file provides function prototypes and data type definitions for
	the application.  Some of these are required by the system (such as the
	"APP_Initialize" and "APP_Tasks" prototypes) and some of them are only used
	internally by the application (such as the "APP_STATES" definition).  Both
	are defined here for convenience.
*******************************************************************************/

// DOM-IGNORE-BEGIN
/*******************************************************************************
 * Copyright (C) 2024 Microchip Technology Inc. and its subsidiaries.
 *
 * Subject to your compliance with these terms, you may use Microchip software
 * and any derivatives exclusively with Microchip products. It is your
 * responsibility to comply with third party license terms applicable to your
 * use of third party software (including open source software) that may
 * accompany Microchip software.
 *
 * THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
 * EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
 * WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
 * PARTICULAR PURPOSE.
 *
 * IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
 * INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
 * WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
 * BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
 * FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN
 * ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
 * THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
 *******************************************************************************/
// DOM-IGNORE-END

#ifndef TOUCH_HOST_DRIVER_H    /* Guard against multiple inclusion */
#define TOUCH_HOST_DRIVER_H


/* ************************************************************************** */
/* ************************************************************************** */
/* Section: Included Files                                                    */
/* ************************************************************************** */
/* ************************************************************************** */

/* This section lists the other files that are included in this file.
 */

/* TODO:  Include other files here if needed. */


/* Provide C++ Compatibility */
#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "definitions.h"
    
#define CIRCULAR_BUFFER_LEN (200u)

#if (CIRCULAR_BUFFER_LEN <= 255)
typedef uint8_t transferSize_t;
#else
typedef uint16_t transferSize_t;
#endif

typedef  void(*callbackTx_T)(void);
typedef  void(*callbackRx_T)(uint8_t callback);

#define SYS_TIME_RESOLUTION_MSEC 10u

    /* Provide C++ Compatibility */
#ifdef __cplusplus
}
#endif

#endif /* TOUCH_HOST_DRIVER_H */

/* *****************************************************************************
 End of File
 */
