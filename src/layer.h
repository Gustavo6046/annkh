#ifndef INC_layer_H
#define INC_layer_H



struct l_layer;

typedef void (*f_activation)(float *buffer, int size);
typedef void (*f_process)(struct l_layer *layer, float *inputs, float *outputs);
typedef void (*f_init)(struct l_layer *layer);

struct l_layer_type {
    f_process process;
    f_init init;
};

struct l_layer {
    int state_size;
    int num_weights;
    int in_size;
    int out_size;

    struct l_layer_type *type;
    f_activation activation;

    void *state;
    float *weights;
};

void l_activ_relu(float *buffer, int size);
void l_activ_sigmoid_fast(float *buffer, int size);
void l_activ_softplus(float *buffer, int size);
void l_activ_softmax(float *buffer, int size);

extern struct l_layer_type layer_type_linear;
extern struct l_layer_type layer_type_quadratic;


void l_layer_init(struct l_layer *layer, struct l_layer_type *type, f_activation activation, int in_size, int out_size);
void l_layer_deinit(struct l_layer *layer);

void l_layer_process(struct l_layer *layer, float *inputs, float *outputs);


#endif // INC_layer_H
