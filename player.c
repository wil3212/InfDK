#include "player.h"
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
