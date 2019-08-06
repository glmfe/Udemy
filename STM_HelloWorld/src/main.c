/**
  ******************************************************************************
  * @file    main.c
  * @author  Ac6
  * @version V1.0
  * @date    01-December-2013
  * @brief   Default main function.
  ******************************************************************************
*/

#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include "FreeRTOS.h"
#include "stm32f4xx.h"
#include "task.h"

#ifdef USE_SEMIHOSTING
	extern void initialise_monitor_handles();
#endif

void vTask1_handler(void *params);
void vTask2_handler(void *params);

static void prvSetupHardware(void);

TaskHandle_t xTaskHandle_1 = NULL;
TaskHandle_t xTaskHandle_2 = NULL;


//uses for semihosting debug

int main(void)
{

	#ifdef USE_SEMIHOSTING
		initialise_monitor_handles();
		printf("Init SEMIHOSTING\n");
	#endif
	//1. Reset Clock to the default state
	//HSI ON, PLL OFF, HSE OFF, System clock = 16Mhz
	RCC_DeInit();
	//Update the SysCoreClock Var
	SystemCoreClockUpdate();

	//Create two Tasks
	xTaskCreate(vTask1_handler,
			"Task 1",
			configMINIMAL_STACK_SIZE,
			NULL,
			2,
			&xTaskHandle_1);

	xTaskCreate(vTask2_handler,
			"Task 2",
			configMINIMAL_STACK_SIZE,
			NULL,
			2,
			&xTaskHandle_2);

	//Scheduling the tasks
	vTaskStartScheduler();

	for(;;);

}


void vTask1_handler(void *params)
{
	while(1){
	}

}

void vTask2_handler(void *params)
{
	while(1){
	}
}

static void prvSetupHardware(void){

	GPIO_InitTypeDef gpio_uart_pins;
	USART_InitTypeDef uart2_init;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	//PA2 as TX PA3 as RX
	memset(&gpio_uart_pins,0, sizeof(gpio_uart_pins));

	gpio_uart_pins.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3;
	gpio_uart_pins.GPIO_Mode = GPIO_Mode_AF;
	gpio_uart_pins.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOA, &gpio_uart_pins);

	GPIO_PinAFConfig(GPIOA, GPIO_PinSource2,GPIO_AF_USART2);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource3,GPIO_AF_USART2);

	memset(&uart2_init,0, sizeof(uart2_init));
	uart2_init.USART_BaudRate = 115200;
	uart2_init.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	uart2_init.USART_Mode = USART_Mode_Tx | USART_Mode_Tx;
	uart2_init.USART_Parity = USART_Parity_No;
	uart2_init.USART_StopBits = USART_StopBits_1;
	uart2_init.USART_WordLength = USART_WordLength_8b;

	USART_Init(USART2, &uart2_init);

}

