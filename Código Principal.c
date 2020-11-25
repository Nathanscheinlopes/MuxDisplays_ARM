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

uint16_t pc7,pb6,pb3,pb4,pa5;
//int jj=-1;


//void AlternarCatodo(uint16_t* catodo);
//void PassosDisplay();
void LigarDisplay(uint16_t* msg, uint16_t aa);

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
    TIM10->PSC = 999;
    TIM10->ARR = 9999;
    TIM10->CR1 = 0x1;
	TIM11->PSC = 7;
    TIM11->ARR = 3999;
    TIM11->CR1 = 0x1;
    //VARIAVEIS//
    uint16_t catodo[] = {0b1111110,0b1111101,0b1101111,0b0111111};//Isso vai assegurar que apenas um catodo estará em nivel baixo ao mesmo tempo
    //uint16_t msg[] = {num0,num0,num1,num2,num3,num4,num5,num6,num7,num8,num9};//Por enquanto, 0 a 9, para testes
    //uint16_t msg[] = {esp,e,h,esp,u,s,esp,g,u,r,i};
	uint16_t msg[] = {esp,n,o,z,esp,e,esp,o,esp,j,o,a,o,esp};
    uint16_t kat,ii=0,duni,ddez,dcem,dmil;
    duni=ddez=dcem=dmil=0;
    uint16_t pc7,pb6,pb3,pb4,pa5,aa;
    int jj=-1;
    while(1)
    {
        //AlternarCatodo(catodo);
        //PassosDisplay(ii);
        //LigarDisplay(ii,msg);

    	if(TIM11->SR & TIM_SR_UIF)
    	    {
    	        TIM11->SR = ~TIM_SR_UIF;
    	        if (++jj == 4)
    	            {
    	                jj =0;
    	            }
    	        GPIOA->ODR = catodo[jj];

    	        switch(jj)
    	        {
    	        case 0:
    	       	aa=duni;
				LigarDisplay(msg,aa);
    	        break;

    	        case 1:
    	        aa=ddez;
    	        LigarDisplay(msg,aa);
    	        break;

    	        case 2:
    	        aa=dcem;
    	        LigarDisplay(msg,aa);
    	        break;

    	        case 3:
    	        aa=dmil;
    	        LigarDisplay(msg,aa);
    	        break;
    	        }


    	    }
            if(TIM10->SR & TIM_SR_UIF)
            {
            TIM10->SR = ~TIM_SR_UIF;

            dmil = dcem;
            dcem = ddez;
            ddez = duni;
            duni = ii;
            ii++;
            if(ii==14)
            {
            	ii=1;
            }
            }
    }
}

void LigarDisplay(uint16_t* msg,uint16_t aa)
{
        pc7 = msg[aa] & 0b1000000;
        pc7 = pc7 << 1;//pc7 como pc6
        pb6 = msg[aa] & 0b0100000;
        pb6 = pb6 << 1;//pb6 como pb5
        pb3 = msg[aa] & 0b0000100;
        pb3 = pb3 << 6;//pb8 como pb3
        pb4 = msg[aa] & 0b0001000;
        pb4 = pb4 << 6;//pb9 como pb4
        pa5 = msg[aa] & 0b0010000;
        pa5 = pa5 << 3;//pa7 como pa5
        GPIOA->ODR |= pa5;
        GPIOB->ODR = pb3 | pb4 | pb6;
        GPIOC->ODR = ((msg[aa] & (GPIO_ODR_ODR_0 | GPIO_ODR_ODR_1) | pc7));
}
