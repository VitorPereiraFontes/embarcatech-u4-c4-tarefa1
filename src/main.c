#include <stdio.h>
#include "pico/stdlib.h"
#include "matriz_leds.h"
#include "hardware/pio.h"
#include "interruption.h"

#define RED_LED_PIN 13
// Pino que realizará a comunicação do microcontrolador com a matriz
#define LED_MATRIX_PIN 7
#define BUTTON_A_PIN 5
#define BUTTON_B_PIN 6
// Define o intervalo de espera para que não ocorra bouncing nos botões (em microssegundos)
#define interval 300000 // 300 ms

// Variáveis para a manipulação da matriz de LED's
PIO pio = pio0;
uint sm;
volatile uint counter = 0; // Armazena a informação do número atual do display

// Variáveis para o controle da interrupção
volatile uint last_time = 0; // Armazena o tempo do último evento (em microssegundos)

// Função que será disparada quando a interrupção for atendida
void buttons_irq_handler(uint gpio, uint32_t events){
    // Obtém o tempo atual em microssegundos
    uint64_t current_time = to_us_since_boot(get_absolute_time());

    // Verifica se já se passaram 100 ms desde o último evento de acionamento do botão
    if (current_time - last_time > interval){
        last_time = current_time;

        int new_counter;

        // Verifica qual botão foi pressionado para incrementar ou decrementar o contador
        switch (gpio){
            case 5:
                printf("Botão A foi pressionado\n");

                if(counter + 1 <= 9){
                    draw_on_matrix(*(get_number_layout(++counter)),pio,sm);
                }
            break;

            case 6:
                printf("Botão B foi pressionado\n");

                if(counter - 1 >= 0){
                    draw_on_matrix(*(get_number_layout(--counter)),pio,sm);
                }
            break;

            default:
            break;
        }
    }
}

int main()
{
    // Inicializa subsistemas de entrada e saída padrão
    stdio_init_all();

    // Configurando a GPIO do LED
    gpio_init(RED_LED_PIN);
    gpio_set_dir(RED_LED_PIN,GPIO_OUT);

    // Inicializando e configurando a matriz de LED's
    sm = initialize_matrix(pio,LED_MATRIX_PIN);

    // Inicializa a matriz de LED's com o número zero
    draw_on_matrix(*(get_number_layout(counter)), pio, sm);

    // Configurando os botões
    initialize_buttons(BUTTON_A_PIN, BUTTON_B_PIN, buttons_irq_handler);

    while (true) {
        // Piscando o led
        gpio_put(RED_LED_PIN,true);
        sleep_ms(50);
        gpio_put(RED_LED_PIN,false);
        sleep_ms(150);
    }
}
