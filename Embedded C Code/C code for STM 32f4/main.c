#include "stm32f4xx.h"
#include "stm32f4_discovery.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_adc.h"

#include <stdio.h>

GPIO_InitTypeDef GPIO_BUTTON;
GPIO_InitTypeDef GPIO_LED;

#define BUTTON_1 GPIO_Pin_0 //port A
#define GREEN GPIO_Pin_12 // PORT D
#define BLUE GPIO_Pin_15 // PORT D
#define RED GPIO_Pin_14 // PORT D
#define ORANGE GPIO_Pin_13 // PORT D
// For ADC value
char sAdcValue[5];
volatile uint16_t adcValue = 0;
//volatile char here because it is changing in two different functions
volatile char counter = 0;
//crude delay for button debounce
void Delay(__IO uint32_t time){
	int i = 0;
	for(i = 0; i < time; i++){

	}
}
void InitializeIO()
{
	GPIO_InitTypeDef GPIO_LEDs;

	// Enable clock for GPIOD
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);

	GPIO_LEDs.GPIO_Pin = ORANGE|BLUE|RED|GREEN;
	GPIO_LEDs.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_LEDs.GPIO_OType = GPIO_OType_PP;
	GPIO_LEDs.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_LEDs.GPIO_Speed = GPIO_Speed_50MHz;

	GPIO_Init(GPIOD, &GPIO_LEDs);
}
void Initialize_Buttons(){
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	GPIO_BUTTON.GPIO_Pin = BUTTON_1;
	GPIO_BUTTON.GPIO_Mode = GPIO_Mode_IN;
	GPIO_BUTTON.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_BUTTON);
}

void ADC_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	ADC_InitTypeDef ADC_InitStruct;

	//Enable the clock on GPIO B
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);

    // Init GPIOB for ADC input - Channel 9 (PB1)
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AN;
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_1;
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOB, &GPIO_InitStruct);

    // Enable clock for ADC1
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);

    // Init ADC1
    ADC_InitStruct.ADC_ContinuousConvMode = DISABLE;
    ADC_InitStruct.ADC_DataAlign = ADC_DataAlign_Right;
    ADC_InitStruct.ADC_ExternalTrigConv = DISABLE;
    ADC_InitStruct.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
    ADC_InitStruct.ADC_NbrOfConversion = 1;
    ADC_InitStruct.ADC_Resolution = ADC_Resolution_12b;
    ADC_InitStruct.ADC_ScanConvMode = DISABLE;
    ADC_Init(ADC1, &ADC_InitStruct);

    ADC_Cmd(ADC1, ENABLE);

    // Select input channel for ADC1
    // ADC1 channel 9 is on PB1
    ADC_RegularChannelConfig(ADC1, ADC_Channel_9, 1, ADC_SampleTime_84Cycles);
}

void config_UART()
{
	GPIO_InitTypeDef GPIO_Struct;
	USART_InitTypeDef UART_Struct;

	// Enable clock for GPIOA
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

	// Enable clock for USART 2
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);

	// Connect PA2 to USART2_TX
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_USART2);
	// Connect PA3 to USART2_RX
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_USART2);

	// Initialize GPIOA
	GPIO_Struct.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3;
	GPIO_Struct.GPIO_Mode = GPIO_Mode_AF;
	GPIO_Struct.GPIO_OType = GPIO_OType_PP;
	GPIO_Struct.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Struct.GPIO_Speed = GPIO_Speed_50MHz;

	GPIO_Init(GPIOA, &GPIO_Struct);

	// Initialize UART
	UART_Struct.USART_BaudRate = 9600;
	UART_Struct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	UART_Struct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	UART_Struct.USART_Parity = USART_Parity_No;
	UART_Struct.USART_StopBits = USART_StopBits_1;
	UART_Struct.USART_WordLength = USART_WordLength_8b;

	USART_Init(USART2, &UART_Struct);

	// Enable USART 2
	USART_Cmd(USART2, ENABLE);
}
//put string and put char to print out the string of chars that the voltage is
void USART_PutChar(char c)
 {
	 while( !USART_GetFlagStatus(USART2, USART_FLAG_TXE) );
	 USART_SendData(USART2, c);
 }

