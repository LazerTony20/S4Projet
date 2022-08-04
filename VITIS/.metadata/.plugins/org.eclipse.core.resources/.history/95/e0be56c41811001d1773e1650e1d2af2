/*
 * example.h
 *
 *  Created on: 8 juil. 2019
 *      Author: jbm
 */

#ifndef FIFO_H_
#define FIFO_H_

#include "xparameters.h"
#include "xil_exception.h"
#include "xstreamer.h"
#include "xil_cache.h"
#include "xllfifo.h"
#include "xstatus.h"
#include "xil_printf.h"

#include "xintc.h"

int XLlFifoSendData(XLlFifo *InstancePtr, u16 DeviceId, u32* DataBuffer);
int TxSend(XLlFifo *InstancePtr, u32 *SourceAddr);
void FifoHandler(XLlFifo *Fifo);
void FifoRecvHandler(XLlFifo *Fifo);
void FifoSendHandler(XLlFifo *Fifo);
void FifoErrorHandler(XLlFifo *InstancePtr, u32 Pending);
void DisableIntrSystem(XIntc *IntcInstancePtr, u16 FifoIntrId);
int SetupInterruptSystem();
int XLFifoInit(XLlFifo *InstancePtr, u16 DeviceId);


//FFT Bloc Parameters
#define WORD_SIZE 4			/* Size of words in bytes */
#define MAX_FFT_LEN 128
#define NO_OF_PACKETS 1
#define MAX_DATA_BUFFER_SIZE NO_OF_PACKETS*MAX_FFT_LEN

#define FIFO_FFT_ID	   		XPAR_AXI_FIFO_0_DEVICE_ID     //The actual data FIFO
#define FIFO_CONFIG_ID		XPAR_AXI_FIFO_1_DEVICE_ID	  //THe FIFO only configures the FFT block


extern u32 ReceiveTempBuffer[MAX_DATA_BUFFER_SIZE]; //buffer for the interrupt data storage

#endif /* FIFO_H_ */
