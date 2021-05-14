#include <stdlib.h>
#include <math.h>

#include "layer.h"
#include "mathutil.h"


void l_activ_relu(float *buffer, int size) {
    mu_relu(buffer, size);
}

void l_activ_sigmoid_fast(float *buffer, int size) {
    mu_sigmoid_fast(buffer, size);
}

void l_activ_softplus(float *buffer, int size) {
    mu_softplus(buffer, size);
}

void l_activ_softmax(float *buffer, int size) {
    mu_softmax(buffer, size);
}

void l_activ_none(float *buffer, int size) {}

///////

// linear

static void layer_proc_linear(struct l_layer *layer, float *inputs, float *outputs) {
    float *biases = layer->weights;
    float *weights = biases + layer->out_size;

    int out_size = layer->out_size;
    int in_size = layer->in_size;

    f_activation activ = layer->activation;

    for (int to = 0; to < out_size; to++) {
        float accum = biases[to];

        for (int from = 0; from < in_size; from++) {
            accum += inputs[from] * weights[from];
        }

        outputs[to] = accum;
        weights += in_size;
    }

    if (activ) {
        activ(outputs, out_size);
    }
}

static void layer_init_linear(struct l_layer *layer) {
    layer->state = 0;
    layer->num_weights = layer->out_size * (/*biases*/ 1 + /*weights*/ layer->in_size);
}

// quadratic

static void layer_proc_quadratic(struct l_layer *layer, float *inputs, float *outputs) {
    float *biases = layer->weights;
    float *weights_1 = biases + layer->out_size;
    float *weights_2 = weights_1 + layer->out_size * layer->in_size;

    int out_size = layer->out_size;
    int in_size = layer->in_size;

    f_activation activ = layer->activation;

    for (int to = 0; to < out_size; to++) {
        float accum = biases[to];

        for (int from = 0; from < in_size; from++) {
            float x = inputs[from];
            accum += x * weights_1[from] + x * x * weights_2[from];
        }

        outputs[to] = accum;

        weights_1 += in_size;
        weights_2 += in_size;
    }

    activ(outputs, out_size);
}

static void layer_init_quadratic(struct l_layer *layer) {
    layer->state = 0;
    layer->num_weights = layer->out_size * (/*biases*/ 1 + /*weights*/ layer->in_size * 2);
}

///////

void l_layer_init(struct l_layer *layer, struct l_layer_type *type, f_activation activation, int in_size, int out_size) {
    layer->in_size = in_size;
    layer->out_size = out_size;
    layer->activation = activation;
    layer->type = type;
    layer->input_buffer = malloc(sizeof(float) * in_size);

    type->init(layer);

    if (layer->num_weights > 0) {
        layer->weights = malloc(sizeof(float) * layer->num_weights);
    }
}

void l_layer_zero_weights(struct l_layer *layer) {
    for (int wi = 0; wi < layer->num_weights; wi++) {
        layer->weights[wi] = 0.0f;
    }
}

void l_layer_deinit(struct l_layer *layer) {
    if (layer->weights != NULL) {
        free(layer->weights);
    }

    if (layer->state != NULL) {
        free(layer->state);
    }

    free(layer->input_buffer);
}

void l_layer_process(struct l_layer *layer) {
    layer->type->process(layer, layer->input_buffer, layer->output_buffer);
}


struct l_layer_type layer_type_linear = {
    layer_proc_linear,
    layer_init_linear
};

struct l_layer_type layer_type_quadratic = {
    layer_proc_quadratic,
    layer_init_quadratic
};
