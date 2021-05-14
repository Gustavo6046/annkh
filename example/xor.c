#include <stdio.h>

#include "environments/e_classifier.h"
#include "layer.h"
#include "net.h"


int main(void) {
    // step 1. make the reference network
    struct n_network net;

    n_network_init(&net, 2, 1);

    int l_middle = n_network_add_layer(&net, &layer_type_linear, &l_activ_sigmoid_fast, 2, 4);
    int l_out = n_network_add_layer(&net, &layer_type_linear, &l_activ_relu, 4, 1);

    n_network_connect_layers(&net, -1, l_middle);
    n_network_connect_layers(&net, l_middle, l_out);
    n_network_connect_layers(&net, l_out, -1);

    e_env_init(env, &et_classifier, &params, , float *inputs, float *outputs)

    // step 2. make the environment
    struct ec_params params = EC_PARMS(
        {
            {-1.0,  1.0},
            {1.0,   -1.0},
            {1.0,   1.0},
            {-1.0,  -1.0},
        },

        {
            {1.0},
            {1.0},
            {-1.0},
            {-1.0},
        }
    );

    struct e_environment env;


    return 0;
}
