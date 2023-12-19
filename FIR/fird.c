#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>

#include "headers/LowP95_FIR_512d.h"

#define INPUT_LENGTH 10000

double inputSignals[INPUT_LENGTH];

typedef struct{
    double buf[FILTER_LENGTH];
    int bufIndex;
    double out;
}FIRfilter;


double GenerateRandomDouble(){

    int randomInt = rand();

    double randomDouble = ((double)randomInt / RAND_MAX) * 0.3;

    return randomDouble;
}

void FIRfilter_init(FIRfilter *fir){
    for(int i = 0; i < FILTER_LENGTH; i++){
        fir->buf[i] = 0.0f;
    }

    fir->bufIndex = 0;

    fir->out = 0.0f;
}

double applyFilter(double input, FIRfilter *fir){
    clock_t enterFunc = clock();
    fir->buf[fir->bufIndex] = input;

    fir->bufIndex++;
    
    if(fir->bufIndex == FILTER_LENGTH) fir->bufIndex = 0;

    fir->out = 0.0f;

    int sumIndex = fir->bufIndex;

    for(int i = 0; i < FILTER_LENGTH; i++){
        if(sumIndex > 0) sumIndex--;
        else sumIndex = FILTER_LENGTH - 1;
        fir->out += Coeffs[i] * fir->buf[sumIndex];
    }
    clock_t exitFunc = clock();
    double funcTime = (double)(exitFunc - enterFunc) / CLOCKS_PER_SEC;
    //double funcTime = (double)(exitFunc - enterFunc);
    //printf("FUNCTIME: %lf ms\n", funcTime * 1000);

    return fir->out;
}

int main(){
    //clock_t begin = clock();
    srand(time(NULL));
    int i;
    double output;
    FIRfilter fir;


    for(i = 0; i < INPUT_LENGTH; i++){
        inputSignals[i] = GenerateRandomDouble();
    }

    FIRfilter_init(&fir);

    clock_t begin = clock();
    for(i = 0; i < INPUT_LENGTH; i++){
        output = applyFilter(inputSignals[i], &fir);
        //printf("[%d]OUT: %lf\n", i, output);
    }
    clock_t end = clock();
    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    //time_spent = (double)(end - begin);
    printf("---TIME: %lf ms---\n", time_spent * 1000);
    return 0;
}
