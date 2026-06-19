#include <raylib.h>
#include "render.h"
/* Função responsável por desenhar o mapa e o jogador na tela */
void drawMatrix(char* matrix, entities* entidades) {
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
                    DrawRectangle(j * POS, i * POS, TAM/4, TAM, YELLOW);
                    DrawRectangle((3*TAM/4) + j * POS, i * POS, TAM/4, TAM, YELLOW);
                    DrawRectangle(j * POS, i * POS, TAM, TAM/4, YELLOW);
                    break;
                case 'H': // Se o caractere for 'H', desenha um retângulo amarelo na posição correspondente, usando as constantes POS e TAM para determinar as coordenadas e o tamanho do retângulo
                    DrawRectangle(j * POS, i * POS, TAM/4, TAM, YELLOW);
                    DrawRectangle((3*TAM/4) + j * POS, i * POS, TAM/4, TAM, YELLOW);
                    DrawRectangle(j * POS, i * POS, TAM, TAM/4, YELLOW);
                    break;
                case 't': // Se o caractere for 'D', desenha um retângulo laranja na posição correspondente, usando as constantes POS e TAM para determinar as coordenadas e o tamanho do retângulo
                    DrawRectangle(j * POS, i * POS, TAM/4, TAM, YELLOW);
                    DrawRectangle((3*TAM/4) + j * POS, i * POS, TAM/4, TAM, YELLOW);
                    DrawRectangle(j * POS, i * POS, TAM, TAM/4, YELLOW);
                    break;
                default:
                    break;
            }
        }
    }

    DrawRectangle(entidades->player->pos[1] * POS, entidades->player->pos[0] * POS, TAM, TAM, RED); // Desenha o jogador como um retângulo vermelho na posição correspondente, usando as coordenadas armazenadas em entidades->player->pos e as constantes POS e TAM para determinar as coordenadas e o tamanho do retângulo

    for (int i=0;i<entidades->nFlames;i++) {
      //DrawRectangle(entidades->flames[i]->pos[1] * POS, entidades->flames[i]->pos[0] * POS, TAM, TAM, BROWN);
      //
      Vector2 a;
      Vector2 b;
      Vector2 c;
      a = (Vector2){entidades->flames[i]->pos[1] * POS, entidades->flames[i]->pos[0] * POS};
      b = (Vector2){entidades->flames[i]->pos[1] * POS + TAM, entidades->flames[i]->pos[0] * POS + (TAM/2)};
      c = (Vector2){entidades->flames[i]->pos[1] * POS, entidades->flames[i]->pos[0] * POS + TAM};
      DrawTriangle(b,a,c, BROWN);
    }
}
