#ifndef MAPA_H
#define MAPA_H

#include "game.h"

// As dimensões do mapa já são definidas em game.h.
// Não redefina NCOL e NLIN aqui para evitar inconsistências.
char* alocaMatriz(void);
char* carregaMapa(mario* player,int fase);
void printaMatriz(char* matrix);
void printaMatriz2(char* matrix, const mario player);

#endif // MAPA_H
