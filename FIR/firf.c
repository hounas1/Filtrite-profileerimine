#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>

#include "headers/LowP95_FIR_128f.h"

#define INPUT_LENGTH 10000

float inputSignals[INPUT_LENGTH];

typedef struct{
    float buf[FILTER_LENGTH];
    int bufIndex;
    float out;
}FIRfilter;

float GenerateRandomFloat(){

    int randomInt = rand();

    float randomFloat = ((float)randomInt / RAND_MAX) * 0.3;

    return randomFloat;
}

void FIRfilter_init(FIRfilter *fir){
    for(int i = 0; i < FILTER_LENGTH; i++){
        fir->buf[i] = 0.0f;
    }

    fir->bufIndex = 0;

    fir->out = 0.0f;
}

float applyFilter(float input, FIRfilter *fir){
    clock_t enterFunc = clock();

    static int n = 0;
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
    //printf("FUNCTIME: %lf ms\n", funcTime * 1000);

    return fir->out;
    
}

int main(){
 
    srand(time(NULL));
    int i;
    float output;
    FIRfilter fir;

    for(i = 0; i < INPUT_LENGTH; i++){
        inputSignals[i] = GenerateRandomFloat();
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
    printf("TIME: %lf ms\n", time_spent * 1000);
    return 0;
}
