#include "game.h"
#include <stdlib.h>
#include <math.h>
#include <stdio.h>
/** A função calculaCantosInt é responsável por calcular as posições inteiras dos cantos do jogador com base em sua posição atual.
 *  Ela armazena essas posições em um array intPos dentro da estrutura mario, que pode ser usado para verificar colisões ou interações com o ambiente do jogo.
 */

void checkDead(base* entity) {
  if (entity->pos[0] > 300)
    entity->alive = 0;
}
void checkDeadList(entities *entidades) {
  for (int i=0;i<entidades->nFlames;i++) {
    checkDead((base*)entidades->flames[i]);
  }
}
int hitBoxCheck(base* entity1, base* entity2) {
  float offset = 0.8;
  float center1[2], center2[2], distX, distY;
  int retorno;
  center1[0] = entity1->pos[0] + TAM / 2;
  center1[1] = entity1->pos[1] + TAM / 2;
  center2[0] = entity2->pos[0] + TAM / 2;
  center2[1] = entity2->pos[1] + TAM / 2;
  distX = fabs(center1[0] - center2[0]);
  distY = fabs(center1[1] - center2[1]);
  if (distX<offset && distY<offset) retorno = 1;
  else retorno = 0;
  return retorno;
}
int hitBoxCheckListl(entities *entidades) {
  for (int i=0;i<entidades->nFlames;i++)
    if (hitBoxCheck((base*)entidades->player,(base*)entidades->flames[i]))
      return 1;
  return 0;
}
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
void move(base *entity, char mode) {
  switch(mode) {
    case 'A':
      if (entity->grounded == 1 && !entity->isClimbing) {
        entity->pos[1] -= SPEED * entity->speedFactor;
        entity->moved[1] = 1;
        entity->horizontalV = -1.0;
      }
      break;
    case 'D':
      if (entity->grounded == 1 && !entity->isClimbing) {
        entity->pos[1] += SPEED * entity->speedFactor;
        entity->moved[3] = 1;
        entity->horizontalV = 1.0;
      }
      break;
    case 'W':
      // === 4. LÓGICA DE PULO (Melhorada) ===
      if (!entity->isClimbing) {
        if (entity->jumpCount == 0 && entity->grounded) {
            entity->verticalV = JUMP_FORCE;
            entity->jumpCount = 1;
            entity->grounded = 0;
            //printf("jumpppp");
        }
        else if (entity->jumpCount == 1) {
            entity->verticalV = D_JUMP_FORCE;
            entity->jumpCount = 2;
        }
        entity->moved[0] = 1;
      }
      break;
    case 'C':
      if (entity->isClimbing) {
        entity->pos[0] -= CSPEED;
        entity->moved[0] = 0;
      }
      entity->moved[4] = 1;
      break;
    case 'c':  //climb down
      if (entity->isClimbing) {
        entity->pos[0] += CSPEED;
        entity->moved[2] = 0;
      }
      entity->moved[5] = 1;
      break;
    case 'G':
      if (!entity->isClimbing) {
      entity->verticalV += GRAVITY;
      if (entity->verticalV != 0 && entity->grounded == 0) {
        entity->pos[1] += SPEED * entity->horizontalV * entity->speedFactor;
        if (entity->horizontalV < 0)
          entity->moved[1] = 1;
        if (entity->horizontalV > 0)
          entity->moved[3] = 1;
      }
      entity->pos[0] += entity->verticalV;
  //  if (entity->grounded == 1)
  //    entity->horizontalV = 0;
      entity->moved[2] = 1;
      }
      break;
  }
}
void colisionCheckMain(base *entity, char* matrix) {
  for (int i=0;i<6;i++)
    if (entity->moved[i])
      colisionCheck(entity, i, matrix);
}
void colisionCheck(base *entity, int direction, char* matrix) {
  switch (direction) {
    case 0:
      // Colisão com o Teto
      if (!entity->isClimbing) {
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
      }
      else {
        calculaCantosInt2(entity);
        if (isSolid(matrix[entity->intPos[0] * NCOL + entity->intPos[1]]) ||
                isSolid(matrix[entity->intPos[2] * NCOL + entity->intPos[3]])) {

          entity->pos[0] = (float)(entity->intPos[0] + 1);
          entity->verticalV = 0;
        }
        entity->moved[0] = 0;
      }
      break;
    case 1: // A
      calculaCantosInt2(entity);
      int rowTop = (int)(entity->pos[0] + 0.1f);
      int rowBottom = (int)(entity->pos[0] + 0.9f);
      if (entity->pos[1] < 0.0f) {
          entity->pos[1] = 0.0f;
      }
      if (isSolid(matrix[rowTop * NCOL + (int)(entity->pos[1])]) ||
          isSolid(matrix[rowBottom * NCOL + (int)(entity->pos[1])])) {
       //   entity->pos[1] -1.0 < -1.0) {
          entity->pos[1] = (float)((int)(entity->pos[1]) + 1.0f);
      }
      entity->moved[1] = 0;
      break;
    case 2:
      if (!entity->isClimbing) {
        if (entity->verticalV >= 0) {
            calculaCantosInt2(entity);
            if (isSolid(matrix[entity->intPos[4] * NCOL + entity->intPos[5]]) ||
                isSolid(matrix[entity->intPos[6] * NCOL + entity->intPos[7]])) {
                  entity->pos[0] = (float)entity->intPos[0];
                  entity->verticalV = 0;
                  entity->grounded = 1;
                  entity->jumpCount = 0;
                  entity->horizontalV = 0;
            }
            else
              entity->grounded = 0;
        }
        entity->moved[2] = 0;
      }
      else {
        calculaCantosInt2(entity);
        if (isSolid(matrix[entity->intPos[4] * NCOL + entity->intPos[5]]) ||
            isSolid(matrix[entity->intPos[6] * NCOL + entity->intPos[7]]))
              entity->pos[0] = (float)entity->intPos[0];
        entity->moved[2] = 0;
      }
      break;
    case 3: // D
      rowTop = (int)(entity->pos[0] + 0.1f);
      rowBottom = (int)(entity->pos[0] + 0.9f);
      if (isSolid(matrix[rowTop * NCOL + (int)(entity->pos[1] + 1.0f)]) ||
          isSolid(matrix[rowBottom * NCOL + (int)(entity->pos[1] + 1.0f)]) ||
          (int)(entity->pos[1] + 1.0f) > 29) {
            entity->pos[1] = (float)((int)(entity->pos[1] + 1.0f) - 1.0f);
      }
      entity->moved[3] = 0;
      break;
    case 4:
        calculaCantosInt2(entity);
        char chur = qBloco(*(base*)entity,matrix);
        if (chur == 'S')
          entity->isClimbing = 1;
        else if (chur == 'D') {
          entity->isClimbing = 0;
          entity->verticalV = 0;
    //      entity->grounded = 1;
          entity->pos[0] = (float)entity->intPos[0];
        }
    //  else
    //    entity->isClimbing = 0;

        entity->moved[4] = 0;
        break;
    case 5:
        calculaCantosInt2(entity);
        chur = qBloco(*(base*)entity,matrix);
        if (chur == 'D')
          entity->isClimbing = 1;
        else if (chur == 'S') {
          entity->isClimbing = 0;
          entity->verticalV = 0;
          entity->grounded = 1;
          entity->pos[0] = (float)entity->intPos[0];
        }
    //  else
    //    entity->isClimbing = 0;
        entity->moved[5] = 0;
      break;
  }
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
  for (int i=0;i<6;i++)
    tmp->moved[i] = 0;
  tmp->grounded = 1;
  tmp->alive = 1;
//tmp->score = 0;
//tmp->lives = 3;
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
  for (int i=0;i<6;i++)
    tmp->moved[i] = 0;
  tmp->grounded = 1;
  tmp->jumpCount = 0;
  tmp->isRight = 0; 
  tmp->alive = 1;
  tmp->isClimbing = 0;
  tmp->clock = 0;
  tmp->clockLimit = 0;
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


int rInt(int min,int max) {
  return (rand() % (max - min + 1)) + min;
}
char qBloco(base entity, char* matrix) {   // retorna o char sob o qual o bloco esta em cima (D ou S); Retorna 'O' caso contrario
// get center of player hitbox, and center of matrix square it is in, then, the distance between teh two centers
  char retorno = 'O';
  float p[2];
  float block[2];
  p[0] = entity.pos[0] + 0.5;
  p[1] = entity.pos[1] + 0.5;
  char ch = matrix[(int)(p[0])*NCOL+(int)(p[1])];
  if (ch == 'S' || ch == 'D') {
    block[0] = (int)p[0] + 0.5;
    block[1] = (int)p[1] + 0.5;
    float dist = sqrtf(powf(p[0]-block[0],2)+powf(p[1]-block[1],2));
    if (dist < ladderOffSet)
      retorno = ch;
  }
  return retorno;
}

void flameIA(flame *f, char* matrix) {
  // direction flipping logic
  f->clock ++;
  if (f->clock > f->clockLimit) {
    f->clock = 0;
    f->clockLimit = 50 + rInt(0,100);
    switch (rInt(0,1)) {
      case 0:
        break;
      case 1:
        f->isRight = !f->isRight; //this will work for climbing as well
        break;
    }
  }
  // moving part

  if (f->isClimbing == 0) {
    if (f->clock < 35) {
      switch (qBloco(*(base*)(f),matrix)) {
        case 'D': // Ladder end
          move((base*)f, 'c'); // C é climb up e 'c' é climb down
          f->clock = 0;
          f->isRight = 0;
          break;
        case 'S': // S é ladder Start
          move((base*)f, 'C'); // C é climb up e 'c' é climb down
          f->clock = 0;
          f->isRight = 1;
          break;
        case 'O':
          if (f->isRight)
            move((base*)f, 'D');
          else
            move((base*)f, 'A');
          break;
      }
    }
    else {
      if (f->isRight)
        move((base*)f, 'D');
      else
        move((base*)f, 'A');
    }
  }
  else {
 // if (f->clock % 3) {
 //   switch (qBloco(*(base*)(f),matrix)) {
 //     case 'D': // Ladder end
 //       move((base*)f, 'C'); // C é climb up e 'c' é climb down
 //       break;
 //     case 'S': // S é ladder Start
 //       move((base*)f, 'c'); // C é climb up e 'c' é climb down
 //       break;
 //   }
 // }
    //else {
      if (f->isRight)
        move((base*)f, 'C');
      else
        move((base*)f, 'c');
   // }
  }
}

void bichoIA(entities *entity,char *matrix) {
  for (int i=0;i<entity->nFlames;i++)
    flameIA(entity->flames[i],matrix);
}
