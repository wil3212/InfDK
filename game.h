#ifndef GAME_H
#define GAME_H

#include <raylib.h>
#include <stdbool.h>

#define NCOL 31
#define NLIN 31

#define POS 20
#define TAM 20

#define SPEED 0.20f
#define GRAVITY 0.02f
#define JUMP_FORCE -0.45f
#define D_JUMP_FORCE -0.40f

// A estrutura mario representa o jogador no jogo, armazenando sua posição atual em coordenadas de ponto flutuante (pos)
typedef struct player {
    float pos[2];
    int intPos[8];
    float verticalV;
    int jumpCount;
    bool canJump;
} mario;

// Declaração das funções utilizadas no jogo, incluindo funções para manipulação do mapa, renderização e lógica do jogador.
char* alocaMatriz(void);
char* carregaMapa(mario* player, int fase);
void printaMatriz(char* matrix);
void printaMatriz2(char* matrix, const mario player);
void drawMatrix(char* matrix, const mario player);
void calculaCantosInt(mario* player);
bool isSolid(char block);

#endif // GAME_H
