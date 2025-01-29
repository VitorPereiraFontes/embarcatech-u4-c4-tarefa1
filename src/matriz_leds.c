#include <stdio.h>
#include <stdlib.h>
#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/clocks.h"
#include "matriz_leds.h"

// Arquivo .pio para controle da matriz
#include "pio_matrix.pio.h"

// Definindo a cor padrão dos leds
#define WHITE_COLOR (RGB_color){1.0, 1.0, 1.0}
#define OFF (RGB_color){0.0, 0.0, 0.0}

// Definindo a estética dos números
Led_matrix_layout zero_layout = {
    {WHITE_COLOR,WHITE_COLOR,WHITE_COLOR,WHITE_COLOR,WHITE_COLOR},
    {WHITE_COLOR,OFF        ,OFF        ,OFF        ,WHITE_COLOR},
    {WHITE_COLOR,OFF        ,OFF        ,OFF        ,WHITE_COLOR},
    {WHITE_COLOR,OFF        ,OFF        ,OFF        ,WHITE_COLOR},
    {WHITE_COLOR,WHITE_COLOR,WHITE_COLOR,WHITE_COLOR,WHITE_COLOR},
};

Led_matrix_layout one_layout = {
    {OFF,OFF,OFF,OFF,WHITE_COLOR},
    {OFF,OFF,OFF,OFF,WHITE_COLOR},
    {OFF,OFF,OFF,OFF,WHITE_COLOR},
    {OFF,OFF,OFF,OFF,WHITE_COLOR},
    {OFF,OFF,OFF,OFF,WHITE_COLOR},
};

Led_matrix_layout two_layout = {
    {WHITE_COLOR,WHITE_COLOR,WHITE_COLOR,WHITE_COLOR,WHITE_COLOR},
    {OFF        ,OFF        ,OFF        ,OFF        ,WHITE_COLOR},
    {WHITE_COLOR,WHITE_COLOR,WHITE_COLOR,WHITE_COLOR,WHITE_COLOR},
    {WHITE_COLOR,OFF        ,OFF        ,OFF        ,OFF        },
    {WHITE_COLOR,WHITE_COLOR,WHITE_COLOR,WHITE_COLOR,WHITE_COLOR},
};

Led_matrix_layout three_layout = {
    {WHITE_COLOR,WHITE_COLOR,WHITE_COLOR,WHITE_COLOR,WHITE_COLOR},
    {OFF        ,OFF        ,OFF        ,OFF        ,WHITE_COLOR},
    {WHITE_COLOR,WHITE_COLOR,WHITE_COLOR,WHITE_COLOR,WHITE_COLOR},
    {OFF        ,OFF        ,OFF        ,OFF        ,WHITE_COLOR},
    {WHITE_COLOR,WHITE_COLOR,WHITE_COLOR,WHITE_COLOR,WHITE_COLOR},
};

Led_matrix_layout four_layout = {
    {WHITE_COLOR,OFF        ,OFF        ,OFF        ,WHITE_COLOR},
    {WHITE_COLOR,OFF        ,OFF        ,OFF        ,WHITE_COLOR},
    {WHITE_COLOR,WHITE_COLOR,WHITE_COLOR,WHITE_COLOR,WHITE_COLOR},
    {OFF        ,OFF        ,OFF        ,OFF        ,WHITE_COLOR},
    {OFF        ,OFF        ,OFF        ,OFF        ,WHITE_COLOR},
};

Led_matrix_layout five_layout = {
    {WHITE_COLOR,WHITE_COLOR,WHITE_COLOR,WHITE_COLOR,WHITE_COLOR},
    {WHITE_COLOR,OFF        ,OFF        ,OFF        ,OFF        },
    {WHITE_COLOR,WHITE_COLOR,WHITE_COLOR,WHITE_COLOR,WHITE_COLOR},
    {OFF        ,OFF        ,OFF        ,OFF        ,WHITE_COLOR},
    {WHITE_COLOR,WHITE_COLOR,WHITE_COLOR,WHITE_COLOR,WHITE_COLOR},
};

Led_matrix_layout six_layout = {
    {WHITE_COLOR,WHITE_COLOR,WHITE_COLOR,WHITE_COLOR,WHITE_COLOR},
    {WHITE_COLOR,OFF        ,OFF        ,OFF        ,OFF        },
    {WHITE_COLOR,WHITE_COLOR,WHITE_COLOR,WHITE_COLOR,WHITE_COLOR},
    {WHITE_COLOR,OFF        ,OFF        ,OFF        ,WHITE_COLOR},
    {WHITE_COLOR,WHITE_COLOR,WHITE_COLOR,WHITE_COLOR,WHITE_COLOR},
};

Led_matrix_layout seven_layout = {
    {WHITE_COLOR,WHITE_COLOR,WHITE_COLOR,WHITE_COLOR,WHITE_COLOR},
    {WHITE_COLOR,OFF        ,OFF        ,OFF        ,WHITE_COLOR},
    {WHITE_COLOR,OFF        ,OFF        ,OFF        ,WHITE_COLOR},
    {OFF        ,OFF        ,OFF        ,OFF        ,WHITE_COLOR},
    {OFF        ,OFF        ,OFF        ,OFF        ,WHITE_COLOR},
};

