#include <stdio.h>
#include <stdlib.h>
#include <raylib.h>

#define NCOL 31
#define NLIN 31

#define LEL 20 
#define TAM 20


int main() {
	
	float pos[2] = {240,250};

	char matrix[NLIN][NCOL];

    FILE *maps;
    maps = fopen("mapa.txt", "r");
    if (maps == NULL) {
        printf("Error: Could not open file.\n");
        return 1;
     }
    for (int i=0;i<NLIN;i++) {
        for (int j=0;j<NCOL;j++) {
            matrix[i][j] = fgetc(maps);		
        }
    }	
    fclose(maps);

    for (int i=0;i<NLIN;i++) {
        for (int j=0;j<NCOL;j++) {
            printf("%c",matrix[i][j]);
        }
       // printf("\n");
    }

	InitWindow(500,500,"shdfhsdfhshf");	

	while (!WindowShouldClose()) {
		if (IsKeyPressed(KEY_UP)) pos[1] -= LEL;
		if (IsKeyPressed(KEY_DOWN)) pos[1] += LEL;
		if (IsKeyPressed(KEY_RIGHT)) pos[0] += LEL;
		if (IsKeyPressed(KEY_LEFT)) pos[0] -= LEL;
		BeginDrawing();
			ClearBackground(BLACK);
			DrawRectangle(pos[0],pos[1], TAM,TAM, BLUE); 
		EndDrawing();	
	}

	return 0;
}

