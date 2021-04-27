#include <stdlib.h>

#include "rng.h"


float rng_random_float(float size) {
    return (double) rand() / ((double) (RAND_MAX) / size);
}
