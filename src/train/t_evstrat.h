#pragma once


#include "../environment.h"
#include "../poollist.h"
#include "../layer.h"
#include "../boxmuller.h"


struct t_params_evolve_strats {
    float alpha;
    float jitter_width;
    int population;
    int steps_per_pop;
};

struct t_state_evolve_strats {
    struct n_network *population;
    float *fitnesses;
    int current;
    int curr_steps;
    int is_done;
    struct bm_state boxmuller;
};

extern struct e_trainer_type t_trainer_evolve_strats;
