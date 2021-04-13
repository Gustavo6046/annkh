#include <math.h>

#include "softmax.h"


void softmax(float *buf, int size) {
    static float val_max = -INFINITY;
    static float val_sum = 0.0;

    for (int i = 0; i < size; i++) {
        if (buf[i] > val_max) val_max = buf[i];
    }

    for (int i = 0; i < size; i++) {
        buf[i] = expf(buf[i] - val_max);
        val_sum += buf[i];
    }

    for (int i = 0; i < size; i++) {
        buf[i] /= val_sum;
    }
}