Led_matrix_layout eight_layout = {
    {WHITE_COLOR,WHITE_COLOR,WHITE_COLOR,WHITE_COLOR,WHITE_COLOR},
    {WHITE_COLOR,OFF        ,OFF        ,OFF        ,WHITE_COLOR},
    {WHITE_COLOR,WHITE_COLOR,WHITE_COLOR,WHITE_COLOR,WHITE_COLOR},
    {WHITE_COLOR,OFF        ,OFF        ,OFF        ,WHITE_COLOR},
    {WHITE_COLOR,WHITE_COLOR,WHITE_COLOR,WHITE_COLOR,WHITE_COLOR},
};

Led_matrix_layout nine_layout = {
    {WHITE_COLOR,WHITE_COLOR,WHITE_COLOR,WHITE_COLOR,WHITE_COLOR},
    {WHITE_COLOR,OFF        ,OFF        ,OFF        ,WHITE_COLOR},
    {WHITE_COLOR,WHITE_COLOR,WHITE_COLOR,WHITE_COLOR,WHITE_COLOR},
    {OFF        ,OFF        ,OFF        ,OFF        ,WHITE_COLOR},
    {WHITE_COLOR,WHITE_COLOR,WHITE_COLOR,WHITE_COLOR,WHITE_COLOR},
};

Led_matrix_layout* get_number_layout(int number){
    switch (number)
    {
    case 1:
        return &one_layout;
    break;

    case 2:
        return &two_layout;
    break;

    case 3:
        return &three_layout;
    break;

    case 4:
        return &four_layout;
    break;

    case 5:
        return &five_layout;
    break;

    case 6:
        return &six_layout;
    break;

    case 7:
        return &seven_layout;
    break;

    case 8:
        return &eight_layout;
    break;

    case 9:
        return &nine_layout;
    break;
    
    default:
    return &zero_layout;
        break;
    }
}

// Gera o binário que controla a cor de cada célula de LED na matriz
uint32_t generate_color_binary(double red, double green, double blue)
{
  unsigned char RED, GREEN, BLUE;
  RED = red * 255.0;
  GREEN = green * 255.0;
  BLUE = blue * 255.0;
  return (GREEN << 24) | (RED << 16) | (BLUE << 8);
}

// Inicializa a matriz de LED's
uint initialize_matrix(PIO pio, uint led_matrix_pin){
    bool ok;

    // Define o clock para 128 MHz, facilitando a divisão pelo clock
    ok = set_sys_clock_khz(128000, false);

    // Inicializa todos os códigos stdio padrão que estão ligados ao binário.
    stdio_init_all();

    printf("iniciando a transmissão PIO");
    if (ok) printf("clock set to %ld\n", clock_get_hz(clk_sys));

    //configurações da PIO
    uint offset = pio_add_program(pio, &pio_matrix_program);
    uint sm = pio_claim_unused_sm(pio, true);
    pio_matrix_program_init(pio, sm, offset, led_matrix_pin);

    return sm;
}

// Exibe na matriz o layout que o usuário definiu
void draw_on_matrix(Led_matrix_layout layout, PIO pio, uint sm){
    for (int line_counter = 4; line_counter >= 0; line_counter--){
        /*
            Realiza a atribuição das cores nos LED's da esquerda para a direita nas linhas 1 e 2 da matriz
            (contando de cima para baixo na placa) e da direita para a esquerda nas demais linhas. Isso garante
            que cada posição na matriz layout (da esquerda para a direita e de cima para baixo) corresponde a
            exata mesma posição física na matriz de LED's presente na placa.
        */
        if(line_counter % 2){
            for (int column_counter = 0; column_counter < 5; column_counter ++){
                uint32_t valor_cor_binario = generate_color_binary(
                    layout[line_counter][column_counter].red,
                    layout[line_counter][column_counter].green,
                    layout[line_counter][column_counter].blue
                );

                pio_sm_put_blocking(pio, sm, valor_cor_binario);
            }
        }else{
            for (int column_counter = 4; column_counter >= 0; column_counter --){
                uint32_t valor_cor_binario = generate_color_binary(
                    layout[line_counter][column_counter].red,
                    layout[line_counter][column_counter].green,
                    layout[line_counter][column_counter].blue
                );

                pio_sm_put_blocking(pio, sm, valor_cor_binario);
            }
        }
    }
}

/* 
    Gera uma estrutura que representa uma cor RGB a partir da intensidade de cada componente de cor,
    entre 0.0 e 1.0, a partir do código RGB da cor.
*/
RGB_color get_color_by_RGB_code(int red, int green, int blue){
    RGB_color custom_color = {red/255.0,green/255.0,blue/255.0};

    return custom_color;
}