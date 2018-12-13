#include <stm32f4xx.h>
#include "main.h"
#include "initFunction.h"
#include "UserFunctions.h"

unsigned int ADCBuffer[50]  = {0};									//ADC den okunucak bilginin kayit edilicegi buffer 
unsigned char	adcTrigger 	  	   = 0;								//ADC converter i baslat1cak trigger degisken
unsigned int sampleNumber        = 10;
unsigned int maxAverageValue     = 2000;
unsigned int ADCBufferAverage   = 0;
unsigned int  BufferCounter      = 0;										//bufferda okuma yazma yapabilmek i�in gerekli sayici
volatile unsigned long systickCNT = 0;
unsigned char AdcBufferReadyFlag = 0;								//ADC 10 �evrim yapip ortalamas1n1 aldiginda bu flagi set eder

void SysTick_Handler(void){
	systickCNT++;
	adcTrigger = 1;
}



void ADC_IRQHandler(){   												//ADC kesme rutini
	if(ADC1->SR & 0x0002){												//ADC �evrim tamamlandi bayragi kontrol ediliyor.
		ADC1->SR &= ~0x0002;												//ADC �evrim bayragi kalmissa tekrardan temizlenip fonksiyona giriliyor

		sensorFilterRoutine(sampleNumber, maxAverageValue, ADCBuffer, &BufferCounter, &AdcBufferReadyFlag, &ADCBufferAverage);

	}
}

int main(){
	
	system_init();
	SysTickBaslat();											
	initButtons();
	while(1){
			ADC_ConvertOneTime(&adcTrigger);                 			//ADC okumaya basladi
	}
}

