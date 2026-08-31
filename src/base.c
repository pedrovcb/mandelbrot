#include <stdio.h>
#include <stdlib.h>
#include "base.h"

int parser(int argc, char **argv, int *largura, int *altura, int *maxIteracoes, int *numThreads){
    if(argc != 5){
        fprintf(stderr, "Comando usado incorretamente! Comando: %s <largura> <altura> <maxIteracoes> <numThreads> \n", argv[0]);
        return -1;
    }

    char *fimPtr; //Ponteiro para informar fim de leitura

    // argv[1] - largura
    long num = strtol(argv[1], &fimPtr, 10);

    if(fimPtr == argv[1] || *fimPtr != '\0'){
        fprintf(stderr, "Erro: Valor '%s' inválido para largura!\n", argv[1]);
        return -1;
    }
    if (num <= 0) {
        fprintf(stderr, "Erro: Valor da largura deve ser maior que zero!\n");
        return -1;
    }
    *largura = (int)num;

    // argv[2] - altura
    num = strtol(argv[2], &fimPtr, 10);
    if(fimPtr == argv[2] || *fimPtr != '\0'){
        fprintf(stderr, "Erro: Valor '%s' inválido para altura!\n", argv[2]);
        return -1;
    }
    if (num <= 0) {
        fprintf(stderr, "Erro: Valor da altura deve ser maior que zero!\n");
        return -1;
    }
    *altura = (int)num;

    //argv[3] - maxIteracoes
    num = strtol(argv[3], &fimPtr, 10);
    if(fimPtr == argv[3] || *fimPtr != '\0'){
        fprintf(stderr, "Erro: Valor '%s' inválido para o máximo de iterações!\n", argv[3]);
        return -1;
    }
    if (num <= 0) {
        fprintf(stderr, "Erro: Valor do limite de iterações deve ser maior que zero!\n");
        return -1;
    }
    *maxIteracoes = (int)num;

    // argv[4] - numThreads
    num = strtol(argv[4], &fimPtr, 10);
    if(fimPtr == argv[4] || *fimPtr != '\0'){
        fprintf(stderr, "Erro: Valor '%s' inválido para a quantidade de Threads!\n", argv[4]);
        return -1;
    }
    if (num <= 0) {
        fprintf(stderr, "Erro: Valor para a quantidade de Threads deve ser maior que zero!\n");
        return -1;
    }
    *numThreads = (int)num;
    
    return 0;
}

int calculoMandelbrot(double cReal, double cImag, int maxIteracoes){
    double zReal = 0.0;
    double zImag = 0.0;

    for (int iteracao = 0; iteracao<maxIteracoes; iteracao++){

        // Forma de escape caso o valor tenda ao infinito
        double zReal2 = zReal * zReal;
        double zImag2 = zImag * zImag;
        if (zReal2 + zImag2 > 4.0) {
            return iteracao;
        }

        //Formula Mandelbrot
        zImag = 2.0 * zReal * zImag + cImag;
        zReal = zReal2 - zImag2 + cReal;
    }

    return maxIteracoes;    
}

unsigned char intensidadePixels(int iteracoes, int maxIteracoes){
    return (unsigned char)((iteracoes / (double)maxIteracoes) * 255.0);
}

int escritaPGM(const char *arquivo, unsigned char *buffer, int largura, int altura){
    FILE *fp = fopen(arquivo, "w");

    if (fp == NULL) {
        fprintf(stderr, "Erro: o arquivo '%s' falhou em ser criado!\n", arquivo);
        return -1;
    }

    for(int py = 0; py<altura; py++){
        for(int px = 0; px<largura; px++){
            if(px>0){
                fprintf(fp, " ");
            }

            fprintf(fp, "%d", buffer[py * largura + px]);
        }

        fprintf(fp, "\n");
    }

    fclose(fp);
    return 0;
}