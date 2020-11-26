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
	TIM11->PSC = 16;
    TIM11->ARR = 4999;
    TIM11->CR1 = 0x1;
    //VARIAVEIS//
    uint16_t catodo[] = {0b1111110,0b1111101,0b1101111,0b0111111};//Isso vai assegurar que apenas um catodo estará em nivel baixo ao mesmo tempo
	uint16_t msg[] = {esp,e,d,n,a,l,d,o,esp,p,e,r,e,i,r,a,esp};//aqui é onde entra a mensagem, cada letra deve ser separada por virgula e o espaço é abreviado para esp, para numeros utilizar "numX" 
    uint16_t ii=0,duni,ddez,dcem,dmil,aa;//declarando uma variavel de contagem e as de deslocamento 
    duni=ddez=dcem=dmil=0;//zerando as variaveis para deslocar o simbolo no display para começar vazio 
    uint16_t pc7,pb6,pb3,pb4,pa5;//variaveis utilizadas na função LigarDisplay 
    int jj=-1;//variavel para fazer a leitura do vetor do cátodo
    while(1)
    {
     	if(TIM11->SR & TIM_SR_UIF)
    	    {
    	        TIM11->SR = ~TIM_SR_UIF;
    	        if (++jj == 4)//número de displays presente na multiplexação 
    	            {
    	                jj =0;
    	            }
    	        GPIOA->ODR = catodo[jj];

    	        switch(jj)//esse switch é responsável por alternar e deslocar para a próxima variável o valor escrito no primeiro display, registrando os passos 
    	        {
    	        case 0://display mais a esquerda
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

    	        case 3://display mais a direita
    	        aa=dmil;
    	        LigarDisplay(msg,aa);
    	        break;
    	        }
    	    }
            if(TIM10->SR & TIM_SR_UIF)//quando esse timer de 1s estourar, o display anda um passo 
            {
                TIM10->SR = ~TIM_SR_UIF;
                dmil = dcem;//deslocamento da variavel para ligar o display  
                dcem = ddez;//
                ddez = duni;//
                duni = ii;//atribuição da variavel de contagem ao display mais a direita 
                ii++;
                if(ii==18)//tamanho do vetor com a mensagem a ser exibida
                {
                 	ii=1;//reset tamanho do vetor 
                }
            }
    }
}

void LigarDisplay(uint16_t* msg,uint16_t aa)//Essa função vai ligar o display, fazendo deslocamento de bits para adequar ao barramento do arduino
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
        GPIOA->ODR |= pa5;//liga os segmentos do GPIOA
        GPIOB->ODR = pb3 | pb4 | pb6;//liga os segmentos do GPIOB
        GPIOC->ODR = ((msg[aa] & (GPIO_ODR_ODR_0 | GPIO_ODR_ODR_1) | pc7));//liga os segmentos do GPIOC
}
