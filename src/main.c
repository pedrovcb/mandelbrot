#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "base.h"

#define LOGIN "pvcb"

void calcSerial(unsigned char *buffer, int largura, int altura, int maxIteracoes);

int main(int argc, char **argv){
    int largura, altura, maxIteracoes, numThreads;

    if(parser(argc, argv, &largura, &altura,&maxIteracoes, &numThreads) != 0){
        return 1;
    }

    unsigned char *buffer = (unsigned char *)malloc(largura * altura * sizeof(unsigned char));
    if (buffer == NULL) {
        fprintf(stderr, "Erro: falha ao alocar memoria para o buffer!\n");
        return 1;
    }

    //Calculo tempo serial
    struct timespec inicio, fim;
    clock_gettime(CLOCK_MONOTONIC, &inicio);

    calcSerial(buffer, largura, altura, maxIteracoes);

    clock_gettime(CLOCK_MONOTONIC, &fim);
    double tempoSerial = (fim.tv_sec - inicio.tv_sec) + (fim.tv_nsec - inicio.tv_nsec) / 1e9;

    //Escrita do arquivo .pgm
    char arquivo[256];
    snprintf(arquivo, sizeof(arquivo), "mandelbrot_%s_serial.pgm", LOGIN);
    if (escritaPGM(arquivo, buffer, largura, altura) != 0) {
        free(buffer);
        return 1;
    }

    // Registro times.txt
    FILE *fp = fopen("times.txt", "w");

    if (fp == NULL) {
        fprintf(stderr, "Erro: falha ao criar times.txt\n");
        free(buffer);
        return 1;
    }

    fprintf(fp, "Serial: %.6f segundos\n", tempoSerial);

    fclose(fp);

    free(buffer);

    return 0;
}