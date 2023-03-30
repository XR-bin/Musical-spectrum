#ifndef _FFT_H
#define _FFT_H

	#include "stm32f10x.h"
	#include "stm32_dsp.h"
	#include "math.h"
	
	#define NPT     256     // FFT Point
	#define SIZE_OF_FREQ  8         // Bar size
	#define FALLBACK_LATCY 1        // 
	
	extern u32 adcValues[NPT];
	extern long lBufInArray[NPT];
	extern long lBufOutArray[NPT/2];
	
	void Spectrum_Bounce(void);

#endif
