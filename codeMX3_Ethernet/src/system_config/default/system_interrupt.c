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
#include "app_commands.h"
#include "UDP_app.h"

uint16_t AKDHNKJADHPsinus[128] = {
500,
655,
794,
905,
976,
1000,
976,
905,
794,
655,
500,
345,
206,
95,
24,
0,
24,
95,
206,
345,
500,
655,
794,
905,
976,
1000,
976,
905,
794,
655,
500,
345,
206,
95,
24,
0,
24,
95,
206,
345,
500,
655,
794,
905,
976,
1000,
976,
905,
794,
655,
500,
345,
206,
95,
24,
0,
24,
95,
206,
345,
500,
655,
794,
905,
976,
1000,
976,
905,
794,
655,
500,
345,
206,
95,
24,
0,
24,
95,
206,
345,
500,
655,
794,
905,
976,
1000,
976,
905,
794,
655,
500,
345,
206,
95,
24,
0,
24,
95,
206,
345,
500,
655,
794,
905,
976,
1000,
976,
905,
794,
655,
500,
345,
206,
95,
24,
0,
24,
95,
206,
345,
500,
655,
794,
905,
976,
1000,
976,
905
};

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
    ACL_ReadRawValues(accel_buffer);
    // Fonction_Lire_Distance(distance_read);
    flag_packet = false;
    //Cr�ation du packet
    
    
        UDP_Send_Buffer[2*compteur_interrupt] = (char)(AKDHNKJADHPsinus[compteur_interrupt]>>8);
        UDP_Send_Buffer[2*compteur_interrupt+1] = (char)AKDHNKJADHPsinus[compteur_interrupt];

    
    ACL_GetRegister(ACL_INT_SOURCE);
    accel_data_ready = true;
    PLIB_INT_SourceFlagClear(INT_ID_0, INT_SOURCE_EXTERNAL_4);
    compteur_interrupt++;
    //compteur_interrupt2++;
    //RGBLED_SetValue(a[compteur_interrupt],b[compteur_interrupt],c[compteur_interrupt]);
    JB1Toggle();
    if (compteur_interrupt > 127)
    {
        compteur_interrupt = 0;
        compteur_packet++;
        sendTask();
        
    }
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
