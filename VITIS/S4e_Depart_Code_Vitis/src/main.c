/*
 *
 * @file main.c
 * @brief Main routine
 *
 * @section License
 *
 * Copyright (C) 2010-2018 Oryx Embedded SARL. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 *
 * @author Oryx Embedded SARL (www.oryx-embedded.com)
 * @version 1.9.0
 **/

//Dependencies
#include <stdlib.h>
#include "xparameters.h"
// #include "microblaze_0/standalone_microblaze_0/bsp/microblaze_0/include/xparameters.h"
#include <stdio.h>
#include "xil_io.h"
#include "xintc_i.h"
#include "common/os_port.h"
#include "cyclone_tcp/core/net.h"
#include "cyclone_tcp/net_config.h"
#include "cyclone_tcp/drivers/eth/enc624j600_driver.h"
#include "cyclone_tcp/core/udp.h"
#include "common/error.h"
#include "cyclone_tcp/std_services/echo.h"
//#include "xgpio.h"
//#include "xuartlite.h"
#include "xspi.h"
//#include "myip_S4e_4reg.h"
#include "cyclone_tcp/spi_driver.h"
#include "cyclone_tcp/ext_int_driver.h"
#include "fft.h"
#include "FIFO_FFT_driver.h"
#include <stdbool.h>
//#include <math.h>


//Application configuration
#define APP_USE_DEFAULT_MAC_ADDR ENABLED
//#define APP_MAC_ADDR "00-AB-CD-EF-07-95"

#define APP_USE_DHCP DISABLED
#define APP_IPV4_HOST_ADDR "192.168.13.2"
#define APP_IPV4_SUBNET_MASK "255.255.255.0"
#define APP_IPV4_DEFAULT_GATEWAY "192.168.13.1"
#define APP_IPV4_PRIMARY_DNS "8.8.8.8"
#define APP_IPV4_SECONDARY_DNS "8.8.4.4"

#define APP_USE_SLAAC DISABLED


#define INTC_DEVICE_ID			XPAR_AXI_INTC_0_DEVICE_ID


//Global variables

u32 SourceBuffer[MAX_DATA_BUFFER_SIZE] = {0};
u32 FFTBuffer[MAX_DATA_BUFFER_SIZE] = {0};

/**
 * @brief System initialization
 **/

Ipv4Addr ipv4Addr;

void systemInit(void)
{
   error_t error;
   MacAddr macAddr;

   //Initialize kernel
   osInitKernel();

   //TCP/IP stack initialization
   error = netInit();

   //Configure the first Ethernet interface

   //Set interface name
   netSetInterfaceName(&netInterface[0], "eth0");
   //Set host name
   netSetHostname(&netInterface[0], "UDPEcho");
   //Select the relevant network adapter
   netSetDriver(&netInterface[0], &enc624j600Driver);
   //netSetPhyDriver(interface, &lan8720PhyDriver);

   netSetSpiDriver(&netInterface[0], &spiDriver);
   netSetExtIntDriver(&netInterface[0], &extIntDriver);

#if (APP_USE_DEFAULT_MAC_ADDR == ENABLED)
   //Use the factory preprogrammed MAC address
   macStringToAddr("00-00-00-00-00-00", &macAddr);
   netSetMacAddr(&netInterface[0], &macAddr);
#endif

   //Initialize network interface
   error = netConfigInterface(&netInterface[0]);
   //Any error to report?

#if (IPV4_SUPPORT == ENABLED)

   //Set IPv4 host address
   ipv4StringToAddr(APP_IPV4_HOST_ADDR, &ipv4Addr);
   ipv4SetHostAddr(&netInterface[0], ipv4Addr);

   //Set subnet mask
   ipv4StringToAddr(APP_IPV4_SUBNET_MASK, &ipv4Addr);
   ipv4SetSubnetMask(&netInterface[0], ipv4Addr);

   //Set default gateway
   ipv4StringToAddr(APP_IPV4_DEFAULT_GATEWAY, &ipv4Addr);
   ipv4SetDefaultGateway(&netInterface[0], ipv4Addr);

   //Set primary and secondary DNS servers
   ipv4StringToAddr(APP_IPV4_PRIMARY_DNS, &ipv4Addr);
   ipv4SetDnsServer(&netInterface[0], 0, ipv4Addr);
   ipv4StringToAddr(APP_IPV4_SECONDARY_DNS, &ipv4Addr);
   ipv4SetDnsServer(&netInterface[0], 1, ipv4Addr);
#endif

   //init_platform();
}


