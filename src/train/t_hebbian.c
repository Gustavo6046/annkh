#include <math.h>
#include "t_hebbian.h"


static void t_hebbian_step(struct e_trainer *trainer) {
    struct t_params_hebbian *params = trainer->params;
    float alpha = params->alpha;

    for (struct pl_iter iter = pl_iterate(&trainer->reference->layers, 0, -1); pl_next(&iter);) {
        struct l_layer *layer = (struct l_layer *) iter.item;

        if (layer->type == &layer_type_linear) {
            float *biases = layer->weights;
            float *weights = layer->weights + layer->out_size;

            for (int oi = 0; oi < layer->out_size; oi++) {
                biases[oi] += alpha * trainer->env->fitness * layer->output_buffer[oi];

                for (int ii = 0; ii < layer->in_size; ii++) {
                    weights[oi] += alpha * trainer->env->fitness * layer->input_buffer[ii] * layer->output_buffer[oi];
                }

                weights += layer->in_size;
            }
        }

        else if (layer->type == &layer_type_quadratic && params->update_quadratic) {
            float *biases = layer->weights;
            float *weights_1 = layer->weights + layer->out_size;
            float *weights_2 = weights_1 + layer->out_size * layer->in_size;

            for (int oi = 0; oi < layer->out_size; oi++) {
                biases[oi] += alpha * trainer->env->fitness * layer->output_buffer[oi];

                for (int ii = 0; ii < layer->in_size; ii++) {
                    weights_1[oi] += alpha * trainer->env->fitness * layer->input_buffer[ii] * layer->output_buffer[oi];
                    weights_2[oi] += alpha * trainer->env->fitness * sqrtf(layer->input_buffer[ii]) * layer->output_buffer[oi];
                }

                weights_1 += layer->in_size;
                weights_2 += layer->in_size;
            }
        }
    }
}

struct e_trainer_type t_trainer_hebbian = {
    NULL,
    t_hebbian_step,
    NULL,
    NULL
};
