#include <stdlib.h>

#include "trainer.h"


void t_trainer_init(struct t_trainer *new_trainer, struct n_network *net) {
    new_trainer->net = net;
    p_init_pool(&new_trainer->fitness_pool, NULL, sizeof(struct t_fitness), FITNESS_POOL_SIZE);
}
