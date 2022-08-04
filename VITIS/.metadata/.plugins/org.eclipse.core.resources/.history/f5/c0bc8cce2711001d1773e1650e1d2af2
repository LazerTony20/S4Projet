/***************************** Include Files *********************************/
#include "FIFO_FFT_driver.h"



#undef DEBUG

/*
 * Flags interrupt handlers use to notify the application context the events.
 */
volatile int Done;
volatile int Error;

extern u32 Received;
u32 ReceiveTempBuffer[MAX_DATA_BUFFER_SIZE];


XLlFifo FifoInstance, FifoInstance2;

/************************** Variable Definitions *****************************/


/*
 * Instance of the Interrupt Controller
 */

//stuff for interrupt management
XIntc InterruptController; /* Instance of the Interrupt Controller */

void AXIS_InterruptHandler(void *CallbackRef)
{
	xil_printf("In interrupt\n\r");
	FifoHandler(&FifoInstance);
}

int SetupInterruptSystem()
{
	int Status;

	Status = XIntc_Initialize(&InterruptController, XPAR_INTC_0_DEVICE_ID);

	/*
	 * Connect a device driver handler that will be called when an interrupt
	 * for the device occurs, the device driver handler performs the
	 * specific interrupt processing for the device.
	 */
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

int XLFifoInit(XLlFifo *InstancePtr, u16 DeviceId){

	XLlFifo_Config *Config;
	int Status;
	Status = XST_SUCCESS;

	/* Initialize the Device Configuration Interface driver */
	Config = XLlFfio_LookupConfig(DeviceId);
	if (!Config) {
		xil_printf("No config found for %d\r\n", DeviceId);
		return XST_FAILURE;
	}

	/*
	 * This is where the virtual address would be used, this example
	 * uses physical address.
	 */
	Status = XLlFifo_CfgInitialize(InstancePtr, Config, Config->BaseAddress);
	if (Status != XST_SUCCESS) {
		xil_printf("Initialization failed\n\r");
		return Status;
	}

	/* Check for the Reset value */
	Status = XLlFifo_Status(InstancePtr);
	XLlFifo_IntClear(InstancePtr,0xffffffff);
	Status = XLlFifo_Status(InstancePtr);
	if(Status != 0x0) {
		xil_printf("\n ERROR : Reset value of ISR0 : 0x%x\t"
			    "Expected : 0x0\n\r",
			    XLlFifo_Status(InstancePtr));
		return XST_FAILURE;
	}


	return XST_SUCCESS;
}

/*****************************************************************************/
/**
*
* This function demonstrates the usage of AXI FIFO
* It does the following:
*       - Set up the output terminal if UART16550 is in the hardware build
*       - Initialize the Axi FIFO Device.
*	- Set up the interrupt handler for fifo
*	- Transmit the data
*	- Compare the data
*	- Return the result
*
* @param	InstancePtr is a pointer to the instance of the
*		XLlFifo instance.
* @param	DeviceId is Device ID of the Axi Fifo Deive instance,
*		typically XPAR_<AXI_FIFO_instance>_DEVICE_ID value from
*		xparameters.h.
*
* @return
*		-XST_SUCCESS to indicate success
*		-XST_FAILURE to indicate failure
*
******************************************************************************/
int XLlFifoSendData(XLlFifo *InstancePtr, u16 DeviceId, u32* DataBuffer)
{
	int Status;

	Done = 0;
	/* Transmit the Data Stream */
	Status = TxSend(InstancePtr, DataBuffer);
	while(!Done); //wait for the fifo to be fully transmitted; this is indicated by an interrupt flag that sets Done

	/* Check for errors */
	if(Error) {
		xil_printf("Errors in the FIFO\n\r");
		return XST_FAILURE;
	}

	return Status;
}

/*****************************************************************************/
/**
*
* TxSend routine, It will send the requested amount of data at the
* specified addr.
*
* @param	InstancePtr is a pointer to the instance of the
*		XLlFifo component.
*
* @param	SourceAddr is the address of the memory
*
* @return
*		-XST_SUCCESS to indicate success
*		-XST_FAILURE to indicate failure
*
* @note		None
*
******************************************************************************/
int TxSend(XLlFifo *InstancePtr, u32  *SourceAddr)
{
	int i;
	int j;
	xil_printf("Transmitting Data ... \r\n");

	for(i=0; i < NO_OF_PACKETS; i++){
		/* Writing into the FIFO Transmit Port Buffer */
		for (j=0; j < MAX_FFT_LEN; j++){
			if( XLlFifo_iTxVacancy(InstancePtr) ){
				XLlFifo_TxPutWord(InstancePtr,
					*(SourceAddr+(i*MAX_FFT_LEN)+j));
			}
		}

	}

	/* Start Transmission by writing transmission length into the TLR */
	XLlFifo_iTxSetLen(InstancePtr, (MAX_DATA_BUFFER_SIZE * WORD_SIZE));


	/* Transmission Complete */
	return XST_SUCCESS;
}

/*****************************************************************************/
/**
*
* This is the interrupt handler for the fifo it checks for the type of interrupt
* and proceeds according to it.
*
* @param	InstancePtr is a reference to the Fifo device instance.
*
* @return	None.
*
* @note		None.
*
******************************************************************************/
void FifoHandler(XLlFifo *InstancePtr)
{
	u32 Pending;

	Pending = XLlFifo_IntPending(InstancePtr);
	while (Pending) {
		if (Pending & XLLF_INT_RC_MASK) {
			FifoRecvHandler(InstancePtr);
			XLlFifo_IntClear(InstancePtr, XLLF_INT_RC_MASK);
		}
		else if (Pending & XLLF_INT_TC_MASK) {
			FifoSendHandler(InstancePtr);
		}
		else if (Pending & XLLF_INT_ERROR_MASK){
			FifoErrorHandler(InstancePtr, Pending);
			XLlFifo_IntClear(InstancePtr, XLLF_INT_ERROR_MASK);
		} else {
			XLlFifo_IntClear(InstancePtr, Pending);
		}
		Pending = XLlFifo_IntPending(InstancePtr);
	}

}

/*****************************************************************************/
/**
*
* This is the Receive handler callback function.
*
* @param	InstancePtr is a reference to the Fifo device instance.
*
* @return	None.
*
* @note		None.
*
******************************************************************************/
void FifoRecvHandler(XLlFifo *InstancePtr)
{
	unsigned int i;
	u32 RxWord;
	u32 ReceiveLength;

	xil_printf("Receiving Data... \n\r");

	while(XLlFifo_iRxOccupancy(InstancePtr)) {
		ReceiveLength = (XLlFifo_iRxGetLen(InstancePtr)) & 0x7FFFFFFF;
		ReceiveLength /= 4;
		xil_printf("Length %d ", ReceiveLength);
		for (i=0; i < ReceiveLength; i++) {
				RxWord = XLlFifo_RxGetWord(InstancePtr);
				ReceiveTempBuffer[i] = RxWord;
		}
	}
	Received = 1;
}

/*****************************************************************************/
/*
*
* This is the transfer Complete Interrupt handler function.
*
* This clears the trasmit complete interrupt and set the done flag.
*
* @param	InstancePtr is a pointer to Instance of AXI FIFO device.
*
* @return	None
*
* @note		None
*
******************************************************************************/
void FifoSendHandler(XLlFifo *InstancePtr)
{
	XLlFifo_IntClear(InstancePtr, XLLF_INT_TC_MASK);

	Done = 1;
}

/*****************************************************************************/
/**
*
* This is the Error handler callback function and this function increments the
* the error counter so that the main thread knows the number of errors.
*
* @param	InstancePtr is a pointer to Instance of AXI FIFO device.
*
* @param	Pending is a bitmask of the pending interrupts.
*
* @return	None.
*
* @note		None.
*
******************************************************************************/
void FifoErrorHandler(XLlFifo *InstancePtr, u32 Pending)
{
	if (Pending & XLLF_INT_RPURE_MASK) {
		XLlFifo_RxReset(InstancePtr);
	} else if (Pending & XLLF_INT_RPORE_MASK) {
		XLlFifo_RxReset(InstancePtr);
	} else if(Pending & XLLF_INT_RPUE_MASK) {
		XLlFifo_RxReset(InstancePtr);
	} else if (Pending & XLLF_INT_TPOE_MASK) {
		XLlFifo_TxReset(InstancePtr);
	} else if (Pending & XLLF_INT_TSE_MASK) {
	}
	Error++;
}


/*****************************************************************************/
/**
*
* This function disables the interrupts for the AXI FIFO device.
*
* @param	IntcInstancePtr is the pointer to the INTC component instance
* @param	FifoIntrId is interrupt ID associated for the FIFO component
*
* @return	None
*
* @note		None
*
******************************************************************************/
void DisableIntrSystem(XIntc *IntcInstancePtr, u16 FifoIntrId)
{
	/* Disconnect the interrupts */
	XIntc_Disconnect(IntcInstancePtr, FifoIntrId);
}

