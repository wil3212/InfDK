#ifndef MENU_H
#define MENU_H
#include <stdbool.h>
#include "game.h"

#define NOPTIONS 3
#define OPTEXTH 50
#define OPTEXTW 100

#define OPTEXTx 250  // coordenada do canto superior esquerdo do texto das opções
#define OPTEXTy 250
#define OPTEXTofset 50

typedef struct menu {
  int selectedOption;
  char *options[NOPTIONS];
} menuOptions;


void menuDraw(int page, menuOptions* menu); // 0 para o menu inicial, 1 para a pagina de pasosu de fase,...
menuOptions* initMenu(); // Ini..
void selecionaOpcMenu(menuOptions* menu,int *gameMode, bool *exit, char** matriz,entities* entidades, int* fase);

menuOptions* initMenuPausa();
void selecionaOpcPausa(menuOptions* menu,int *gameMode, bool *exit);


#endif // MENU_H
