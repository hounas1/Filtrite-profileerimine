#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define INPUT_LENGTH 10000
#define NBQ          8

double biquadA[] = {0.9904270943918133,-1.988029537786207,0.9719128736135143,
                    -1.9695376181976625,0.954785151760269,-1.952430527435495,0.9396230207448886,-1.9372866598709455,
                    0.9269125440714382,-1.9245914935233015,0.9170422484899456,-1.9147330871451047,0.9103020778040724,
                    -1.9080010352900072,0.9068841759295282,-1.9045872504279573};
double biquadB[] = {1,2,1,2,1,2,1,2,1,2,1,2,1,2,1,2};
double gain = 7.384630392452121e+25;
double xyv[] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

double inputSignal[INPUT_LENGTH];

double GenerateRandomDouble(){

    int randomInt = rand();

    double randomDouble = ((double)randomInt / RAND_MAX) * 0.3;

    return randomDouble;
}

double ApplyIIR(double in){
    int i, b;
    int xp = 0;
    int yp = 3;
    int bqp = 0;

    double out = in / gain;

    for(i = 26; i > 0; i--) xyv[i] = xyv[i - 1];
    for(b = 0; b < NBQ; b++){
        int len = 2;
        xyv[xp] = out;
        for(i = 0; i < len; i++){
            out += xyv[xp + len - i] * biquadB[bqp + i] - xyv[yp + len - i] * biquadA[bqp + i];
        }
        bqp += len;
        xyv[yp] = out;
        xp = yp;
        yp += len + 1;
    }
    return out;
}


int main(){
    int i;
    double output;
    srand(time(NULL));

    for(i = 0; i < INPUT_LENGTH; i++){
        inputSignal[i] = GenerateRandomDouble();
    }

    clock_t begin = clock();
    for(i = 0; i < INPUT_LENGTH; i++){
        output = ApplyIIR(inputSignal[i]);
        printf("IN: %.12f   OUT:%.12f\n", inputSignal[i], output);
    }

    clock_t end = clock();
    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("---TIME: %lf ms---\n", time_spent * 1000);

    return 0;
}