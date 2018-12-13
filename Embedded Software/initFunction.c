#include <stm32f4xx.h>
#include "initFunction.h"

void SysTickBaslat(void){
	SysTick->CTRL = 0;									 // Ayarlamalar icin sysTick sayiciyi kapat	
	SysTick->LOAD = 0x00014821;					 // Saymaya baslayacagi sayiyi ayarla 0.5ms (Ayarlanan Deger=84001-1)
	SysTick->VAL  = 0;									 // En son degeri sifirla
	SysTick->CTRL|= 0x00000007;					 // Sayiciyi ana clk ve kesmeyle birlikte aktif et
}

void system_init(){
	
	unsigned int i;

      for (i=0;i<0x00100000;i++);     											// OSC oturtma ve kurtarma rutini
	
      RCC->CFGR	  |= 0x00009400;        										// AHB ve APB hizlarini max degerlere set edelim
      RCC->CR 		|= 0x00010000;         									  // HSE Xtal osc calismaya baslasin       
	
      while (!(RCC->CR & 0x00020000));											// Xtal osc stabil hale gelsin
	
      RCC->PLLCFGR = 0x07402A04;     									 		  // PLL katsayilarini M=4, N=168, P=2 ve Q=7 yapalim   168 Mhz
      RCC->CR 		|= 0x01000000;        								    // PLL calismaya baslasin  (Rehber Sayfa 95)
	
      while(!(RCC->CR & 0x02000000));											  // Pll hazir oluncaya kadar bekle
	
      FLASH->ACR   = 0x00000605;        										// Flash ROM icin 5 Wait state secelim ve ART yi aktif edelim (Rehber Sayfa 55)
      RCC->CFGR 	|= 0x00000002;        										// Sistem Clk u PLL uzerinden besleyelim
	
      while ((RCC->CFGR & 0x0000000F) != 0x0000000A);	  		// Besleninceye kadar bekle
			
			//ADC1
			
      RCC->AHB1ENR 	|= 0x00000004;    											  // GPIO C portu clock'u aktif edelim
      GPIOC->MODER   = 0x000000C0; 									  		  	// GPIO C portunun 3. pinini Analog input yaptik
      GPIOC->OSPEEDR = 0xFFFFFFFF;  									  		  // GPIOC nin tum cikislari en yuksek hizda kullanacagiz	
			RCC->APB2ENR	|= 0x00000100;                     		    // ADC1 saat kaynagini aktif ettik.
			ADC1->CR1 		|= 0x00000020;														// ADC interrupt aktif, 12bit ADC resolution
			NVIC_EnableIRQ(ADC_IRQn);                               // NVIC ADC kesmesini aktif ettik.
			
			ADC1->SQR1  &= ~(15 << 20);                              // Kaç kanali tarayacagimizi bildiriyoruz. L[3:0] bitler; 1 kanal                    
      ADC1->SQR3  |= 0x0000000D;                               // Çevrime ilk girecek kanal 13. (PC3)
			/*SQRx registerlari ile birden fazla kanal ile adc yapiyorsak sirasini belirliyoruz
				burada sadece bir(CH13 / PC3) kanal kullanilicagi icin ilk siraya ch13 u yerlestirdik ve biraktik*/
				
			ADC1->CR2 |=0x00000001;                       			    // ADC converter'i açtik. Single  çevirim yapacagiz.
			
			ADC1->SMPR1 |=0xFFFFFFFF;                               // En düsük çevirim hizi.
      ADC1->SMPR2 |=0xFFFFFFFF;                               // En düsük çevirim hizi.
			
			//ADC2
			//GPIOC->MODER   |= 0x000000C3; 									  		  	// GPIO C portunun 3. pinini Analog input yaptik
}

void initButtons(){
	GPIOC->MODER		&=	 ~((3UL << 4*2)	| (3UL << 5*2 )  );	 		    // Digital Input PC4, PC5										 			
	GPIOC->OSPEEDR	|=	  ((3UL << 4*2)	| (3UL << 5*2 )	 );					// PC4, PC5 Very High Speed 									 		
	GPIOC->PUPDR	  &=	 ~((3UL << 4*2)	| (3UL << 5*2 )	 );					// No PullUp-Pull Down for PC4, PC5
	
	GPIOA->MODER		&=   ~((3UL << 13*2)	| (3UL << 14*2 )   ); 		// Digital Input PA13, PA14		
	GPIOA->OSPEEDR	|=	  ((3UL << 13*2)	| (3UL << 14*2 )	 );			//PC4, PC5 Very High Speed
	GPIOA->PUPDR		&=	 ~((3UL << 13*2)	| (3UL << 14*2 )	 );			//No PullUp-Pull Down for PA13, PA14
	
	GPIOB->MODER		&=   ~((3UL << 1*2));													 	// Digital Input PB1	
	GPIOB->OSPEEDR	|=	  ((3UL << 1*2));														//PC4, PC5 Very High Speed
	GPIOB->PUPDR		&=	 ~((3UL << 1*2));														//No PullUp-Pull Down for PB1
}