/* void MyIP_InterruptHandler(void *CallbackRef)
{
	// XIntc_Acknowledge(&InterruptController, XPAR_MICROBLAZE_0_AXI_INTC_CALCUL_MOYENNE_DSP2_DELAI_2CYCLE_Q_INTR);


} */

int SetupInterruptSystem()
{

	int Status;

	Status = XIntc_Initialize(&InterruptController, INTC_DEVICE_ID);

	/*
	 * Connect a device driver handler that will be called when an interrupt
	 * for the device occurs, the device driver handler performs the
	 * specific interrupt processing for the device. */

	Status = XIntc_Connect(&InterruptController, XPAR_AXI_INTC_0_SYSTEM_INT_NIC100_INTR,
			   (XInterruptHandler)ENC_Int_Handler,
			   (void *)0);

	Status = XIntc_Connect(&InterruptController, XPAR_INTC_0_LLFIFO_0_VEC_ID,
				   (XInterruptHandler)AXIS_InterruptHandler,
				   (void *)0);

		Status = XIntc_Start(&InterruptController, XIN_REAL_MODE);

		XIntc_Enable(&InterruptController, XPAR_INTC_0_LLFIFO_0_VEC_ID);

		Xil_ExceptionInit();

		Xil_ExceptionRegisterHandler(XIL_EXCEPTION_ID_INT,
					(Xil_ExceptionHandler)XIntc_InterruptHandler,
					&InterruptController);

	Xil_ExceptionEnable();

	return XST_SUCCESS;
}


bool udpSocketUp = false;

int_t main(void)
{
	int dataInFlag = 0;
	double maxModule= 0;
	double Module;
	char indiceMax[3] = {0};
	int Reel;
	int Imaginaire;
	int i;

   initFIFO_FFT();

   context.socket = 0;

   //Create user task
   SetupInterruptSystem();
   systemInit();
   netTaskInit();

   while(1)
   {

	   netTask();
	   if(netInterface[0].linkState == 1)
	   {
		   if(!udpSocketUp)
		   {
			   	   udpEchoStart();
				   udpSocketUp = true;
		   }
		   else
		   {
			   dataInFlag = udpEchoTask(SourceBuffer);

			   if(dataInFlag)
			   {
				   do_forward_FFT(SourceBuffer, FFTBuffer);

				   for (i=0; i<64;i++)
				   {
				   	 Reel = (int16_t)(FFTBuffer[i] & 0x0000FFFF);
				   	 Imaginaire = (int16_t)((FFTBuffer[i] & 0xFFFF0000) >> 16 );
				   	 Module = (double)((Reel*Reel)+(Imaginaire*Imaginaire));
				   	 if (Module > maxModule)
				 	 {
				   		maxModule = Module;
				   		indiceMax[0] = i;
				   	 }
				   }

				   	udpSendPacket(indiceMax);

				   	indiceMax[0] = 0;
					maxModule = 0;
					dataInFlag = 0;

			   }
		   }
	   }
	   else
	   {
		   if(udpSocketUp){
			   udpSocketUp = false;
		   }
	   }
   }

   //This function should never return
   return 0;
}
/* main.c
 *
 *  Created on: 25 mars 2019
 *      Author: jbm
 */




