#include <stdio.h>
#include <stdlib.h>
#include "mapa.h"
#include <string.h>

char* alocaMatriz(void) {
    // Aloca memória para armazenar o mapa como uma matriz unidimensional
    return (char*) malloc(sizeof(char) * NCOL * NLIN);
}

char* carregaMapa(int fase) {
    // Variável temporária para cada caractere lido do arquivo
    char tmpChar;
    // Aloca a matriz do mapa na memória
    char* matrix = alocaMatriz();
    // Inicializa a matriz com espaços em branco
    for (int i = 0; i < NCOL * NLIN; i++) {
        matrix[i] = ' ';
    }
    // Forma muito ruim de fazer a string do nome do arquivo de mapaX.txt
    char *nome1 = "mapa";
    char *nome2 = ".txt";
    char mapaNome[10];
    for (int i=0;i<4;i++)
      mapaNome[i] = nome1[i];
    mapaNome[4] = 48+fase;
    for (int i=0;i<5;i++)
      mapaNome[i+5] = nome2[i];

    // Abre o arquivo de mapa para leitura
    FILE* maps = fopen(mapaNome, "r");
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

            /* // Desativei essa parte; A ideia vai ser deixar 'P' guardado na char matriz na sua posição original.
               // A posição no struct player sera fornecida por outra função
               // Futuramente, otimização desse processo consistiria em salvar todos os indexes de posição de player e entidades em uma lista agora (já na leitura do arquivo)
               // Mas atualmente, a função secundária simplesmente varerá toda matriz procurando por 'P'
               // A função em questõa é a getPoss, ficará declarada em game.c

            if (tmpChar == 'P') {
                // Registra a posição inicial do jogador
                player->pos[0] = (float) row;
                player->pos[1] = (float) col;
                matrix[idx] = ' '; // Substitui 'P' por espaço no mapa
                */
           // } else {
            matrix[idx] = tmpChar; // Guarda o caractere do mapa na matriz
           // }
            col++; // Avança para a próxima coluna
        }
    }
    fclose(maps); // Fecha o arquivo após a leitura
    printf("fase atual: %d\n",fase);
    return matrix; // Retorna a matriz do mapa preenchida
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
