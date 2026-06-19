#ifndef GAME_H
#define GAME_H

#include <raylib.h>
#include <stdbool.h>

#define NCOL 31
#define NLIN 31

#define POS 20
#define TAM 20

#define CSPEED 0.15f
#define SPEED 0.20f
#define GRAVITY 0.02f
#define JUMP_FORCE -0.38f
#define D_JUMP_FORCE -0.40f

#define maxFlames 50

#define ladderOffSet 1.0
typedef struct {
  int score;
  int lives;
} stats;

// A estrutura mario representa o jogador no jogo, armazenando sua posição atual em coordenadas de ponto flutuante (pos)
typedef struct player {
    float pos[2];
    int intPos[8];
    int moved[6];
    float speedFactor;  // +
    float verticalV;
    float horizontalV; //slippery ground?
    int grounded;
    int jumpCount;
    bool alive;
    bool canJump;
    bool isClimbing;
    int isRight; // 0 -> esquerda 1 -> direita
 // int score;
 // int lives;
} mario;

typedef struct enemy {
  float pos[2];
  int intPos[8];
  int moved[6];
  float speedFactor;  // +
  float verticalV;
  float horizontalV; //slippery ground?
  int grounded;
  int jumpCount;
  bool alive;
  bool isClimbing;
  int isRight; // 0 -> esquerda 1 -> direita
  int clock;
  int clockLimit;
} flame;

typedef struct { //common to any kind of entity
  float pos[2];
  int intPos[8];
  int moved[6]; // +
  float speedFactor;  // +
  float verticalV;
  float horizontalV; //slippery ground?
  int grounded;
  int jumpCount;
  bool alive;
  bool isClimbing;
  int isRight; // 0 -> esquerda 1 -> direita
} base;

typedef struct entities {
  mario* player;
  flame* flames[maxFlames];
  int nFlames;
} entities;

void bichoIA(entities *entity,char *matrix);
void flameIA(flame *f, char* matrix);
char qBloco(base entity, char* matrix);   // retorna o char sob o qual o bloco esta em cima (D ou S); Retorna 'O' caso contrario
int rInt(int min,int max);

void checkDead(base* entity);
void checkDeadList(entities *entidades);

int hitBoxCheck(base* entity1, base* entity2);
int hitBoxCheckListl(entities *entidades);

void calculaCantosInt(mario* player);              // ok
void calculaCantosInt2(base* entity);

void colisionCheck(base *entity, int direction, char* matrix);
void colisionCheckMain(base *entity, char* matrix);

void move(base *entity, char mode);

bool isSolid(char block);

void getPoss(char* matriz, mario* player);  // le a char matrix e a partir dai, passa a posição do P para o player

mario* initPlayer();
flame* initFlame();

entities* getEntities(char* matriz); // returns the entities list (struct) with all enemies and player from the matrix (with its possitions)


#endif // GAME_H
