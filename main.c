/*
-> Por hora temos a renderização basica do mapa, com um player movimentavel.
* Implementar a logica que faz o player (e futuros inimigos não atravessarem plataformas)
  . Isso vai envolver manter o jogo a um certo FPS, e então checar se o que há abaixo do jogador é uma plataforma.
  . Futuramente, será necessário deixar a posição do jogador como float 


*/
#include <stdio.h>
#include <stdlib.h>
#include <raylib.h>

#define NCOL 31
#define NLIN 31

#define POS 20 
#define TAM 20

#define SPEED .25
#define GRAVITY .01

typedef struct player {
	float pos[2];
    int intPos[8];      // possição em inteiros dos 4 cantos do player, da esquerda para a direita, de cima para baixo; ex: intPos[0] = cord x do cando superior esquerdo; intPos[5] = cord y do canto inferior esquerdo.
    float verticalV;    // velocidade vertical sera usada para lidar gravidade, quanto a saltos eu cair de plataformas
} mario; //sim, nome horrivel para isso


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
                case 'F':
                    DrawRectangle(i*POS,j*POS,TAM,TAM, PINK); 
                    break;
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

    DrawRectangle(player.pos[1]*POS,player.pos[0]*POS, 4,4, PINK); 
    DrawRectangle(player.pos[1]*POS,player.pos[0]*POS+TAM-1, 4,4, PINK); 
}


void calculaCantosInt(mario* player) {
    float offset = 0.9;
    player->intPos[0] = (int)player->pos[0];
    player->intPos[1] = (int)player->pos[1];

    player->intPos[2] = (int)player->pos[0];
    player->intPos[3] = (int)(player->pos[1]+offset);

    player->intPos[4] = (int)(player->pos[0]+offset);
    player->intPos[5] = (int)player->pos[1];

    player->intPos[6] = (int)(player->pos[0]+offset);
    player->intPos[7] = (int)(player->pos[1]+offset);
}

void mover(mario* player) {
    if (player->verticalV > .7) player->verticalV = .7;
    player->pos[0] += player->verticalV;

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
    SetTargetFPS(60);
	while (!WindowShouldClose()) {
        //O que segue é um sistema *ruim* para colisão com plataformas, utilizando ocmo base a posição em inteiros do player (isso descobre em qual quadrado da grade se encontra o player), a partir disso, facilitado pelo fato de que todas as plataformas estao alinhadas a uma grade, fica facil checar se a colisão ocorre ou não (isso vai quebrar se a velocidade for muito grande....
		if (IsKeyDown(KEY_W)) player1->pos[0] -= SPEED;
        calculaCantosInt(player1);
        if (matrix[(player1->intPos[4]-1)*NCOL+player1->intPos[5]] == 'Z') { player1->pos[0] = player1->intPos[4]; calculaCantosInt(player1);}
        if (matrix[(player1->intPos[6]-1)*NCOL+player1->intPos[7]] == 'Z') { player1->pos[0] = player1->intPos[4]; calculaCantosInt(player1);}

		if (IsKeyDown(KEY_S)) player1->pos[0] += SPEED;
        calculaCantosInt(player1);
        if (matrix[(player1->intPos[0]+1)*NCOL+player1->intPos[1]] == 'Z') {player1->pos[0] = player1->intPos[0]; calculaCantosInt(player1);}
        if (matrix[(player1->intPos[2]+1)*NCOL+player1->intPos[3]] == 'Z') {player1->pos[0] = player1->intPos[0]; calculaCantosInt(player1);}

		if (IsKeyDown(KEY_D)) player1->pos[1] += SPEED;
        calculaCantosInt(player1);
        if (matrix[(player1->intPos[0])*NCOL+player1->intPos[1]+1] == 'Z') { player1->pos[1] = player1->intPos[1]; calculaCantosInt(player1);}
        if (matrix[(player1->intPos[4])*NCOL+player1->intPos[5]+1] == 'Z') { player1->pos[1] = player1->intPos[1]; calculaCantosInt(player1);}

		if (IsKeyDown(KEY_A)) player1->pos[1] -= SPEED;
        calculaCantosInt(player1);
        if (matrix[(player1->intPos[2])*NCOL+player1->intPos[3]-1] == 'Z') { player1->pos[1] = player1->intPos[3]; calculaCantosInt(player1);}
        if (matrix[(player1->intPos[6])*NCOL+player1->intPos[7]-1] == 'Z') { player1->pos[1] = player1->intPos[7]; calculaCantosInt(player1);}

        player1->verticalV += GRAVITY;      
        mover(player1);
        calculaCantosInt(player1);
        if (matrix[(player1->intPos[0]+1)*NCOL+player1->intPos[1]] == 'Z') {
            player1->pos[0] = player1->intPos[0]; 
            calculaCantosInt(player1);
            player1->verticalV = 0.0;

        }

        if (matrix[(player1->intPos[2]+1)*NCOL+player1->intPos[3]] == 'Z') {
            player1->pos[0] = player1->intPos[0]; 
            calculaCantosInt(player1);
            player1->verticalV = 0.0;
        }
                             
        printf("current Velocity: %.2f \n", player1->verticalV);




		BeginDrawing();
			ClearBackground(BLACK);
            drawMatrix(matrix,structThing);
		EndDrawing();	
	}


	return 0;
}
