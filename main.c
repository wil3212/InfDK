/*
-> Por hora temos a renderização basica do mapa, com um player movimentavel.
* Implementar a logica que faz o player (e futuros inimigos não atravessarem plataformas)
  . Isso vai envolver manter o jogo a um certo FPS, e então checar se o que há abaixo do jogador é uma plataforma.
  . Futuramente, será necessário deixar a posição do jogador como float 


*/
#include <stdio.h>
#include <stdlib.h>
#include <raylib.h>
#include "game.h"
#include "mapa.h"
#include "render.h"
#include "player.h"


int main(void) {
    // Inicializa a estrutura do jogador
    mario structThing = {0};
    mario* player1 = &structThing;
    
    // Carrega o mapa do arquivo
    char* matrix = carregaMapa(player1);
    if (matrix == NULL) {
        return 1;
    }

    // Imprime a matriz do mapa no console
    printaMatriz(matrix);

    // Cria a janela do jogo com dimensões baseadas no mapa
    InitWindow((NLIN - 1) * TAM, (NCOL - 1) * TAM, "Donkey Kong INF");
    // Define o FPS alvo para 60
    SetTargetFPS(60);

    // Loop principal do jogo
    while (!WindowShouldClose()) {
        // === 1. MOVIMENTAÇÃO HORIZONTAL (Apenas altera o float, colisão vem depois) ===
        if (IsKeyDown(KEY_D)) player1->pos[1] += SPEED;
        if (IsKeyDown(KEY_A)) player1->pos[1] -= SPEED;

        // === 2. COLISÃO HORIZONTAL (O SEGREDO ESTÁ AQUI) ===
        calculaCantosInt(player1);
        // Checamos a colisão lateral usando um pequeno offset (0.1) para não bater no chão
        // Em vez de usar intPos[4] e [5] (que estão no chão), checamos a altura do "corpo"
        int rowTop = (int)(player1->pos[0] + 0.1f);
        int rowBottom = (int)(player1->pos[0] + 0.9f); // 0.9 evita tocar a linha do chão

        // Colisão Direita
        if (isSolid(matrix[rowTop * NCOL + (int)(player1->pos[1] + 1.0f)]) ||
            isSolid(matrix[rowBottom * NCOL + (int)(player1->pos[1] + 1.0f)])) {
            player1->pos[1] = (float)((int)(player1->pos[1] + 1.0f) - 1.0f);
        }
        // Colisão Esquerda
        if (isSolid(matrix[rowTop * NCOL + (int)(player1->pos[1])]) ||
            isSolid(matrix[rowBottom * NCOL + (int)(player1->pos[1])])) {
            player1->pos[1] = (float)((int)(player1->pos[1]) + 1.0f);
        }

        // === 3. FÍSICA VERTICAL (Gravidade e Pulo) ===
        player1->verticalV += GRAVITY;
        player1->pos[0] += player1->verticalV;
        calculaCantosInt(player1); // Atualiza para checar o chão

        bool grounded = false;
        // Colisão com o Chão (Apenas se estiver caindo)
        if (player1->verticalV >= 0) {
            if (isSolid(matrix[player1->intPos[4] * NCOL + player1->intPos[5]]) || 
                isSolid(matrix[player1->intPos[6] * NCOL + player1->intPos[7]])) {
                player1->pos[0] = (float)player1->intPos[0];
                player1->verticalV = 0;
                grounded = true;
                player1->jumpCount = 0;
            }
        }

        // Colisão com o Teto
        if (player1->verticalV < 0) {
            if (isSolid(matrix[player1->intPos[0] * NCOL + player1->intPos[1]]) || 
                isSolid(matrix[player1->intPos[2] * NCOL + player1->intPos[3]])) {
                player1->pos[0] = (float)(player1->intPos[0] + 1);
                player1->verticalV = 0;
            }
        }

        // === 4. LÓGICA DE PULO (Melhorada) ===
        if (IsKeyPressed(KEY_SPACE) || IsKeyPressed(KEY_W)) {
            if (grounded) {
                player1->verticalV = JUMP_FORCE;
                player1->jumpCount = 1;
            } else if (player1->jumpCount == 1) {
                player1->verticalV = D_JUMP_FORCE;
                player1->jumpCount = 2;
            }
        }

        // === RENDERIZAÇÃO ===
        // Inicia o frame de desenho
        BeginDrawing();
                        // Limpa a tela com cor preta
            ClearBackground(BLACK);
            // Desenha o mapa e o jogador
            drawMatrix(matrix, *player1);
        EndDrawing();
    }

    // Libera a memória alocada para a matriz
    free(matrix);
    // Fecha a janela do jogo
    CloseWindow();
    return 0;
}


