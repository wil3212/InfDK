#include "menu.h"
#include <raylib.h>
#include <stdlib.h>
#include <stdbool.h>

void menuDraw(int page, menuOptions* menu) {
  switch (page) {
    case 0:
      /*
      //DrawText(const char *text, int posX, int posY, int fontSize, Color color);
      DrawText(menu->options[0],250,250,20, BLUE);
      DrawText(menu->options[1],250,300,20, BLUE);
      DrawText(menu->options[2],250,350,20, BLUE);
      */
      // Desenha o texto das 3 opções
      for (int i=0;i<NOPTIONS;i++)
        DrawText(menu->options[i],OPTEXTx,OPTEXTy+OPTEXTofset*i,20, BLUE);

      // Desenha o retangulo vermelho (seleção da opção)
      //void DrawRectangleLines(int posX, int posY, int width, int height, Color color);
      DrawRectangleLines(OPTEXTx-5, OPTEXTy+OPTEXTofset*(menu->selectedOption)-5,OPTEXTW+20,OPTEXTH-15, RED);

    break;
    case 1:
      //DrawText(const char *text, int posX, int posY, int fontSize, Color color);
      DrawText("Passou de fase!!!",250,250,20, BLUE);
    break;
  }

}

void selecionaOpc(menuOptions* menu,int *gameMode, bool *exit) {
  switch (menu->selectedOption) {
    case 0:
      *gameMode = 1;
    break;

    case 2:
      *exit = true;
    break;
  }
}

menuOptions* initMenu() {
  menuOptions* thisMenu = malloc(sizeof(menuOptions));
  thisMenu->selectedOption = 0;
  thisMenu->options[0] = "Novo Jogo";
  thisMenu->options[1] = "Ranking";
  thisMenu->options[2] = "Sair";
  return thisMenu;
}
