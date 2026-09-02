#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "base.h"

#define LOGIN "pvcb"

void calcSerial(unsigned char *buffer, int largura, int altura, int maxIteracoes);
void calcOpenMP(unsigned char *buffer, int largura, int altura, int maxIteracoes, int numThreads);
void calcPthreads1(unsigned char *buffer, int largura, int altura, int maxIteracoes, int numThreads);
void calcPthreads2(unsigned char *buffer, int largura, int altura, int maxIteracoes, int numThreads);

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

    char arquivo[256];
    struct timespec inicio, fim;

    //Mandelbrot - Serial
    clock_gettime(CLOCK_MONOTONIC, &inicio);
    calcSerial(buffer, largura, altura, maxIteracoes);
    clock_gettime(CLOCK_MONOTONIC, &fim);
    double tempoSerial = (fim.tv_sec - inicio.tv_sec) + (fim.tv_nsec - inicio.tv_nsec) / 1e9;

    snprintf(arquivo, sizeof(arquivo), "mandelbrot_%s_serial.pgm", LOGIN);
    escritaPGM(arquivo, buffer, largura, altura);

    //Mandelbrot - OpenMP
    clock_gettime(CLOCK_MONOTONIC, &inicio);
    calcOpenMP(buffer, largura, altura, maxIteracoes, numThreads);
    clock_gettime(CLOCK_MONOTONIC, &fim);
    double tempoOpenMP = (fim.tv_sec - inicio.tv_sec) + (fim.tv_nsec - inicio.tv_nsec) / 1e9;

    snprintf(arquivo, sizeof(arquivo), "mandelbrot_%s_openmp.pgm", LOGIN);
    escritaPGM(arquivo, buffer, largura, altura);

    //Mandelbrot - Pthreads1
    clock_gettime(CLOCK_MONOTONIC, &inicio);
    calcPthreads1(buffer, largura, altura, maxIteracoes, numThreads);
    clock_gettime(CLOCK_MONOTONIC, &fim);
    double tempoPthreads1 = (fim.tv_sec - inicio.tv_sec) + (fim.tv_nsec - inicio.tv_nsec) / 1e9;

    snprintf(arquivo, sizeof(arquivo), "mandelbrot_%s_pthreads1.pgm", LOGIN);
    escritaPGM(arquivo, buffer, largura, altura);

    //Mandelbrot - Pthreads2
    clock_gettime(CLOCK_MONOTONIC, &inicio);
    calcPthreads2(buffer, largura, altura, maxIteracoes, numThreads);
    clock_gettime(CLOCK_MONOTONIC, &fim);
    double tempoPthreads2 = (fim.tv_sec - inicio.tv_sec) + (fim.tv_nsec - inicio.tv_nsec) / 1e9;

    snprintf(arquivo, sizeof(arquivo), "mandelbrot_%s_pthreads2.pgm", LOGIN);
    escritaPGM(arquivo, buffer, largura, altura);


    // Registro times.txt
    FILE *fp = fopen("times.txt", "w");

    if (fp == NULL) {
        fprintf(stderr, "Erro: falha ao criar times.txt\n");
        free(buffer);
        return 1;
    }

    fprintf(fp, "Serial: %.6fs\n", tempoSerial);
    fprintf(fp, "OpenMP: %.6fs\n", tempoOpenMP);
    fprintf(fp, "Pthreads1: %.6fs\n", tempoPthreads1);
    fprintf(fp, "Pthreads2: %.6fs\n", tempoPthreads2);

    fclose(fp);

    free(buffer);

    return 0;
}