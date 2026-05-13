#include <raylib.h>
#include "render.h"
/* Função responsável por desenhar o mapa e o jogador na tela */
void drawMatrix(char* matrix, const mario player) {
    for (int i = 0; i < NLIN; i++) { // Itera sobre cada linha do mapa, usando a constante NLIN para determinar o número total de linhas a serem processadas
        for (int j = 0; j < NCOL; j++) { // Itera sobre cada coluna do mapa, usando a constante NCOL para determinar o número total de colunas a serem processadas
            switch (matrix[i * NCOL + j]) {  // Acessa o caractere correspondente à posição atual (i, j) no mapa, calculando o índice na matriz unidimensional usando a fórmula i * NCOL + j
               
                case 'F':   // Se o caractere for 'F', desenha um retângulo rosa na posição correspondente, usando as constantes POS e TAM para determinar as coordenadas e o tamanho do retângulo
                    DrawRectangle(j * POS, i * POS, TAM, TAM, PINK);
                    break;
                case 'Z': // Se o caractere for 'Z', desenha um retângulo azul na posição correspondente, usando as constantes POS e TAM para determinar as coordenadas e o tamanho do retângulo
                    DrawRectangle(j * POS, i * POS, TAM, TAM, BLUE);
                    break;
                case 'S': // Se o caractere for 'S', desenha um retângulo verde na posição correspondente, usando as constantes POS e TAM para determinar as coordenadas e o tamanho do retângulo
                    DrawRectangle(j * POS, i * POS, TAM, TAM, GREEN);
                    break;
                case 'H': // Se o caractere for 'H', desenha um retângulo amarelo na posição correspondente, usando as constantes POS e TAM para determinar as coordenadas e o tamanho do retângulo
                    DrawRectangle(j * POS, i * POS, TAM, TAM, YELLOW);
                    break;
                case 'D': // Se o caractere for 'D', desenha um retângulo laranja na posição correspondente, usando as constantes POS e TAM para determinar as coordenadas e o tamanho do retângulo
                    DrawRectangle(j * POS, i * POS, TAM, TAM, ORANGE);
                    break;
                default:
                    break;
            }
        }
    }

    DrawRectangle(player.pos[1] * POS, player.pos[0] * POS, TAM, TAM, RED); // Desenha o jogador como um retângulo vermelho na posição correspondente, usando as coordenadas armazenadas em player.pos e as constantes POS e TAM para determinar as coordenadas e o tamanho do retângulo
    DrawRectangle(player.pos[1] * POS, player.pos[0] * POS + TAM - 1, 4, 4, PINK); // Desenha um pequeno retângulo rosa na parte inferior do retângulo do jogador, usando as coordenadas armazenadas em player.pos e as constantes POS e TAM para determinar as coordenadas e o tamanho do retângulo, criando um efeito visual que destaca a base do jogador
}
