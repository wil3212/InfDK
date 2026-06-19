#ifndef PLACAR_H_
#define PLACAR_H_

#define maxScoreEntries 10
#define placarArq "placar.bin"

typedef struct {
char nome[20];
int time;
} tipo_placar;


void printPlacar(tipo_placar* placarVec);
void criaPlacar();
void atualizaPlacar(tipo_placar* novoPlacar);
tipo_placar* readPlacar();
int novoScore(int newScore);

#endif // PLACAR_H_
