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
//#include <stdlib.h>
#include "xparameters.h"
#include "FIFO_FFT_driver.h"

//stucture for the axi-stream fifos that feed the fft module
XLlFifo FifoInstance, FifoInstance2; //to write the data and the config

//data returned from axi-stream processing flag
volatile u32 Received;


//the output index of the FFT is bit reversed. Look-up table for the destination index
//e.g. index 1100110 is in fact the 0110011 element of the natural order result
u8 FFT_ReorderIndex[MAX_DATA_BUFFER_SIZE]=
 {0x00, 0x40, 0x20, 0x60, 0x10, 0x50, 0x30, 0x70, 0x08, 0x48, 0x28, 0x68, 0x18, 0x58, 0x38, 0x78,
  0x04, 0x44, 0x24, 0x64, 0x14, 0x54, 0x34, 0x74, 0x0C, 0x4C, 0x2C, 0x6C, 0x1C, 0x5C, 0x3C, 0x7C,
  0x02, 0x42, 0x22, 0x62, 0x12, 0x52, 0x32, 0x72, 0x0A, 0x4A, 0x2A, 0x6A, 0x1A, 0x5A, 0x3A, 0x7A,
  0x06, 0x46, 0x26, 0x66, 0x16, 0x56, 0x36, 0x76, 0x0E, 0x4E, 0x2E, 0x6E, 0x1E, 0x5E, 0x3E, 0x7E,
  0x01, 0x41, 0x21, 0x61, 0x11, 0x51, 0x31, 0x71, 0x09, 0x49, 0x29, 0x69, 0x19, 0x59, 0x39, 0x79,
  0x05, 0x45, 0x25, 0x65, 0x15, 0x55, 0x35, 0x75, 0x0D, 0x4D, 0x2D, 0x6D, 0x1D, 0x5D, 0x3D, 0x7D,
  0x03, 0x43, 0x23, 0x63, 0x13, 0x53, 0x33, 0x73, 0x0B, 0x4B, 0x2B, 0x6B, 0x1B, 0x5B, 0x3B, 0x7B,
  0x07, 0x47, 0x27, 0x67, 0x17, 0x57, 0x37, 0x77, 0x0F, 0x4F, 0x2F, 0x6F, 0x1F, 0x5F, 0x3F, 0x7F};


void initFIFO_FFT(){
	//initialize les fifos axi-stream
	int Status;
	Status = XLFifoInit(&FifoInstance, FIFO_FFT_ID);
	Status = XLFifoInit(&FifoInstance2, FIFO_CONFIG_ID);
	SetupInterruptSystem();
	XLlFifo_IntEnable(&FifoInstance, XLLF_INT_ALL_MASK);
}

void do_forward_FFT(u32* dataToFFT, u32* FFTResult)
{
	u32 FFTConf = 0x1; // = forward fft, no scaling
	if( XLlFifo_iTxVacancy(&FifoInstance2) ){
		XLlFifo_TxPutWord(&FifoInstance2, FFTConf);
	}
	XLlFifo_iTxSetLen(&FifoInstance2, 1);		/* The data is actually sent here by writing into the TLR */

	//Envoie les données à convertir au module FFT
	Received = 0;
	int Status = XLlFifoSendData(&FifoInstance, FIFO_FFT_ID, dataToFFT);
	while(!Received); //wait for the data to come out of the fft processing via interrupt

	//build the natural order result
	for(unsigned int i=0;i<MAX_DATA_BUFFER_SIZE;i++){
		FFTResult[i] = ReceiveTempBuffer[FFT_ReorderIndex[i]];
	}

}

void do_reverse_FFT(u32* dataToiFFT, u32* iFFTResult)
{
	//now for the IFFT
	u32 FFTConf = 0b01101010 << 1; //this is the scaling required to recover the original time-domain data; bit0 = 0 -> IFFT
	if( XLlFifo_iTxVacancy(&FifoInstance2) ){
		XLlFifo_TxPutWord(&FifoInstance2, FFTConf);
	}
	XLlFifo_iTxSetLen(&FifoInstance2, 1); //same as above

	Received = 0;
	int Status = XLlFifoSendData(&FifoInstance, FIFO_FFT_ID, dataToiFFT);
	while(!Received); //wait for the data to come out of the fft processing via interrupt

	//again build the natural order result
	for(unsigned int i=0;i<MAX_DATA_BUFFER_SIZE;i++){
		iFFTResult[i] = ReceiveTempBuffer[FFT_ReorderIndex[i]];
	}

}

