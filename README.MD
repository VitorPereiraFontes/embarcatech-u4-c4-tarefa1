# Utilização de interrupções programadas

## Descrição

Este repositório contém os códigos correspondentes aos requisitos solicitados pela tarefa 1 do capítulo 4 da unidade 4 do programa de capacitação em sistemas embarcados (Embarcatech). A tarefa em questão busca incitar a utilização de interrupções programadas no microcontrolador RP2040, da placa BitDogLab, para manipular uma matriz de LED's endereçáveis WS2812.

## Requerimentos

O presente projeto deverá atender aos seguintes requerimentos:

* O LED vermelho do LED RGB deve piscar continuamente 5 vezes por segundo.
* O botão A deve incrementar o número exibido na matriz de LEDs cada vez que for pressionado.
* O botão B deve decrementar o número exibido na matriz de LEDs cada vez que for pressionado.
* Os LEDs WS2812 devem ser usados para criar efeitos visuais representando números de 0 a 9.

## Demonstração

Segue o link do vídeo de demonstração do projeto na placa BitDogLab.

[Vídeo](https://youtu.be/QCehlGIUWpQ)

## Instruções

Para que o projeto funcione em sua máquina, é necessário ter a extensão Raspberry Pi Pico Project instalada no VSCode.

1. Realize o clone do repositório.
2. Importe o repositório como um projeto através da extensão Raspberry Pi Pico Project.
3. Compile o projeto.
4. Caso esteja usando a placa BitDogLab ou a Raspberry Pi Pico W, faça o upload do arquivo uf2, encontrado na pasta `build` após o processo de compilação, para a placa. Caso deseje simular, abra o arquivo `diagram.json` (É necessário ter a extensão do Wokwi instalada).
