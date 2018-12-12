#include <stm32f4xx.h>
#include "UserFunctions.h"



unsigned int getAverage(unsigned int *index,int indexNumber){
	unsigned int sumOfIndex = 0;
	int i;																								//döngüde kullanilicak counter
	
	for(i = indexNumber ;i >0 ; i--){
		sumOfIndex += index[i-1];														//en büyük eleman eleman sayisinin bir eksigi oldugu için index-1 ifadesi ile elmanlara ulastik
	}
	
	return (sumOfIndex/indexNumber);											//tüm elemanlarin toplamini eleman sayisina böl ve döndür
}

void ADC_ConvertOneTime(unsigned char* triggerValue){
	if((*triggerValue) != 0){
		ADC1->CR2 |= 0x40000000;															//ADC1_CR2 biti set edildiginde adc tek tur conversion yapar(single cycle modda)
		*triggerValue = 0;
	}
}

void ADC_ConvertSampleTime(unsigned char triggerValue,unsigned int sampleNumber){	//triggerin sampleNumber kadar kendini tekrar tetiklemesiasdf
	if((triggerValue) != sampleNumber){
		ADC1->CR2 |= 0x40000000;
	}
	
}

short valueComparator(unsigned int data, unsigned int maxValue){
	if(data >= maxValue){
		return 1;
	}
	else{
		return 0;
	}
}

unsigned int sensorFilterRoutine(unsigned int sampleNumber, unsigned int maxAverageValue, unsigned int* sampleBuffer, unsigned int* BufferCounter){
	
	short				  biggerOrSmaller  	 = 0;
	
	sampleBuffer[(*BufferCounter)] = ADC1->DR;				//ADC okur
	(*BufferCounter)++;												  		//tek trigger da 10 okuma yapmasi için gerekli counter
	
	if((*BufferCounter) != sampleNumber){										
		ADC_ConvertSampleTime((*BufferCounter), sampleNumber);											//eger 10 conversion yapilmadiysa tekrar adc yi tetikle
	}
	else if((*BufferCounter) == sampleNumber){								//eger 10 conversion yapildiysa diziyi ve eleman sayisini getAverage a gönder
		unsigned int ADCBufferAverage    = 0;								//ADC'de degerler okunduktan sonra ortalamanin tutulucagi degisken
		
		ADCBufferAverage = getAverage(sampleBuffer ,sampleNumber);
		biggerOrSmaller = valueComparator(ADCBufferAverage, maxAverageValue);	//10 sample in ortalamasi 2000 den büyükse 1 kücükse 0 döner.
		(*BufferCounter) = 0;												//bir sonraki trigger da kullanmak için buggerCounteri temizle
		
		return biggerOrSmaller;
	}
	return 0;
}
