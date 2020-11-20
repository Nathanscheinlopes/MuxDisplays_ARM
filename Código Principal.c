/*O painel deve ser composto por, no mínimo, 4 displays de 7 segmentos.
 Os displays devem utilizar a técnica de multiplexação para reduzir o uso dos pinos de
GPIO.
 As mensagens exibidas devem ter, no mínimo 10 caracteres afanuméricos (incluindo
espaço)
 Todos os displays devem aparentar o mesmo brilho.
 Não pode ocorrer flicker (cintilação) na exibição da mensagem
*/

#include "stm32f4xx.h"
#include "BinariosDisplay.h"//biblioteca com defines de binarios para o alfabeto

int main()
{
    //LIGANDO CLOCK REGISTRADORES//
    RCC->AHB1ENR=RCC_AHB1ENR_GPIOBEN|RCC_AHB1ENR_GPIOAEN|RCC_AHB1ENR_GPIOCEN; // Ligando clock dos registradores GPIO
    RCC->APB2ENR|=RCC_APB2ENR_TIM10EN|RCC_APB2ENR_TIM11EN;	//Ligando clock dos Timers 10 e 11
    //ENTRADAS E SAIDAS//
    GPIOA->MODER&=~(GPIO_MODER_MODER0|GPIO_MODER_MODER1|GPIO_MODER_MODER4|GPIO_MODER_MODER6|GPIO_MODER_MODER5|GPIO_MODER_MODER7|GPIO_MODER_MODER9);//PA0,PA1,PA4,PA6,PA5,PA7 E PA9 SAIDAS
    GPIOA->MODER|=GPIO_MODER_MODER0_0|GPIO_MODER_MODER1_0|GPIO_MODER_MODER4_0|GPIO_MODER_MODER6_0|GPIO_MODER_MODER7_0|GPIO_MODER_MODER5_0|GPIO_MODER_MODER9_0;
    GPIOB->MODER&=~(GPIO_MODER_MODER0|GPIO_MODER_MODER6|GPIO_MODER_MODER8|GPIO_MODER_MODER9);//PB6, PB8 E PB9 SAIDAS
	GPIOB->MODER|=(GPIO_MODER_MODER0_0|GPIO_MODER_MODER6_0|GPIO_MODER_MODER8_0|GPIO_MODER_MODER9_0);
    GPIOC->MODER&=~(GPIO_MODER_MODER0|GPIO_MODER_MODER1|GPIO_MODER_MODER7);//PC0, PC1 E PC7 SAIDAS
	GPIOC->MODER|=(GPIO_MODER_MODER0_0|GPIO_MODER_MODER1_0|GPIO_MODER_MODER7_0);    
    //CONFIGURANDO TIMERS//
    TIM10->PSC = 
    TIM10->ARR = 
    TIM10->CR1 = 0x1;
	TIM11->PSC = 
    TIM11->ARR = 
    TIM11->CR1 = 0x1;
    //VARIAVEIS//
    uint16_t catodo[] = {1110,1101,1011,0111};//Isso vai assegurar que apenas um catodo estará em nivel baixo ao mesmo tempo
    uint16_t msg[] = {num0,num1,num2,num3,num4,num5,num6,num7,num8,num9};//Por enquanto, 0 a 9, para testes













}