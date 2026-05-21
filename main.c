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
#include "menu.h"


int main(void) {
    // Inicializa a estrutura do jogador
    mario structThing = {0};
    mario* player1 = &structThing;
    int faseAtual = 1;  // Indica qual a fase atual, 1, 2, 3...
    int gameMode = 0;   // 0 caso estejamos no Menu, 1 caso estejamos jogando, 2 caso acessando scores...

    menuOptions* menu = initMenu(); //Inicializa o menu

    // Carrega o mapa do arquivo
    char* matrix = carregaMapa(player1,faseAtual);
    if (matrix == NULL) {
        return 1;
    }

    // Imprime a matriz do mapa no console
    printaMatriz(matrix);

    // Cria a janela do jogo com dimensões baseadas no mapa
    InitWindow((NLIN - 1) * TAM, (NCOL - 1) * TAM, "Donkey Kong INF");
    // Define o FPS alvo para 60
    SetTargetFPS(60);

    bool exit = false;
    // Loop principal do jogo
    while (!WindowShouldClose() && !exit) {
      switch (gameMode) {
        case 0:
          if (IsKeyPressed(KEY_SPACE) || IsKeyPressed(KEY_ENTER)) selecionaOpc(menu,&gameMode,&exit);
          if (menu->selectedOption < NOPTIONS-1)
            if (IsKeyPressed(KEY_S) || IsKeyPressed(KEY_DOWN)) menu->selectedOption++;
          if (menu->selectedOption > 0)
            if (IsKeyPressed(KEY_W) || IsKeyPressed(KEY_UP)) menu->selectedOption--;
          BeginDrawing();
              ClearBackground(BLACK);
              menuDraw(0,menu);
          EndDrawing();
          break;
        case 1: // --> Jogo rodando
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
              }
             else if (player1->jumpCount == 1) {
                 player1->verticalV = D_JUMP_FORCE;
                 player1->jumpCount = 2;
             }
          }

         // Checa se player está na porta
         if (matrix[(int)(player1->pos[0])*NCOL+(int)(player1->pos[1])] == 'F') {
           gameMode = 3;
           printf("encostou no F\nfase atual: %d\n",faseAtual);
         }

         
          // === RENDERIZAÇÃO ===
          // Inicia o frame de desenho
          BeginDrawing();
                          // Limpa a tela com cor preta
              ClearBackground(BLACK);
              // Desenha o mapa e o jogador
              drawMatrix(matrix, *player1);
          EndDrawing();
          break;
        case 3:
          if (IsKeyPressed(KEY_SPACE) || IsKeyPressed(KEY_W)) {
            faseAtual++;
            matrix = carregaMapa(player1,faseAtual);
            if (matrix == NULL) {
                return 1;
            }
            gameMode = 1;
          }
          BeginDrawing();
              ClearBackground(BLACK);
              menuDraw(1,menu);
          EndDrawing();
          break;
      }
    }

    // Libera a memória alocada para a matriz
    free(matrix);
    // Fecha a janela do jogo
    CloseWindow();
    return 0;
}
