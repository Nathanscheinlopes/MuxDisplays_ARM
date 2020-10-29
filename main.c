	/*
 * Exercicio.c
 *
 *  Created on: Aug 31, 2020
 *      Author: Nathan
 *      Exercício 1 : “Hello word!”
    Este é o clássico dos clássicos. Elabore um programa capaz de piscar o LD2
    com um ciclo de trabalho de 50% (metade do tempo ligado, metade
    desligado) e com período de 500ms (2 Hz ou 2 piscadas/segundo).
 */

#include "stm32f4xx.h"


int main()
{
	RCC->AHB1ENR=0x00000087;//habilita o clock GPIO
	GPIOA->MODER=0xA8000400;//comunicação do debug e pa5 como saída
	RCC->APB2ENR= RCC_APB2ENR_TIM11EN;//habilita o clock do TIM11
	TIM11->PSC=9999;//
	TIM11->ARR=399;// 4 hz
	TIM11->CR1=0x01;// da o enable no contador
	while(1)
	{
		if((TIM11->SR & 0x01)==1)//se o flag UIF do registrador SR ligar
		{
			GPIOA->ODR ^= GPIO_ODR_ODR_5;//xor para piscar o led LD2 conectado ao PA5
			TIM11->SR&=~1;//reset flag UIF
		}
	}
}
