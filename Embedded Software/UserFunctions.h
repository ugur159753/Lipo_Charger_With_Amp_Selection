
unsigned int getAverage(unsigned int *,int );				//bir dizi ve o dizin kaç eleman aldigi bilgisini alir ve
																										//tum elemanlarin ortalamasini döndürür.
																																		
void ADC_ConvertOneTime(unsigned char* triggerValue);							//ADC yi tek tur döndürmek için ADC1_CR2 bitini set eder

void ADC_ConvertSampleTime(unsigned char triggerValue, unsigned int BufferCounter);							

short valueComparator(unsigned int, unsigned int);	//ADC den gelen digital degeri istenen degerle kars1lastirir	

unsigned int sensorFilterRoutine(unsigned int sampleNumber, unsigned int maxAverageValue, unsigned int* sampleBuffer,unsigned int *BufferCounter);