void USART_PutString(char *s)
{
	// Send a string
	while(*s)
	{
		USART_PutChar(*s++);
	}
}
//configure the button
void configure_PA0(void){

	/* Set variables used */
	GPIO_InitTypeDef GPIO_InitStruct;
	EXTI_InitTypeDef EXTI_InitStruct;
	NVIC_InitTypeDef NVIC_InitStruct;

	/* Enable clock for GPIOA */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	/* Enable clock for SYSCFG */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

	/* Set pin as input */
	GPIO_InitStruct.GPIO_Pin = BUTTON_1;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;
	//GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	//GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(GPIOA, &GPIO_InitStruct);

	/* Tell system that you will use PA0 for EXTI_Line0 */
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource0);

	/* PA0 is connected to EXTI_Line0 */
	EXTI_InitStruct.EXTI_Line = EXTI_Line0;
	/* Enable interrupt */
	EXTI_InitStruct.EXTI_LineCmd = ENABLE;
	/* Interrupt mode */
	EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
	/* Triggers on rising and falling edge */
	EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
	/* Add to EXTI */
	EXTI_Init(&EXTI_InitStruct);

	/* Add IRQ vector to NVIC */
	/* PA0 is connected to EXTI_Line0, which has EXTI0_IRQn vector */
	NVIC_InitStruct.NVIC_IRQChannel = EXTI0_IRQn;
	/* Set priority */
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0x00;
	/* Set sub priority */
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0x00;
	/* Enable interrupt */
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	/* Add to NVIC */
	NVIC_Init(&NVIC_InitStruct);
}

//adc read function to get a uint16_t
uint16_t ADC_Read(void)
{
    // Start ADC conversion
    ADC_SoftwareStartConv(ADC1);

    // Wait until conversion is finish
    while (!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC));

    return ADC_GetConversionValue(ADC1);
}
//button interrupt handler
void EXTI0_IRQHandler(void){
	if(EXTI_GetITStatus(EXTI_Line0) != RESET){
		//iterate counter;
		counter = counter + 1;
		//turn on LEDS to track button presses
		if(counter == 1){
			GPIO_WriteBit(GPIOD, ORANGE, Bit_SET);//led on
			GPIO_WriteBit(GPIOD, BLUE|RED|GREEN, Bit_RESET);//led off
		}
		if(counter == 2){
			GPIO_WriteBit(GPIOD, BLUE, Bit_SET);//led on
			GPIO_WriteBit(GPIOD, ORANGE|RED|GREEN, Bit_RESET);//led off
		}
		if(counter == 3 ){
			GPIO_WriteBit(GPIOD, RED, Bit_SET);//led on
			GPIO_WriteBit(GPIOD, ORANGE|BLUE|GREEN, Bit_RESET);//led off
		}
		if(counter == 4){
			GPIO_WriteBit(GPIOD, GREEN, Bit_SET);//led on
			GPIO_WriteBit(GPIOD, ORANGE|BLUE|RED, Bit_RESET);//led off
		}
		if(counter == 5){
			GPIO_WriteBit(GPIOD, ORANGE|BLUE|RED|GREEN, Bit_RESET);//led off
		}
		//crude delay
		Delay(2000000);
		EXTI_ClearFlag(EXTI_Line0);
	}

}

int main(void)
{
    // ADC Config
	//initalize GPIO, button, ADC, and UART
	InitializeIO();
	ADC_Config();
	config_UART();
	Initialize_Buttons();
	configure_PA0();
	/* Infinite loop */

	while (1){
		if(counter == 5){
			counter = 0;
			adcValue = ADC_Read();
			//a crude conversion, the thousands place is the ones place, and everything to the
			//right of the thousands is the decimal. Converts to voltage decently well. 1.000v as read on PS is 1009 in the terminal
			//.732 is roughly 3/4.096
			adcValue = adcValue * .732;
			sprintf(sAdcValue, "%i\n", adcValue);
			USART_PutString(sAdcValue);

		}
	}
}


/*
 * Callback used by stm32f4_discovery_audio_codec.c.
 * Refer to stm32f4_discovery_audio_codec.h for more info.
 */
void EVAL_AUDIO_TransferComplete_CallBack(uint32_t pBuffer, uint32_t Size){
  /* TODO, implement your code here */
  return;
}

/*
 * Callback used by stm324xg_eval_audio_codec.c.
 * Refer to stm324xg_eval_audio_codec.h for more info.
 */
uint16_t EVAL_AUDIO_GetSampleCallBack(void){
  /* TODO, implement your code here */
  return -1;
}
