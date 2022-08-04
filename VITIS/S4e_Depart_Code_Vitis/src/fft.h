#ifndef FFT_H_
#define FFT_H_


#include "FIFO_FFT_driver.h"

void do_forward_FFT(u32* dataToFFT, u32* FFTResult);
void initFIFO_FFT();

extern XLlFifo FifoInstance, FifoInstance2;

#endif
