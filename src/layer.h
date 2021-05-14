#pragma once


struct l_layer;

typedef void (*f_activation)(float *buffer, int size);

struct l_layer_type {
    void (*process)(struct l_layer *layer, float *inputs, float *outputs);
    void (*init)(struct l_layer *layer);
};

struct l_layer {
    struct l_layer_type *type;

    void *state;
    float *weights;

    float *input_buffer;
    float *output_buffer;

    f_activation activation;

    int state_size;
    int num_weights;
    int in_size;
    int out_size;
    int in_used;
};

void l_activ_relu(float *buffer, int size);
void l_activ_sigmoid_fast(float *buffer, int size);
void l_activ_softplus(float *buffer, int size);
void l_activ_softmax(float *buffer, int size);
void l_activ_none(float *buffer, int size);

extern struct l_layer_type layer_type_linear;
extern struct l_layer_type layer_type_quadratic;


void l_layer_init(struct l_layer *layer, struct l_layer_type *type, f_activation activation, int in_size, int out_size);
void l_layer_zero_weights(struct l_layer *layer);
void l_layer_deinit(struct l_layer *layer);

void l_layer_process(struct l_layer *layer);
