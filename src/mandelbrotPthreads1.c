#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "base.h"

void *threadFunc(void *arg) {

    MandelbrotArgs *args = (MandelbrotArgs *)arg;

    int linhaInicio = args->threadId * args->altura / args->numThreads;
    int linhaFim = (args->threadId + 1) * args->altura / args->numThreads;

    for (int py = linhaInicio; py < linhaFim; py++){
        for (int px = 0; px < args->largura; px++){

            double cReal = -2.0 + (double)px / args->largura * 3.0;
            double cImag = -1.5 + (double)py / args->altura * 3.0;

            int iteracoes = calculoMandelbrot(cReal, cImag, args->maxIteracoes);
            unsigned char intensidade = intensidadePixels(iteracoes, args->maxIteracoes);
            args->buffer[py * args->largura + px] = intensidade;
        }
    }
    
    return NULL;
}

void calcPthreads1(unsigned char *buffer, int largura, int altura, int maxIteracoes, int numThreads){

    pthread_t *threads = malloc(numThreads * sizeof(pthread_t));
    MandelbrotArgs *args = malloc(numThreads * sizeof(MandelbrotArgs));

    if (!threads || !args) {
        fprintf(stderr, "Erro: falha ao alocar memoria para as threads!\n");
        free(threads);
        free(args);
        return;
    }

    for (int i = 0; i < numThreads; i++) {
        args[i].buffer = buffer;
        args[i].largura = largura;
        args[i].altura = altura;
        args[i].maxIteracoes = maxIteracoes;
        args[i].numThreads = numThreads;
        args[i].threadId = i;

        pthread_create(&threads[i], NULL, threadFunc, &args[i]);
    }

    for (int i = 0; i < numThreads; i++) {
        pthread_join(threads[i], NULL);
    }

    free(threads);
    free(args);

}