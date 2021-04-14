#ifndef INC_TRAINER_H
#define INC_TRAINER_H

#include "layer.h"
#include "pool.h"
#include "net.h"

#define FITNESS_POOL_SIZE 16


struct t_fitness {
    struct p_item *pool_item;
    struct l_layer *which;
    float *outputs;
    float *scores;
};

struct t_trainer {
    struct p_root fitness_pool;
    struct n_network *net;
};

void t_trainer_init(struct t_trainer *new_trainer, struct n_network *net);


#endif // INC_TRAINER_H