#include <stdlib.h>

#include "rng.h"


float rng_random_float(float size) {
    return (double) rand() * size / (double) RAND_MAX;
}
