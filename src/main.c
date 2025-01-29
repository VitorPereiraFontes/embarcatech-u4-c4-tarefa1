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

    while (true) {
        // Piscando o led
        gpio_put(RED_LED_PIN,true);
        sleep_ms(50);
        gpio_put(RED_LED_PIN,false);
        sleep_ms(150);
    }
}
