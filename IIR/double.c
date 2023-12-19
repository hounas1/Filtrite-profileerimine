#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define INPUT_LENGTH 10000
#define NBQ          1

double biquadA[] = {0.9329347317566122,-1.9306064272196681};
double biquadB[] = {1, 2};
double gain = 1717.9883200544907;
//double gain = 41.735483872083165;
double xyv[] = {0, 0, 0, 0, 0, 0};

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

    for(i = 5; i > 0; i--) xyv[i] = xyv[i - 1];
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
        //printf("IN: %f   OUT:%f\n", inputSignal[i], output);
    }

    clock_t end = clock();
    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("---TIME: %lf ms---\n", time_spent * 1000);

    return 0;
}