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


int main() {
    // Inicializa a estrutura do jogador
    int faseAtual = 1;  // Indica qual a fase atual, 1, 2, 3...
    int gameMode = 0;   // 0 caso estejamos no Menu, 1 caso estejamos jogando, 2 caso acessando scores...

    menuOptions* menu = initMenu(); //Inicializa o menu
    menuOptions* menuPausa = initMenuPausa(); //Inicializa o menu

    // Carrega o mapa do arquivo
    char* matrix = carregaMapa(faseAtual);
    if (matrix == NULL) {
        return 1;
    }

    entities* entidades = getEntities(matrix);

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
        case 0:  // Case 0 -> menu inicial (iniciar jogo, ranking, sair...)
          if (IsKeyPressed(KEY_SPACE) || IsKeyPressed(KEY_ENTER)) selecionaOpcMenu(menu,&gameMode,&exit,&matrix,entidades,&faseAtual);
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

          // mechhe os bixos!
          for (int i=0;i<entidades->nFlames;i++) {

            if (entidades->flames[i]->isRight) entidades->flames[i]->pos[1] += SPEED*.6;
            else entidades->flames[i]->pos[1] -= SPEED*.6;

          }



          if (IsKeyPressed(KEY_P) || IsKeyPressed(KEY_TAB)) gameMode = 4;
          // === 1. MOVIMENTAÇÃO HORIZONTAL (Apenas altera o float, colisão vem depois) ===
          if (IsKeyDown(KEY_D)) entidades->player->pos[1] += SPEED;
          if (IsKeyDown(KEY_A)) entidades->player->pos[1] -= SPEED;

          // === 2. COLISÃO HORIZONTAL (O SEGREDO ESTÁ AQUI) ===
          calculaCantosInt(entidades->player);
          // Checamos a colisão lateral usando um pequeno offset (0.1) para não bater no chão
          // Em vez de usar intPos[4] e [5] (que estão no chão), checamos a altura do "corpo"
          int rowTop = (int)(entidades->player->pos[0] + 0.1f);
          int rowBottom = (int)(entidades->player->pos[0] + 0.9f); // 0.9 evita tocar a linha do chão

          // Colisão Direita
          if (isSolid(matrix[rowTop * NCOL + (int)(entidades->player->pos[1] + 1.0f)]) ||
              isSolid(matrix[rowBottom * NCOL + (int)(entidades->player->pos[1] + 1.0f)])) {
              entidades->player->pos[1] = (float)((int)(entidades->player->pos[1] + 1.0f) - 1.0f);
          }
          // Colisão Esquerda
          if (isSolid(matrix[rowTop * NCOL + (int)(entidades->player->pos[1])]) ||
              isSolid(matrix[rowBottom * NCOL + (int)(entidades->player->pos[1])])) {
              entidades->player->pos[1] = (float)((int)(entidades->player->pos[1]) + 1.0f);
          }
          // === 3. FÍSICA VERTICAL (Gravidade e Pulo) ===
          entidades->player->verticalV += GRAVITY;
          entidades->player->pos[0] += entidades->player->verticalV;
          calculaCantosInt(entidades->player); // Atualiza para checar o chão

          bool grounded = false;
          // Colisão com o Chão (Apenas se estiver caindo)
          if (entidades->player->verticalV >= 0) {
              if (isSolid(matrix[entidades->player->intPos[4] * NCOL + entidades->player->intPos[5]]) ||
                  isSolid(matrix[entidades->player->intPos[6] * NCOL + entidades->player->intPos[7]])) {
                  entidades->player->pos[0] = (float)entidades->player->intPos[0];
                  entidades->player->verticalV = 0;
                  grounded = true;
                  entidades->player->jumpCount = 0;
              }
          }

          // Colisão com o Teto
          if (entidades->player->verticalV < 0) {
              if (isSolid(matrix[entidades->player->intPos[0] * NCOL + entidades->player->intPos[1]]) ||
                  isSolid(matrix[entidades->player->intPos[2] * NCOL + entidades->player->intPos[3]])) {
                  entidades->player->pos[0] = (float)(entidades->player->intPos[0] + 1);
                  entidades->player->verticalV = 0;
              }
          }

          // === 4. LÓGICA DE PULO (Melhorada) ===
          if (IsKeyPressed(KEY_SPACE) || IsKeyPressed(KEY_W)) {
              if (grounded) {
                  entidades->player->verticalV = JUMP_FORCE;
                  entidades->player->jumpCount = 1;
              }
             else if (entidades->player->jumpCount == 1) {
                 entidades->player->verticalV = D_JUMP_FORCE;
                 entidades->player->jumpCount = 2;
             }
          }

         // Checa se player está na porta
         if (matrix[(int)(entidades->player->pos[0])*NCOL+(int)(entidades->player->pos[1])] == 'F') {
           gameMode = 3;
           printf("encostou no F\nfase atual: %d\n",faseAtual);
         }

         
          // === RENDERIZAÇÃO ===
          // Inicia o frame de desenho
         //printf("antes de desenhar %d\n",faseAtual);
          BeginDrawing();
                          // Limpa a tela com cor preta
              ClearBackground(BLACK);
              // Desenha o mapa e o jogador
              drawMatrix(matrix, entidades);
              DrawFPS(400,400);
          EndDrawing();
          break;
        case 3: // Tela de "Passou de fase!"
          if (IsKeyPressed(KEY_SPACE) || IsKeyPressed(KEY_W)) {
            faseAtual++;
            matrix = carregaMapa(faseAtual);
            if (matrix == NULL) {
                return 1;
            }
            entidades = getEntities(matrix);
            gameMode = 1;
          }
          BeginDrawing();
              ClearBackground(BLACK);
              menuDraw(1,menu);
          EndDrawing();
          break;
        case 4: // Tela "Pausa"
          if (IsKeyPressed(KEY_P) || IsKeyPressed(KEY_TAB)) gameMode = 1;
          if (IsKeyPressed(KEY_SPACE) || IsKeyPressed(KEY_ENTER)) selecionaOpcPausa(menuPausa,&gameMode,&exit);
          if (menuPausa->selectedOption < NOPTIONS-1)
            if (IsKeyPressed(KEY_S) || IsKeyPressed(KEY_DOWN)) menuPausa->selectedOption++;
          if (menuPausa->selectedOption > 0)
            if (IsKeyPressed(KEY_W) || IsKeyPressed(KEY_UP)) menuPausa->selectedOption--;
          BeginDrawing();
              ClearBackground(BLACK);
              menuDraw(0,menuPausa);
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
