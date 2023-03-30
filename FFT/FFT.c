#include "FFT.h"
#include "MAX7219.h"

u32 adcValues[NPT]={0};
long lBufInArray[NPT];
long lBufOutArray[NPT/2];
long lBufMagArray[NPT/2];
u8 fftDisplayBuffer[SIZE_OF_FREQ];
u8 isReverse = 0;
u8 count = 0;


void GetPowerMag(void)
{
    signed short lX,lY;
    float X,Y,Mag;
    unsigned short i;
    for(i=0; i<NPT/2; i++)
    {
        lX  = (lBufOutArray[i] << 16) >> 16;
        lY  = (lBufOutArray[i] >> 16);

		
        X = NPT * ((float)lX) / 32768;
        Y = NPT * ((float)lY) / 32768;
        Mag = sqrt(X * X + Y * Y)*1.0/ NPT;
        if(i == 0)
            lBufMagArray[i] = (unsigned long)(Mag * 32768);
        else
            lBufMagArray[i] = (unsigned long)(Mag * 65536);
    }
}


int64_t map(int64_t x, int64_t in_min, int64_t in_max, int64_t out_min, int64_t out_max)
{
	return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}


void Spectrum_Bounce(void)
{
	u16 i;
	uint8_t value = 0;
	uint8_t barIndex;
	
	for (i=0; i<NPT; i++)
	{
		lBufInArray[i] = ((signed short) (adcValues[i] - 2048)) << 16;
	}
	
	cr4_fft_256_stm32(lBufOutArray, lBufInArray, NPT);
	
	GetPowerMag();
	
	for (i = 0; i < SIZE_OF_FREQ; i++)
	{

		value = map(lBufMagArray[i * 4 + 3], 0, 128, 0, 8);
		value = value > 8 ? 8 : value;
		
		if(!isReverse)
		{
			value = (~(0xFF << value)) | 1;
			barIndex = i;
			
			if (value > fftDisplayBuffer[barIndex])
			{
				 fftDisplayBuffer[barIndex] = value;
			}
			else if (fftDisplayBuffer[barIndex] > 0x01 && (count++ > FALLBACK_LATCY))
			{
				fftDisplayBuffer[barIndex] >>= 1;
				count = 0;
			}
		}
		//·­×ª
//		else
//		{
//			value = 0xFF << (7 - value) | 0x80;
//			barIndex = 7 - i;
//			
//			if (value > fftDisplayBuffer[barIndex]) {
//				 fftDisplayBuffer[barIndex] = value;
//			}
//			else if (fftDisplayBuffer[barIndex] > 0x80 && (count++ > FALLBACK_LATCY)) {
//				fftDisplayBuffer[barIndex] <<= 1;
//				count = 0;
//			}
//		}
	}
	
	for (i = 0; i < SIZE_OF_FREQ; i++) {
		MAX7219_Display(i, fftDisplayBuffer[i]);
	}
	
}
