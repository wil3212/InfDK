
#ifndef PLAYER_H
#define PLAYER_H
/** A estrutura mario representa o jogador no jogo, armazenando sua posição atual em coordenadas de ponto flutuante (pos)
* as posições inteiras dos cantos do jogador (intPos), a velocidade vertical (verticalV), o número de pulos realizados (jumpCount) 
* e um indicador booleano para determinar se o jogador pode pular (canJump). */
#include "game.h"

void calculaCantosInt(mario* player);

#endif // PLAYER_H
