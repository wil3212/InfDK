#include "game.h"
#include <stdlib.h>
/** A função calculaCantosInt é responsável por calcular as posições inteiras dos cantos do jogador com base em sua posição atual. 
 *  Ela armazena essas posições em um array intPos dentro da estrutura mario, que pode ser usado para verificar colisões ou interações com o ambiente do jogo.
 */
void calculaCantosInt(mario* player) {
    player->intPos[0] = (int) player->pos[0];
    player->intPos[1] = (int) player->pos[1];
    
  // As próximas linhas calculam as posições inteiras dos cantos do jogador, considerando que o jogador ocupa um espaço de 2x2 blocos no mapa.
    player->intPos[2] = (int) player->pos[0];
    player->intPos[3] = (int) (player->pos[1] + 1);

    player->intPos[4] = (int) (player->pos[0] + 1);
    player->intPos[5] = (int) player->pos[1];

    player->intPos[6] = (int) (player->pos[0] + 1);
    player->intPos[7] = (int) (player->pos[1] + 1);
}

bool isSolid(char block) {
    // Retorna true se o bloco for um tipo sólido que bloqueia o jogador
    return (block == 'Z');
}

mario* initPlayer() {
  mario* tmp = (mario*) malloc(sizeof(mario));
  tmp->pos[0] = 0.0;
  tmp->pos[1] = 0.0;
  for (int i=0;i<8;i++)
    tmp->intPos[i] = 0;
  tmp->verticalV = 0.0;
  tmp->jumpCount = 0;
  tmp->canJump = 0;
  tmp->isClimbing = 0;
  tmp->horizontalV = 0.0; //slippery ground?
  tmp->speedFactor = 1.0;  // +
  for (int i=0;i<4;i++)
    tmp->moved[i] = 0;
  tmp->grounded = 1;
  return tmp;
}

flame* initFlame() {
  flame* tmp = (flame*) malloc(sizeof(flame));
  tmp->pos[0] = 0.0;
  tmp->pos[1] = 0.0;
  for (int i=0;i<8;i++)
    tmp->intPos[i] = 0;
  tmp->horizontalV = 0.0; //slippery ground?
  tmp->verticalV = 0;
  tmp->speedFactor = 0.6;  // +
  for (int i=0;i<4;i++)
    tmp->moved[i] = 0;
  tmp->grounded = 1;
  tmp->jumpCount = 0;
  tmp->isRight = 0; //seg fault
  tmp->alive = 0;
  return tmp;
}

void getPoss(char* matriz, mario* player) {
  for (int i=0;i<NLIN;i++) {
    for (int j=0;j<NCOL;j++) {
      if (matriz[i*NCOL+j] == 'p') {
        player->pos[1] = j;
        player->pos[0] = i;
      }
    }
  }
}

entities* getEntities(char* matriz) {
  entities* lista = malloc(sizeof(entities));
  lista->nFlames = 0;
  for (int i=0;i<NLIN;i++) {
    for (int j=0;j<NCOL;j++) {
      if (matriz[i*NCOL+j] == 'P') {
        mario* tmp = initPlayer();
        lista->player = tmp;
        tmp->pos[1] = j;
        tmp->pos[0] = i;
      }
      if (matriz[i*NCOL+j] == 'E') {
        flame* tmp = initFlame();
        lista->flames[lista->nFlames] = tmp;
        lista->nFlames++;
        tmp->pos[1] = j;
        tmp->pos[0] = i;
        tmp->alive = 1;
        if (lista->nFlames % 2)
          tmp->isRight = 1;
        else tmp->isRight = 0;
      }
    }
  }
  return lista;
}
