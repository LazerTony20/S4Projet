/*******************************************************************************
  MPLAB Harmony Application Source File

  Company:
    Microchip Technology Inc.

  File Name:
    main.c

  Summary:
    This file contains the source code for the MPLAB Harmony application.

  Description:
    This file contains the source code for the MPLAB Harmony application.  It
    implements the logic of the application's state machine and it may call
    API routines of other MPLAB Harmony modules in the system, such as drivers,
    system services, and middleware.  However, it does not call any of the
    system interfaces (such as the "Initialize" and "Tasks" functions) of any of
    the modules in the system or make any assumptions about when those functions
    are called.  That is the responsibility of the configuration-specific system
    files.
 *******************************************************************************/

// DOM-IGNORE-BEGIN
/*******************************************************************************
Copyright (c) 2013-2014 released Microchip Technology Inc.  All rights reserved.

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

#include "main.h"
#include "system_config.h"
#include "system/common/sys_module.h" // SYS function prototypes
#include "driver/spi/src/dynamic/drv_spi_internal.h"
#include "UDP_app.h"
#include "led.h"
#include "ssd.h"
#include "accel.h"
#include "lcd.h"
#include "dst.h"
#include "pmods.h"
#include "ctrl.h"
#include "btn.h"
#include "swt.h"
#include "app_commands.h"
// *****************************************************************************
// *****************************************************************************
// Section: Global Data Definitions
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
/* Application Data

  Summary:
    Holds application data

  Description:
    This structure holds the application's data.

  Remarks:
    This structure should be initialized by the TCPIP_Initialize function.

    Application strings and buffers are be defined outside this structure.
 */

MAIN_DATA mainData;

void TMR3_Init(void);

// *****************************************************************************
// *****************************************************************************
// Section: Application Callback Functions
// *****************************************************************************
// *****************************************************************************

/* TODO:  Add any necessary callback functions.
 */

// *****************************************************************************
// *****************************************************************************
// Section: Application Local Functions
// *****************************************************************************
// *****************************************************************************

/* Application's LED Task Function */
static unsigned long int counter = 0;
static void LedTask(void)
{
  if (counter++ == 20000)
  {
    LED_ToggleValue(1);
    counter = 0;
  }
}

static bool sw0_old;
void ManageSwitches()
{
  bool sw0_new = SWITCH0StateGet();
  if ((sw0_new != sw0_old) && sw0_new)
  {
    // strcpy(UDP_Send_Buffer, "Bonjour S4\n\r");
    // UDP_bytes_to_send = strlen(UDP_Send_Buffer);
    // UDP_Send_Packet = true;
  }

  sw0_old = sw0_new;
}

// *****************************************************************************
// *****************************************************************************
// Section: Application Initialization and State Machine Functions
// *****************************************************************************
// *****************************************************************************

/*******************************************************************************
  Function:
    void MAIN_Initialize ( void )

  Remarks:
    See prototype in main.h.
 */

void MAIN_Initialize(void)
{
  /* Place the App state machine in its initial state. */
  mainData.state = MAIN_STATE_INIT;

  mainData.handleUSART0 = DRV_HANDLE_INVALID;

  UDP_Initialize();
  LCD_Init();
  I2C_Init(400000);
  ACL_Init();
  SSD_Init();
  TMR3_Init();

  PMODS_InitPin(1, 2, 1, 1, 0);
  PMODS_InitPin(1, 3, 1, 1, 0);
  PMODS_InitPin(1, 4, 1, 1, 0);
  PMODS_InitPin(1, 7, 1, 1, 0);
  PMODS_InitPin(1, 8, 1, 1, 0);
  PMODS_InitPin(1, 9, 1, 1, 0);
  PMODS_InitPin(1, 10, 1, 1, 0);
  PMODS_InitPin(0, 9, 1, 1, 0); // start
  PMODS_InitPin(0, 8, 1, 1, 0);
  // DST_Init();
  CTRL_Init();
}

/******************************************************************************
  Function:
    void MAIN_Tasks ( void )

  Remarks:
    See prototype in main.h.
 */

