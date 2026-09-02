#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "base.h"

void *threadFunc2(void *arg) {
    MandelbrotArgs *args = (MandelbrotArgs *)arg;
    
    int linha;
    
    while (1) {
        pthread_mutex_lock(args->mutex);
        
        linha = *args->linhaAtual;
        (*args->linhaAtual)++;
        
        pthread_mutex_unlock(args->mutex);
        
        if (linha >= args->altura) {
            break;
        }
        
        for (int px = 0; px < args->largura; px++) {
            double cReal = -2.0 + (double)px / args->largura * 3.0;
            double cImag = -1.5 + (double)linha / args->altura * 3.0;
            
            int iteracoes = calculoMandelbrot(cReal, cImag, args->maxIteracoes);
            unsigned char intensidade = intensidadePixels(iteracoes, args->maxIteracoes);
            args->buffer[linha * args->largura + px] = intensidade;
        }
    }
    
    return NULL;
}

void calcPthreads2(unsigned char *buffer, int largura, int altura, int maxIteracoes, int numThreads) {
    pthread_t *threads = malloc(numThreads * sizeof(pthread_t));
    MandelbrotArgs *args = malloc(numThreads * sizeof(MandelbrotArgs));
    
    if (!threads || !args) {
        fprintf(stderr, "Erro: falha ao alocar memoria para as threads!\n");
        free(threads);
        free(args);
        return;
    }
    
    pthread_mutex_t mutex;
    pthread_mutex_init(&mutex, NULL);
    
    int linhaAtual = 0;
    
    for (int i = 0; i < numThreads; i++) {
        args[i].buffer = buffer;
        args[i].largura = largura;
        args[i].altura = altura;
        args[i].maxIteracoes = maxIteracoes;
        args[i].numThreads = numThreads;
        args[i].threadId = i;
        
        args[i].mutex = &mutex;
        args[i].linhaAtual = &linhaAtual;
        
        pthread_create(&threads[i], NULL, threadFunc2, &args[i]);
    }
    
    for (int i = 0; i < numThreads; i++) {
        pthread_join(threads[i], NULL);
    }
    
    pthread_mutex_destroy(&mutex);
    
    free(threads);
    free(args);
}