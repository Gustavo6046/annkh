#include <math.h>
#include "t_evstrat.h"


static void t_evolve_strats_jitter(struct e_trainer *trainer, struct n_network *net) {
    struct t_state_evolve_strats *state = trainer->state;

    for (struct pl_iter li = pl_iterate(&net->layers, 0, -1); pl_next(&li);) {
        struct l_layer *layer = li.item;

        for (int i = 0; i < layer->num_weights; i++) {
            layer->weights[i] += bm_next(&state->boxmuller);
        }
    }
}

static void t_evolve_strats_make_population(struct e_trainer *trainer) {
    struct t_params_evolve_strats *params = trainer->params;
    struct t_state_evolve_strats *state = trainer->state;

    int population = params->population;

    struct n_network *pop_list = state->population;

    for (int i = 0; i < population; i++) {
        struct n_network *net = (pop_list + i);

        n_network_copy(trainer->reference, net);
        t_evolve_strats_jitter(trainer, net);
    }
}

static void t_evolve_strats_deinit_population(struct e_trainer *trainer) {
    struct t_params_evolve_strats *params = trainer->params;
    struct t_state_evolve_strats *state = trainer->state;

    int population = params->population;
    struct n_network *pop_list = state->population;

    for (int i = 0; i < population; i++) {
        struct n_network *net = (pop_list + i);

        n_network_deinit(net);
    }
}

static void t_evolve_strats_init(struct e_trainer *trainer) {
    struct t_params_evolve_strats *params = trainer->params;
    struct t_state_evolve_strats *state = trainer->state = malloc(sizeof(struct t_state_evolve_strats));

    int population = params->population;

    state->fitnesses = malloc(sizeof(float) * population);
    state->population = malloc(sizeof(struct n_network) * population);

    t_evolve_strats_make_population(trainer);

    trainer->env->net = &state->population[0];

    state->curr_steps = params->steps_per_pop;
    state->is_done = 0;
    state->current = 0;

    bm_state_init(&state->boxmuller, 0.0f, params->jitter_width);
}

static void t_evolve_strats_deinit(struct e_trainer *trainer) {
    struct t_state_evolve_strats *state = trainer->state;

    free(state->fitnesses);
    free(state->population);

    free(state);
}

static void t_evolve_strats_epoch(struct e_trainer *trainer) {
    struct t_params_evolve_strats *params = trainer->params;
    struct t_state_evolve_strats *state = trainer->state;

    // update weights
    int population = params->population;
    float alpha = params->alpha / params->population;

    struct n_network *dest_net = trainer->reference;

    // (precompute fitness products)
    float *fitnesses = malloc(sizeof(float) * params->population);

    for (int pi = 0; pi < population; pi++) {
        fitnesses[pi] = state->fitnesses[pi] * alpha;
    }

    for (struct pl_iter iter = pl_iterate(&dest_net->layers, 0, -1); pl_next(&iter);) {
        struct l_layer *dest_layer = iter.item;
        float *dest_weights = dest_layer->weights;

        for (int wi = 0; wi < dest_layer->num_weights; wi++) {
            dest_weights[wi] = 0.0;
        }

        for (int pi = 0; pi < population; pi++) {
            float fitness = fitnesses[pi];
            struct n_network *src_net = state->population + pi;
            struct l_layer *src_layer = pl_get(&src_net->layers, iter.index);
            float *src_weights = src_layer->weights;

            for (int wi = 0; wi < dest_layer->num_weights; wi++) {
                dest_weights[wi] += src_weights[wi] * fitness;
            }
        }
    }

    free(fitnesses);

    // reset population
    t_evolve_strats_deinit_population(trainer);
    t_evolve_strats_make_population(trainer);

    state->current = 0;
    state->is_done = 1;
}

static void t_evolve_strats_next(struct e_trainer *trainer) {
    struct t_params_evolve_strats *params = trainer->params;
    struct t_state_evolve_strats *state = trainer->state;

    state->curr_steps = params->steps_per_pop;
        
    if (state->current < params->population) {
        state->current++;

        trainer->env->net = &state->population[state->current];
    }

    else {
        t_evolve_strats_epoch(trainer);
    }
}

static void t_evolve_strats_step(struct e_trainer *trainer) {
    struct t_state_evolve_strats *state = trainer->state;

    state->fitnesses[state->current] = trainer->env->fitness;

    if (state->curr_steps > 0) {
        state->curr_steps--;
    }

    else {
        t_evolve_strats_next(trainer);
    }
}

static int t_evolve_strats_is_done(struct e_trainer *trainer) {
    return ((struct t_state_evolve_strats *) trainer->state)->is_done;
}

struct e_trainer_type t_trainer_evolve_strats = {
    t_evolve_strats_init,
    t_evolve_strats_step,
    t_evolve_strats_epoch,
    t_evolve_strats_is_done,
    t_evolve_strats_deinit
};
