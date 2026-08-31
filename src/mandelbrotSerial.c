#include "base.h"

void calcSerial(unsigned char *buffer, int largura, int altura, int maxIteracoes){
    for (int py = 0; py < altura; py++){
        for (int px = 0; px < largura; px++){

            double cReal = -2.0 + (double)px / largura * 3.0;
            double cImag = -1.5 + (double)py / altura * 3.0;

            int iteracoes = calculoMandelbrot(cReal, cImag, maxIteracoes);

            unsigned char intensidade = intensidadePixels(iteracoes, maxIteracoes);
            buffer[py * largura + px] = intensidade;
        }
    }
}