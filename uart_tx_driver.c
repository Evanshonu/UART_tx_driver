#include "stm32f4xx.h"
#include<stdint.h>
#include<stdio.h>
#define   GPIOAEN		(1UL<<0)
#define   GPIOBEN		(1UL<<1)
#define   GPIOCEN		(1UL<<2)
#define   USART2EN		(1UL<<17);

#define  CR1_TE			(1UL<<3)
#define  CR1_UE			(1UL<<13)

#define  SYS_FREQ		16000000
#define  BAUDRATE		115200
#define  ABP1_CLK		SYS_FREQ

void uart_tx_init(void);
uint16_t compute_bd(uint32_t PeriphCLK, uint32_t BaudRate);
void static set_baudRate(USART_TypeDef *USARTx, uint32_t PeriphCLK,
		uint32_t BaudRate);
void transmit(int character);
void tramsmisionComplete(void);

int main(void) {
	uart_tx_init();
	while (1) {
		printf("Hello Evans, Welcome to STM32 world \n\r");
		tramsmisionComplete();
	}

}

void transmit(int character) {
	/***Check whether the transmit data register not empty**/
	while (!(USART2->SR & (1UL << 7))) {
	}

	/**write to the UART Data register***/
	USART2->DR = (character & 0xFF);
}

void uart_tx_init(void) {
	/**Enable clock access to GPIOA **/
	RCC->AHB1ENR |= GPIOAEN;

	/**Set PA2 as MODER as alternate function**/
	GPIOA->MODER &= ~(1UL << 4);
	GPIOA->MODER |= (1UL << 5);

	/**Set Alternate function type to Tx	**/
	GPIOA->AFR[0] |= (1ul << 8);
	GPIOA->AFR[0] |= (1ul << 9);
	GPIOA->AFR[0] |= (1ul << 10);
	GPIOA->AFR[0] &= ~(1ul << 11);
	/**Enable clock access to the USART module**/
	RCC->APB1ENR |= USART2EN;

	/**Configure the baudRate**/
	set_baudRate(USART2, ABP1_CLK, BAUDRATE);

	/**Enable the transfer direction**/
	USART2->CR1 = CR1_TE;

	/**Enable the UART module**/
	USART2->CR1 |= CR1_UE;
}
void static set_baudRate(USART_TypeDef *USARTx, uint32_t PeriphCLK,
		uint32_t BaudRate) {
	USARTx->BRR = compute_bd(PeriphCLK, BaudRate);
}
uint16_t compute_bd(uint32_t PeriphCLK, uint32_t BaudRate) {
	return ((PeriphCLK + (BaudRate / 2U)) / BaudRate);
}
void tramsmisionComplete(void){
	//Setting GPIOA as outPUT
	GPIOA->MODER |= (1UL<<10);
	GPIOA->MODER &=~(1UL<<11);
	/**Using GPIOA PA5 as indicator**/
 while(!(USART2->SR &(1UL<<6))){}

 //toggle the led for 100ms
 GPIOA->ODR ^=(1UL<<5);
 for(int i = 0; i<100000;i++){}
}
