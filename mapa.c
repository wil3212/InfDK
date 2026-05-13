#include <stdio.h>
#include <stdlib.h>
#include "mapa.h"

char* alocaMatriz(void) {
    // Aloca memória para armazenar o mapa como uma matriz unidimensional
    return (char*) malloc(sizeof(char) * NCOL * NLIN);
}

char* carregaMapa(mario* player) {
    // Variável temporária para cada caractere lido do arquivo
    char tmpChar;

    // Aloca a matriz do mapa na memória
    char* matrix = alocaMatriz();

    // Inicializa a matriz com espaços em branco
    for (int i = 0; i < NCOL * NLIN; i++) {
        matrix[i] = ' ';
    }

    // Abre o arquivo de mapa para leitura
    FILE* maps = fopen("mapa.txt", "r");
    if (maps == NULL) {
        printf("Error: Could not open file.\n");
        free(matrix); // Libera memória se o arquivo não puder ser aberto
        return NULL;
    }

    int row = 0, col = 0; // Contadores de linha e coluna do mapa

    // Lê o arquivo caractere a caractere até o fim do arquivo ou até preencher todas as linhas
    while ((tmpChar = (char) fgetc(maps)) != EOF && row < NLIN) {
        // Ignora quebras de linha e retornos de carro
        if (tmpChar == '\n' || tmpChar == '\r') {
            if (col > 0) {
                row++; // Avança para a próxima linha
                col = 0; // Reinicia a coluna
            }
            continue;
        }

        // Apenas processa caracteres dentro do limite de colunas definido
        if (col < NCOL) {
            int idx = row * NCOL + col; // Converte coordenada 2D para índice 1D

            if (tmpChar == 'P') {
                // Registra a posição inicial do jogador
                player->pos[0] = (float) row;
                player->pos[1] = (float) col;
                matrix[idx] = ' '; // Substitui 'P' por espaço no mapa
            } else {
                matrix[idx] = tmpChar; // Guarda o caractere do mapa na matriz
            }

            col++; // Avança para a próxima coluna
        }
    }

    fclose(maps); // Fecha o arquivo após a leitura
    return matrix; // Retorna a matriz do mapa preenchida
}

bool isSolid(char block) {
    // Retorna true se o bloco for um tipo sólido que bloqueia o jogador
    return (block == 'F' || block == 'Z' || block == 'S' || block == 'H' || block == 'D');
}

void printaMatriz(char* matrix) {
    // Imprime a matriz do mapa sem mostrar o jogador
    for (int i = 0; i < NLIN; i++) {
        for (int j = 0; j < NCOL; j++) {
            printf("%c", matrix[i * NCOL + j]);
        }
        printf("\n");
    }
}

void printaMatriz2(char* matrix, const mario player) {
    // Imprime a matriz do mapa mostrando a posição atual do jogador
    for (int i = 0; i < NLIN; i++) {
        for (int j = 0; j < NCOL; j++) {
            if (player.pos[0] == i && player.pos[1] == j) {
                printf("P");
            } else {
                printf("%c", matrix[i * NCOL + j]);
            }
        }
        printf("\n");
    }
}
