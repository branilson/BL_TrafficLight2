#include"mbed.h"

//classes do MBED
InterruptIn button(p5);
DigitalOut red_led(p6);
DigitalOut yellow_led(p7);
DigitalOut green_led(p8);
Ticker tick;
Timeout timeout;

//variaveis
enum Estados
{
RED,YELLOW,GREEN,ALERT,OFF
};
Estados atual = RED;
Estados anterior = RED; //estado anterior, comecando no vermelho
float press = 0;
bool isPressed = false;

//funcoes
void clear();//limpar os buffers para desativar leds, mudar o timeoutr e o ticker e mudar o estado
void ir_vermelho();//ir para o estado vermelho e preparar para ir ao verde
void ir_amarelo();//ir para o estado amarelo e preparar para ir ao vermelho
void ir_verde();//ir para o estado verde e preparar para ir ao amarelo
void ficar_off();//ir para o estado OFF
void pisca_amarelo(); //Alternar a led amarela
void ficar_alerta(); //Ir para o estado de alerta, na qual a LED amarela pisca
void voltar_anterior(); //Voltar ao estado anterior;
void btn_begin(); //Funcao para quando o botao for apertado pela primeira vez
void btn_end();//funcao para quando o botao for solto
int main()
{
    ir_vermelho();//comecar no estado vermelho
    button.rise(&btn_begin); //Esperar o botao ser pressionado
    button.fall(&btn_end); //esperar o botao ser solto
    while (1)
    {
        if(isPressed)
        {
            press += 0.1; //iterando o tempo do botao caso ele esteja pressionado;
        }
        wait_ms(1);
    }
    

    return 0;
}


void clear()
{
    anterior = atual;
    red_led = 0;
    yellow_led = 0;
    green_led = 0;
    timeout.detach();
    tick.detach();
}
void ir_vermelho()//ir para o estado vermelho e preparar para ir ao verde
{
    clear();
    red_led = 1;
    atual = RED;
    timeout.attach(&ir_verde, 10);
    printf("Estado RED \n");
}    
void ir_amarelo() //ir para o estado amarelo e preparar para ir ao vermelho
{
    clear();
    yellow_led = 1;
    atual = YELLOW;
    timeout.attach(&ir_vermelho, 4);
    printf("Estado YELLOW \n");
}
void ir_verde()//ir para o estado verde e preparar para ir ao amarelo
{
    clear();
    green_led = 1;
    atual = GREEN;
    timeout.attach(&ir_amarelo, 20);
    printf("Estado GREEN \n");
}
void ficar_off()//ir para o estado OFF
{
    clear();
    atual = OFF;
    printf("Estado OFF \n");
}
void pisca_amarelo() //Alternar a led amarela
{
    yellow_led = !yellow_led;
}
void ficar_alerta()//Ir para o estado de alerta, na qual a LED amarela pisca
{
    clear();
    tick.attach(&pisca_amarelo, 0.5);
    atual = atual;
    printf("Estado ALERT \n");
}
void voltar_anterior()//Voltar ao estado anterior;
{
    clear();
    switch (anterior)
    {
    case RED:
        ir_vermelho();
        break;
    case YELLOW:
        ir_amarelo();
        break;
    case GREEN:
        ir_verde();
        break;
    case ALERT:
        ficar_alerta();
        break;        
    default:
        ficar_off();
        break;
    }
}
void btn_begin() //Funcao para quando o botao for apertado pela primeira vez
{
    press = 0;
    isPressed = true;
}
void btn_end() //funcao para quando o botao for solto
{
    printf("o botao ficou ativo por %f \n", press);
    if(press <= 3)
    {
        if(atual == GREEN)
        {
            ir_amarelo();
        }
    }else if (press <=10)
    {
        if(atual != ALERT)
        {
            ficar_alerta();
        }else
        {
            voltar_anterior();
        }
    }else
    {
        if(atual != OFF)
        {
            ficar_off();
        }else
        {
            ir_vermelho();
        }
    }
    isPressed = false;
}
























