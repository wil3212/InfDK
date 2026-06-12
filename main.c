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

typedef struct {
  int score;
  int kills;
  int secrets;
  char name[40];
} player_score;

void calculaCantosInt2(base* entity) {
    entity->intPos[0] = (int) entity->pos[0];
    entity->intPos[1] = (int) entity->pos[1];

  // As próximas linhas calculam as posições inteiras dos cantos do jogador, considerando que o jogador ocupa um espaço de 2x2 blocos no mapa.
    entity->intPos[2] = (int) entity->pos[0];
    entity->intPos[3] = (int) (entity->pos[1] + 1);

    entity->intPos[4] = (int) (entity->pos[0] + 1);
    entity->intPos[5] = (int) entity->pos[1];

    entity->intPos[6] = (int) (entity->pos[0] + 1);
    entity->intPos[7] = (int) (entity->pos[1] + 1);
}
void colisionCheck(base *entity, int direction, char* matrix) {
  switch (direction) {
    case 0:
      // Colisão com o Teto
      if (entity->verticalV < 0) {
          calculaCantosInt2(entity);
          if (isSolid(matrix[entity->intPos[0] * NCOL + entity->intPos[1]]) ||
              isSolid(matrix[entity->intPos[2] * NCOL + entity->intPos[3]])) {
                entity->pos[0] = (float)(entity->intPos[0] + 1);
                entity->verticalV = 0;
          }
      }
      else
        entity->moved[0] = 0;
      break;
    case 1: // A
      int rowTop = (int)(entity->pos[0] + 0.1f);
      int rowBottom = (int)(entity->pos[0] + 0.9f);
      if (isSolid(matrix[rowTop * NCOL + (int)(entity->pos[1])]) ||
          isSolid(matrix[rowBottom * NCOL + (int)(entity->pos[1])])) {
          entity->pos[1] = (float)((int)(entity->pos[1]) + 1.0f);
      }
      entity->moved[1] = 0;
      break;
    case 2:
      if (entity->verticalV >= 0) {
          calculaCantosInt2(entity);
          if (isSolid(matrix[entity->intPos[4] * NCOL + entity->intPos[5]]) ||
              isSolid(matrix[entity->intPos[6] * NCOL + entity->intPos[7]])) {
                entity->pos[0] = (float)entity->intPos[0];
                entity->verticalV = 0;
                entity->grounded = 1;
                entity->jumpCount = 0;
          }
      }
      entity->moved[2] = 0;
      break;
    case 3: // D
      rowTop = (int)(entity->pos[0] + 0.1f);
      rowBottom = (int)(entity->pos[0] + 0.9f);
      if (isSolid(matrix[rowTop * NCOL + (int)(entity->pos[1] + 1.0f)]) ||
          isSolid(matrix[rowBottom * NCOL + (int)(entity->pos[1] + 1.0f)])) {
            entity->pos[1] = (float)((int)(entity->pos[1] + 1.0f) - 1.0f);
      }
      entity->moved[3] = 0;
      break;
  }
}
void colisionCheckMain(base *entity, char* matrix) {
  for (int i=0;i<4;i++)
    if (entity->moved[i])
      colisionCheck(entity, i, matrix);
}


void move(base *entity, char mode) {
  switch(mode) {
    case 'A':
      entity->pos[1] -= SPEED * entity->speedFactor;
      entity->moved[1] = 1;
      break;
    case 'D':
      entity->pos[1] += SPEED * entity->speedFactor;
      entity->moved[3] = 1;
      break;
    case 'W':
      // === 4. LÓGICA DE PULO (Melhorada) ===
      if (entity->jumpCount == 0 && entity->grounded) {
          entity->verticalV = JUMP_FORCE;
          entity->jumpCount = 1;
      }
      else if (entity->jumpCount == 1) {
          entity->verticalV = D_JUMP_FORCE;
          entity->jumpCount = 2;
      }
      entity->moved[0] = 1;
      break;
    case 'S':
      entity->verticalV += GRAVITY;
      entity->pos[0] += entity->verticalV;
      entity->moved[2] = 1;
      break;
  }
}


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
          if (IsKeyPressed(KEY_P) || IsKeyPressed(KEY_TAB)) gameMode = 4;

          // mechhe os bixos!
          for (int i=0;i<entidades->nFlames;i++) {
            if (entidades->flames[i]->isRight)
              move((base*)entidades->flames[i], 'D');
            else
              move((base*)entidades->flames[i], 'A');
          }

          // === 1. MOVIMENTAÇÃO HORIZONTAL (Apenas altera o float, colisão vem depois)
          if (IsKeyDown(KEY_D)) move((base*)entidades->player,'D');
          if (IsKeyDown(KEY_A)) move((base*)entidades->player,'A');


          if (IsKeyPressed(KEY_SPACE) || IsKeyPressed(KEY_W))
            move((base*)entidades->player, 'W');

          // gravity for all
          for (int i=0;i<entidades->nFlames;i++)
            move((base*)entidades->flames[i], 'S');
          move((base*)entidades->player, 'S');

          for (int i=0;i<entidades->nFlames;i++)
            colisionCheckMain((base*)entidades->flames[i], matrix);
        colisionCheckMain((base*)entidades->player, matrix);


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