void MAIN_Tasks(void)
{

  /* Check the application's current state. */
  switch (mainData.state)
  {
    /* Application's initial state. */
  case MAIN_STATE_INIT:
  {
    bool appInitialized = true;
    SYS_CONSOLE_MESSAGE("Init\r\n");

    if (mainData.handleUSART0 == DRV_HANDLE_INVALID)
    {
      mainData.handleUSART0 = DRV_USART_Open(MAIN_DRV_USART, DRV_IO_INTENT_READWRITE | DRV_IO_INTENT_NONBLOCKING);
      appInitialized &= (DRV_HANDLE_INVALID != mainData.handleUSART0);
    }

    if (appInitialized)
    {
      mainData.state = MAIN_STATE_SERVICE_TASKS;
    }
    break;
  }

  case MAIN_STATE_SERVICE_TASKS:
  {
    LedTask();
    accel_tasks();
    UDP_Tasks();
    ManageSwitches();

    LED0Toggle();
    break;
  }

    /* The default state should never be executed. */
  default:
  {
    /* TODO: Handle error in application's state machine. */
    break;
  }
  }
}

int main(void)
{

  SYS_Initialize(NULL);
  MAIN_Initialize();
  SYS_INT_Enable();
  // SSD_WriteDigitsGrouped(0xFA9B, 0x1);

  while (1)
  {
    SYS_Tasks();
    MAIN_Tasks();
  };

  return 0;
}

void TMR3_Init(void)
{
  PR3 = (int)(((float)(0.05 * PB_FRQ) / 256) + 0.5); // set period register, generates one interrupt every 50 ms                     //             set period register, generates one interrupt every 300 us
  TMR3 = 0;                                          //    initialize count to 0
  T3CONbits.TCKPS = 7;                               //    1:256 prescaler value
  T3CONbits.TGATE = 0;                               //    not gated input (the default)
  T3CONbits.TCS = 0;                                 //    PCBLK input (the default)
  IPC3bits.T3IP = 1;                                 //    INT step 4: priority
  IPC3bits.T3IS = 0;                                 //    subpriority
  IFS0bits.T3IF = 0;                                 //    clear interrupt flag
  IEC0bits.T3IE = 1;                                 //    enable interrupt
  T3CONbits.ON = 1;                                  //    turn on Timer3
}

void __ISR(_TIMER_3_VECTOR, IPL1AUTO) Timer3ISR(void)
{
  static unsigned int last_distance;
  // unsigned int distance = DST_Get();
  unsigned int distance = 10;

  if (distance < 44 || (distance > 300 && last_distance < 100))
    distance = 44;
  else if (distance > 300)
    distance = 300;
  last_distance = distance;

  if (distance > 172)
  {
    ctrl.joystick_right_x = distance - 172;
    ctrl.joystick_right_x = (ctrl.joystick_right_x << 8) * (-1);
  }
  else
  {
    ctrl.joystick_right_x = 172 - distance;
    ctrl.joystick_right_x = (ctrl.joystick_right_x << 8) + (-1);
  }

  // Menu mode
  if (SWT_GetValue(7) == 1)
  {
    ctrl.btns.bits.dpad_up = BTN_GetValue('R');
    ctrl.btns.bits.dpad_down = BTN_GetValue('L');
    LCD_WriteStringAtPos("Mode Menu", 1, 0);
  }
  // Game mode
  else
  {
    ctrl.btns.bits.dpad_up = !PMODS_GetValue(1, 9);
    ctrl.btns.bits.dpad_down = !PMODS_GetValue(1, 10);
    LCD_WriteStringAtPos("Mode Jeu ", 1, 0);
    
  }

  // Get button values
  ctrl.btns.bits.green = !PMODS_GetValue(1, 2);
  ctrl.btns.bits.red = !PMODS_GetValue(1, 3);
  ctrl.btns.bits.yellow = !PMODS_GetValue(1, 4);
  ctrl.btns.bits.blue = !PMODS_GetValue(1, 7);
  ctrl.btns.bits.orange = !PMODS_GetValue(1, 8);
  ctrl.btns.bits.start = !PMODS_GetValue(0, 9);
  ctrl.btns.bits.white = !PMODS_GetValue(0, 8); // BACK

  CTRL_Refresh();

  // distance = DST_Get();
  LCD_WriteIntAtPos(distance, 6, 0, 0, 0);
  IFS0bits.T3IF = 0; // clear interrupt flag
}

/*******************************************************************************
 End of File
 */
