 #include <stdio.h>
#include <stdlib.h>
#include <raylib.h>

#define NCOL 31
#define NLIN 31

#define POS 20 
#define TAM 20



typedef struct player {
	int pos[2];
} mario;


char* alocaMatriz() { // matriz[i*NCOL+j]
    return (char*) malloc(sizeof(char)*NCOL*NLIN);
}

char* carregaMapa(mario* mario) { 
	char tmpChar;
	char* matrix = alocaMatriz();
    FILE *maps;         
    maps = fopen("mapa.txt", "r");
    if (maps == NULL) {
        printf("Error: Could not open file.\n");
        return NULL;
     }
    for (int i=0;i<NLIN;i++) {
        for (int j=0;j<NCOL;j++) {
	    tmpChar = fgetc(maps);
            matrix[i*NCOL+j] = tmpChar;		
	    if (tmpChar == 'P') {
		    mario->pos[0] = i;
		    mario->pos[1] = j;
	    }

        }
    }	
    fclose(maps);
    return matrix;
}

void printaMatriz2(char* matrix, mario player) {
    for (int i=0;i<NLIN;i++) {
        for (int j=0;j<NCOL;j++) {
		if (player.pos[0] == i && player.pos[1] == j) printf("P");
		else printf("%c",matrix[i*NCOL+j]);
        }
    }
    printf("\n");
}
void printaMatriz(char* matrix) {
    for (int i=0;i<NLIN;i++) {
        for (int j=0;j<NCOL;j++) {
            printf("%c",matrix[i*NCOL+j]);
        }
    }
}

void drawMatrix(char* matrix, mario player) {
    for (int i=0;i<NLIN;i++) {
        for (int j=0;j<NCOL;j++) {
            //printf("%c",matrix[i*NCOL+j]);
            switch (matrix[j*NCOL+i]) {
                case 'Z':
                    DrawRectangle(i*POS,j*POS,TAM,TAM, BLUE); 
                    break;
                case 'S': 
                    //break;
                case 'H':
                    //break;
                case 'D':
                    DrawRectangle(i*POS,j*POS,TAM,TAM, YELLOW); 
                    break;
            }
        }
    }
    DrawRectangle(player.pos[1]*POS,player.pos[0]*POS, TAM,TAM, RED); 
}

int main() {
	
    mario structThing;
	mario* player1 = &structThing;
    char* matrix = carregaMapa(player1);
    printaMatriz(matrix);

/*
    while (1) {
	    char tmpC;
	    scanf("%c", &tmpC);
		if (tmpC == 'w') player1->pos[0] --;
		if (tmpC == 's') player1->pos[0] ++;
		if (tmpC == 'd') player1->pos[1] ++;
		if (tmpC == 'a') player1->pos[1] --;
    printaMatriz2(matrix,structThing);

    printf("%c", tmpC);
    printf("p pos 1: %d /np.pos 2: %d/n", player1->pos[0], player1->pos[1]);
    }
*/

	InitWindow((NLIN-1)*TAM,(NCOL-1)*TAM,"shdfhsdfhshf");	

	while (!WindowShouldClose()) {
		if (IsKeyPressed(KEY_UP)) player1->pos[0] --;
		if (IsKeyPressed(KEY_DOWN)) player1->pos[0] ++;
		if (IsKeyPressed(KEY_RIGHT)) player1->pos[1] ++;
		if (IsKeyPressed(KEY_LEFT)) player1->pos[1] --;
		BeginDrawing();
			ClearBackground(BLACK);
            drawMatrix(matrix,structThing);
		EndDrawing();	
	}


	return 0;
}
