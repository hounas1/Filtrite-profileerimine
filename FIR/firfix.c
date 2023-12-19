#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#include "headers/LowP95_FIR_128d.h"

#define INPUT_LENGTH 10000
#define WORD_LENGTH 24

double inputSignals[INPUT_LENGTH];

unsigned int fixedCoeffs[FILTER_LENGTH];
unsigned int fixedInputs[INPUT_LENGTH];

typedef struct{
    unsigned int buf[FILTER_LENGTH];
    int bufIndex;
    unsigned int out;
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

int DoubleToFixed(double input){
    int integerPart = 0, fractionalPart = 0;
    int i;
    float t;

    integerPart = (int)floor(input) << WORD_LENGTH;
    input -= (int)floor(input);

    t = 0.5;

    for(i = 0; i < WORD_LENGTH; i++){
        if((input - t) >= 0){
            input -= t;
            fractionalPart += (1 << (WORD_LENGTH - 1 - i));
        }
        t = t / 2;
    }

    return integerPart + fractionalPart;
}

double FixedToDouble(int input){
    const double scaleFactor = 1 << WORD_LENGTH;
    return input / scaleFactor;
}

int applyFilter(int input,  FIRfilter *fir){
    fir->buf[fir->bufIndex] = input;
    fir->bufIndex++;

    if(fir->bufIndex == FILTER_LENGTH) fir->bufIndex = 0;
    
    fir->out = 0;

    int sumIndex = fir->bufIndex;

    for(int i = 0; i < FILTER_LENGTH; i++){
        if(sumIndex > 0) sumIndex--;
        else sumIndex = FILTER_LENGTH - 1;
        fir->out += (fixedCoeffs[i] * fir->buf[sumIndex]) >> WORD_LENGTH;
    }
    return fir->out;
}

int main(){
    srand(time(NULL));
    int i;
    float output;

    FIRfilter fir;

    for(i = 0; i < INPUT_LENGTH; i++){
        inputSignals[i] = GenerateRandomDouble();
    }

    //  for(i = 0; i < INPUT_LENGTH; i++){
    //     fixedInputs[i] = DoubleToFixed(inputSignals[i]);
    // }

    for(i = 0; i < FILTER_LENGTH; i++){
        fixedCoeffs[i] = DoubleToFixed(Coeffs[i]);
    }

    FIRfilter_init(&fir);

    clock_t begin = clock();
    for(i = 0; i < INPUT_LENGTH; i++){
        output = FixedToDouble(applyFilter(DoubleToFixed(inputSignals[i]), &fir));
        //printf("%f\n", output);
    }

    clock_t end = clock();
    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("---TIME: %lf ms---\n", time_spent * 1000);

    return 0;
}