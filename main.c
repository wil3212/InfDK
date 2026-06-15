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
#include <math.h>


typedef struct {
  int score;
  int kills;
  int secrets;
  int currentStage;
  char name[40];
} player_score;


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

    int counTimer = 0;

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


          if (IsKeyPressed(KEY_SPACE))
            move((base*)entidades->player, 'W');

          if (IsKeyDown(KEY_W))
            move((base*)entidades->player, 'C');
          if (IsKeyDown(KEY_S))
            move((base*)entidades->player, 'c');

          //printf("isCLimbing? %d\n",entidades->player->isClimbing);
          //printf("bloco %c \n",matrix[entidades->player->intPos[0] * NCOL + entidades->player->intPos[1]]);
          //printf("hor V: %.2f \n",entidades->player->horizontalV);
          printf("pos X: %.2f \n",entidades->player->pos[1]);
          printf("pos Y: %.2f \n",entidades->player->pos[0]);

          // gravity for all
         for (int i=0;i<entidades->nFlames;i++)
            move((base*)entidades->flames[i], 'G');
         move((base*)entidades->player, 'G');

         for (int i=0;i<entidades->nFlames;i++)
            colisionCheckMain((base*)entidades->flames[i], matrix);
         colisionCheckMain((base*)entidades->player, matrix);


         checkDeadList(entidades);                                      //    Previne seg fault por entidades ganhando
         for (int i=0;i<entidades->nFlames;i++) {                       //    pos muito elevadas, quando caem do mapa.
           if (!entidades->flames[i]->alive) {                          //
             entidades->flames[i]->pos[0] = 400;                        //
             entidades->flames[i]->pos[1] = 400;                        //
             entidades->flames[i]->verticalV = 0.0;                     //
             entidades->flames[i]->horizontalV = 0.0; //slippery ground?//
           }
         }
         //printf("pos enem: %f %f \n",entidades->flames[4]->pos[0],entidades->flames[0]->pos[1]);

         if (hitBoxCheckListl(entidades)) {
           printf("morreu!!");
           counTimer = 0;
           gameMode = 2;
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
        case 2:
          if (counTimer > 150) {
            matrix = carregaMapa(faseAtual);
            if (matrix == NULL) {
                return 1;
            }
            entidades = getEntities(matrix);
            gameMode = 1;
          }
          printf("counter: %d\n",counTimer);
          counTimer ++;
          BeginDrawing();
              ClearBackground(BLACK);
              menuDraw(2,menu);
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
