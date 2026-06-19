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
#include <time.h>
#include "placar.h"





int main() {

    srand(time(NULL));
    // Inicializa a estrutura do jogador
    int faseAtual = 1;  // Indica qual a fase atual, 1, 2, 3...
    int gameMode = 0;   // 0 caso estejamos no Menu, 1 caso estejamos jogando, 2 caso acessando scores...

    menuOptions* menu = initMenu(); //Inicializa o menu
    menuOptions* menuPausa = initMenuPausa(); //Inicializa o menu


    stats currentStatus = {0,3};
    stats *ptrStatus = &currentStatus;

    printPlacar(readPlacar()); // testes ***



    // Carrega o mapa do arquivo
    char* matrix = carregaMapa(faseAtual);
    if (matrix == NULL) {
        return 1;
    }

    entities* entidades = getEntities(matrix);

    // Imprime a matriz do mapa no console
    //printaMatriz(matrix);



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
          if (IsKeyPressed(KEY_SPACE) || IsKeyPressed(KEY_ENTER)) selecionaOpcMenu(menu,&gameMode,&exit,&matrix,entidades,&faseAtual,ptrStatus);
          if (menu->selectedOption < NOPTIONS-1)
            if (IsKeyPressed(KEY_S) || IsKeyPressed(KEY_DOWN)) menu->selectedOption++;
          if (menu->selectedOption > 0)
            if (IsKeyPressed(KEY_W) || IsKeyPressed(KEY_UP)) menu->selectedOption--;
          BeginDrawing();
              ClearBackground(BLACK);
              menuDraw(0,menu,ptrStatus);
          EndDrawing();
          break;
        case 1: // --> Jogo rodando
          if (IsKeyPressed(KEY_P) || IsKeyPressed(KEY_TAB)) gameMode = 4;

          // mechhe os bixos!
      //  for (int i=0;i<entidades->nFlames;i++) {
      //    if (entidades->flames[i]->isRight)
      //      move((base*)entidades->flames[i], 'D');
      //    else
      //      move((base*)entidades->flames[i], 'A');
      //  }
          bichoIA(entidades,matrix);



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
   //   printf("pos X: %.2f \n",entidades->player->pos[1]);
   //   printf("pos Y: %.2f \n",entidades->player->pos[0]);
   //   system("clear");
   //    // printf("0: %d\n1: %d\n2: %d\n3: %d\n4: %d\n5: %d\n",entidades->player->moved[0],entidades->player->moved[1],entidades->player->moved[2],entidades->player->moved[3],entidades->player->moved[4],entidades->player->moved[5]);
   //    printf("player ta em %c\n",qBloco(*(base*)entidades->player,matrix));

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
           ptrStatus->lives --;
           if (ptrStatus->lives == 0) {
             if (ptrStatus->score > 0) novoScore(ptrStatus->score);
             gameMode = 5;
           }
         }

         // Checa se player está na porta
         if (matrix[(int)(entidades->player->pos[0])*NCOL+(int)(entidades->player->pos[1])] == 'F') {
           gameMode = 3;
           ptrStatus->score  += 100;
           counTimer = 0;
         //  printf("encostou no F\nfase atual: %d\n",faseAtual);
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
      //    printf("counter: %d\n",counTimer);
          counTimer ++;
          BeginDrawing();
              ClearBackground(BLACK);
              menuDraw(2,menu,ptrStatus);
          EndDrawing();
          break;
        case 3: // Tela de "Passou de fase!"
          //if (IsKeyPressed(KEY_SPACE) || IsKeyPressed(KEY_W)) {
          if (counTimer > 150) {
            faseAtual++;
            matrix = carregaMapa(faseAtual);
            if (matrix == NULL) {
              faseAtual = 1;
              matrix = carregaMapa(faseAtual);
              if (matrix == NULL) {
                printf("erro ao carregar mapa\n");
                break;
              }
            }
            entidades = getEntities(matrix);
            gameMode = 1;
          }
        //  printf("counter: %d\n",counTimer);
          counTimer ++;
          BeginDrawing();
              ClearBackground(BLACK);
              menuDraw(1,menu,ptrStatus);
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
              menuDraw(0,menuPausa,ptrStatus);
          EndDrawing();
          break;
          case 5:
            if (IsKeyPressed(KEY_SPACE) || IsKeyPressed(KEY_ENTER)) selecionaOpcMenu(menu,&gameMode,&exit,&matrix,entidades,&faseAtual,ptrStatus);
            if (menu->selectedOption < NOPTIONS-1)
              if (IsKeyPressed(KEY_S) || IsKeyPressed(KEY_DOWN)) menu->selectedOption++;
            if (menu->selectedOption > 0)
              if (IsKeyPressed(KEY_W) || IsKeyPressed(KEY_UP)) menu->selectedOption--;
            BeginDrawing();
                ClearBackground(BLACK);
                menuDraw(3,menu,ptrStatus);
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
