#ifndef COMMON_H
#define COMMON_H

#include <pthread.h>

typedef struct {
    unsigned char *buffer; // intesidade dos pixels
    int largura;
    int altura;
    int maxIteracoes;
    int numThreads;
    int threadId;

    //Pthreads2 com mutex para fila de trabalho
    pthread_mutex_t *mutex; // mutex compartilhado
    int *linhaAtual; // contador compartilhado
} MandelbrotArgs;

//Parser dos argumentos
int parser(int argc, char **argv, int *largura, int *altura, int *maxIteracoes, int *numThread);

//Função de Calculo para o num de iterações
int calculoMandelbrot(double cReal, double cImag, int maxIteracoes);

//Conversao do num de iteracoes para uma intensidade entre 0 e 255
unsigned char intensidade(int iteracoes, int maxIteracoes);

//Escrita do arquivo .pgm
int escritaPGM(const char *arquivo, unsigned char *buffer, int largura, int altura);

#endif