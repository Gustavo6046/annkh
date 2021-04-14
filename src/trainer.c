#include <stdlib.h>

#include "trainer.h"


void t_trainer_init(struct t_trainer *new_trainer, struct n_network *net) {
    new_trainer->net = net;
    new_trainer->fitness_pool.item_size = sizeof(struct t_fitness);
    new_trainer->fitness_pool.capacity = FITNESS_POOL_SIZE;
    //p_init_pool(&new_trainer->fitness_pool, NULL, sizeof(struct t_fitness), FITNESS_POOL_SIZE);
}
