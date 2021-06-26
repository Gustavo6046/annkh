#include <math.h>
#include "mathutil.h"


const float two_pi = 2.0 * M_PI;

void mu_relu(float *buffer, int size) {
    for (int i = 0; i < size; i++) {
        if (buffer[i] < 0) {
            buffer[i] = 0;
        }
    }
}

void mu_sigmoid_fast(float *buffer, int size) {
    for (int i = 0; i < size; i++) {
        buffer[i] = 0.5f + buffer[i] / (2.0f + fabsf(buffer[i]) * 2.0f);
    }
}

void mu_softplus(float *buffer, int size) {
    for (int i = 0; i < size; i++) {
        buffer[i] = logf(1.0 + expf(buffer[i]));
    }
}

void mu_softmax(float *buffer, int size) {
    float val_max = -INFINITY;
    float val_sum = 0.0;

    for (int i = 0; i < size; i++) {
        if (buffer[i] > val_max) {
            val_max = buffer[i];
        }
    }

    for (int i = 0; i < size; i++) {
        buffer[i] = expf(buffer[i] - val_max);
        val_sum += buffer[i];
    }

    for (int i = 0; i < size; i++) {
        buffer[i] /= val_sum;
    }
}

////

float mu_operate(f_mu_operation oper, float x) {
    oper(&x, 1);
    return x;
}
