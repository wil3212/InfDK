#include "placar.h"
#include <stdio.h>
#include <stdlib.h>


// ----------------------------------------------------------

void printPlacar(tipo_placar* placarVec) {
  if (placarVec) {
    for (int i=0;i<maxScoreEntries;i++) {
      if (placarVec[i].time > 0) {
        printf("---------\n");
        printf("Rank: %d\n",i+1);
        printf("%s",placarVec[i].nome);
        printf("Score: %d\n",placarVec[i].time);
      }
    }
  }
  else printf("Placar em branco\n");
}
void criaPlacar() {
  FILE *scoreBin = fopen(placarArq,"wb");
  if (scoreBin == NULL)
    printf("Falha ao abrir placar.bin\n");
  else {
    tipo_placar tmp = {"\0", 0};
    for (int i=0;i<maxScoreEntries;i++) {
      if(1 == fwrite(&tmp,sizeof(tipo_placar),1,scoreBin)) printf("escreveu %d com sucesso\n",i);
      else printf("erro ao escrever no arquivo\n");
    }
    fclose(scoreBin);
  }
}
void atualizaPlacar(tipo_placar* novoPlacar) {
  FILE *scoreBin = fopen(placarArq,"wb");
  if (scoreBin == NULL)
    printf("Falha ao abrir placar.bin\n");
  else {
      if(maxScoreEntries == fwrite(novoPlacar,sizeof(tipo_placar),maxScoreEntries,scoreBin)) printf("escreveu  com sucesso\n");
      else printf("erro ao escrever no dddddarquivo\n");
    fclose(scoreBin);
  }
}
tipo_placar* readPlacar() {
  tipo_placar *retorno = (tipo_placar*) malloc(sizeof(tipo_placar) * maxScoreEntries);
  retorno[0].time = 0; // sinalizando lista vazia
  FILE *scoreBin = fopen(placarArq,"rb");
  if (scoreBin == NULL) {
    printf("Falha ao ler placar.bin\n");
    criaPlacar();
    retorno = NULL;
  }
  else {
    for (int i=0;i<maxScoreEntries;i++)
      fread(&retorno[i],sizeof(tipo_placar),1,scoreBin);
    fclose(scoreBin);
  }
  return retorno;
}
int novoScore(int newScore) {
  int retorno;
  tipo_placar *velho = readPlacar();
  tipo_placar *novoPlacar = (tipo_placar*) malloc(sizeof(tipo_placar) * maxScoreEntries);
  int i=0;
  tipo_placar *tmp = &velho[i];
  while (velho[i].time > newScore) {
    novoPlacar[i] = velho[i];
    i++;
  }
  if (i<maxScoreEntries) {
    retorno = i;
    printf("informe seu nome\n");
    fgets(novoPlacar[i].nome,20,stdin);
    novoPlacar[i].time = newScore;
    i++;
    while (i<maxScoreEntries) {
      novoPlacar[i] = tmp[i-1];
      i++;
    }
  }
  printf("Seu score esta na %d° colocação\n",retorno+1);
  printPlacar(novoPlacar);
  atualizaPlacar(novoPlacar);
  free(velho);
  free(novoPlacar);
  return retorno;
}
