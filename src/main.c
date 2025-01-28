#include <stdio.h>
#include "pico/stdlib.h"
#include "matriz_leds.h"
#include "hardware/pio.h"

#define RED_LED_PIN 13

int main()
{
    // Inicializa subsistemas de entrada e saída padrão
    stdio_init_all();

    // Configurando a GPIO do LED
    gpio_init(RED_LED_PIN);
    gpio_set_dir(RED_LED_PIN,GPIO_OUT);

    while (true) {
        // Piscando o led
        gpio_put(RED_LED_PIN,true);
        sleep_ms(50);
        gpio_put(RED_LED_PIN,false);
        sleep_ms(150);
    }
}
