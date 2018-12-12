#include <stm32f4xx.h>
#include "main.h"
#include "initFunction.h"
#include "UserFunctions.h"

unsigned int ADCBuffer[50]  = {0};									//ADC den okunucak bilginin kayit edilicegi buffer 
unsigned char	adcTrigger 	  	   = 0;								//ADC converter i baslat1cak trigger degisken
unsigned int sampleNumber        = 10;
unsigned int maxAverageValue     = 2000;
short        ADC_compareResult   = 0;
unsigned int  BufferCounter      = 0;										//bufferda okuma yazma yapabilmek için gerekli sayici

void ADC_IRQHandler(){   												//ADC kesme rutini
	if(ADC1->SR & 0x0002){												//ADC çevrim tamamlandi bayragi kontrol ediliyor.
		ADC1->SR &= ~0x0002;												//ADC çevrim bayragi kalmissa tekrardan temizlenip fonksiyona giriliyor

		ADC_compareResult = sensorFilterRoutine(sampleNumber, maxAverageValue, ADCBuffer, &BufferCounter);

	}
}


int main(){
	system_init();
	
	while(1){
			ADC_ConvertOneTime(&adcTrigger);                 			//ADC okumaya basladi
	}
}

