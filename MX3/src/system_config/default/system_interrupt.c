/*******************************************************************************
 System Interrupts File

  File Name:
    system_interrupt.c

  Summary:
    Raw ISR definitions.

  Description:
    This file contains a definitions of the raw ISRs required to support the
    interrupt sub-system.

  Summary:
    This file contains source code for the interrupt vector functions in the
    system.

  Description:
    This file contains source code for the interrupt vector functions in the
    system.  It implements the system and part specific vector "stub" functions
    from which the individual "Tasks" functions are called for any modules
    executing interrupt-driven in the MPLAB Harmony system.

  Remarks:
    This file requires access to the systemObjects global data structure that
    contains the object handles to all MPLAB Harmony module objects executing
    interrupt-driven in the system.  These handles are passed into the individual
    module "Tasks" functions to identify the instance of the module to maintain.
 *******************************************************************************/

// DOM-IGNORE-BEGIN
/*******************************************************************************
Copyright (c) 2011-2014 released Microchip Technology Inc.  All rights reserved.

Microchip licenses to you the right to use, modify, copy and distribute
Software only when embedded on a Microchip microcontroller or digital signal
controller that is integrated into your product or third party product
(pursuant to the sublicense terms in the accompanying license agreement).

You should refer to the license agreement accompanying this Software for
additional information regarding your rights and obligations.

SOFTWARE AND DOCUMENTATION ARE PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND,
EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION, ANY WARRANTY OF
MERCHANTABILITY, TITLE, NON-INFRINGEMENT AND FITNESS FOR A PARTICULAR PURPOSE.
IN NO EVENT SHALL MICROCHIP OR ITS LICENSORS BE LIABLE OR OBLIGATED UNDER
CONTRACT, NEGLIGENCE, STRICT LIABILITY, CONTRIBUTION, BREACH OF WARRANTY, OR
OTHER LEGAL EQUITABLE THEORY ANY DIRECT OR INDIRECT DAMAGES OR EXPENSES
INCLUDING BUT NOT LIMITED TO ANY INCIDENTAL, SPECIAL, INDIRECT, PUNITIVE OR
CONSEQUENTIAL DAMAGES, LOST PROFITS OR LOST DATA, COST OF PROCUREMENT OF
SUBSTITUTE GOODS, TECHNOLOGY, SERVICES, OR ANY CLAIMS BY THIRD PARTIES
(INCLUDING BUT NOT LIMITED TO ANY DEFENSE THEREOF), OR OTHER SIMILAR COSTS.
 *******************************************************************************/
// DOM-IGNORE-END

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include "system/common/sys_common.h"
#include "main.h"
#include "system_definitions.h"
#include "accel.h"
#include "ctrl.h"
#include "swt.h"

// *****************************************************************************
// *****************************************************************************
// Section: System Interrupt Vector Functions
// *****************************************************************************
// *****************************************************************************
void __ISR(_UART_4_VECTOR, ipl1AUTO) _IntHandlerDrvUsartInstance0(void)
{
  DRV_USART_TasksTransmit(sysObj.drvUsart0);
  DRV_USART_TasksError(sysObj.drvUsart0);
  DRV_USART_TasksReceive(sysObj.drvUsart0);
}

void __ISR(_EXTERNAL_4_VECTOR, IPL1AUTO) _IntHandlerExternalInterruptInstance0(void)
{
  int accelX, accelY, accelZ;
  int val;
  int dir;
  static int last_dir = 0;

  ACL_ReadRawValues(accel_buffer);
  accelX = ((signed int)accel_buffer[0] << 24) >> 20 | accel_buffer[1] >> 4; // VR
  accelY = ((signed int)accel_buffer[2] << 24) >> 20 | accel_buffer[3] >> 4; // VR
  accelZ = ((signed int)accel_buffer[4] << 24) >> 20 | accel_buffer[5] >> 4; // VR

  dir = SWT_GetValue(6);
  if(last_dir != dir)
  {
    if (dir == 1)
      LCD_WriteStringAtPos("H", 1, 15);
    else
      LCD_WriteStringAtPos("B", 1, 15);
  }
  last_dir = dir;

  if (dir == 1 && accelX > 700 && accelY < 700)
    val = 1;
  else if (dir == 0 && accelX < -700 && accelY < 700)
    val = 1;
  else
    val = 0;

  if (val == 1)
  {
    LED_SetValue(4, 1);
    ctrl.btns.bits.star_power = 1;
  }
  else
  {
    LED_SetValue(4, 0);
    ctrl.btns.bits.star_power = 0;
  }

  ACL_GetRegister(ACL_INT_SOURCE);
  accel_data_ready = true;
  PLIB_INT_SourceFlagClear(INT_ID_0, INT_SOURCE_EXTERNAL_4);
}

void __ISR(_TIMER_1_VECTOR, ipl1AUTO) IntHandlerDrvTmrInstance0(void)
{
  DRV_TMR_Tasks(sysObj.drvTmr0);
}

void __ISR(_SPI_2_VECTOR, ipl1AUTO) _IntHandlerSPIInstance0(void)
{
  DRV_SPI_Tasks(sysObj.spiObjectIdx0);
}
/*******************************************************************************
 End of File
*/
