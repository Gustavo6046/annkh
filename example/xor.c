#include <stdio.h>
#include <fenv.h>

#include "layer.h"
#include "net.h"
#include "errorc.h"
#include "environments/e_classifier.h"
#include "train/t_evstrat.h"

#define NUM_EPOCHS 10
#define POP_SIZE 20
#define NUM_STEPS NUM_EPOCHS * POP_SIZE * 4


void cleanup(struct e_environment *env, struct n_network *net) {
    printf("Execution status: %s\n\n", errorval);

    if (net) {
        n_network_deinit(net);
    }

    if (env) {
        if (env->net && (env->net != net)) {
            n_network_deinit(env->net);
        }

        e_env_deinit(env);
    }
}

int main(void) {
    feenableexcept(FE_ALL_EXCEPT & ~FE_INEXACT);

    // step 1. make the reference network
    fprintf(stderr, " * Initializing network\n");

    struct n_network net;

    n_network_init(&net, 2, 1);

    fprintf(stderr, "   - Middle layer definition");
    int l_middle = n_network_add_layer(&net, &layer_type_linear, &l_activ_sigmoid_fast, 2, 4, NULL);
    fprintf(stderr, " (%d)\n", l_middle);

    fprintf(stderr, "   - Out layer definition");
    int l_out = n_network_add_layer(&net, &layer_type_linear, &l_activ_softmax, 4, 1, NULL);
    fprintf(stderr, " (%d)\n", l_out);

    fprintf(stderr, "   - Layer connections\n");
    MAIN_CALL(n_network_connect_layers(&net, -1, l_middle),     cleanup(NULL, &net), 2);
    MAIN_CALL(n_network_connect_layers(&net, l_middle, l_out),  cleanup(NULL, &net), 2);
    MAIN_CALL(n_network_connect_layers(&net, l_out, -1),        cleanup(NULL, &net), 2);

    // step 2. make the environment
    fprintf(stderr, " * Initializing environment\n");

    float our_inputs[] = {
        1.0, -1.0,
        -1.0, 1.0,
        1.0, 1.0,
        -1.0, -1.0
    };

    float our_outputs[] = {
        1.0,
        1.0,
        -1.0,
        -1.0
    };

    struct ec_params params = {
        EC_PARMS(
            2, 1,

            our_inputs,
            our_outputs
        )
    };

    struct e_environment env;

    float inputs[2];
    float output;


    e_env_init(&env, &et_classifier, &params, &net, inputs, &output);

    // step 3. test our zero-initialized network

    fprintf(stderr, " * Run network for the first time\n");

    for (int i = 0; i < 4; i++) {
        inputs[0] = our_inputs[i * 2];
        inputs[1] = our_inputs[i * 2 + 1];

        n_network_process_to(&net, inputs, &output);

        fprintf(stderr, "   - Initial net result: (%f,%f) -> %f\n", inputs[0], inputs[1], output);
    }

    // step 4. make a trainer to redeem our network

    fprintf(stderr, " * Initialize trainer\n");

    struct t_params_evolve_strats trainparms = {
        .alpha = 0.3,
        .jitter_width = 0.3,
        .population = POP_SIZE,
        .steps_per_pop = 4
    };

    e_env_init_trainer(&env, &t_trainer_evolve_strats, &trainparms);

    e_env_training_activate(&env, NUM_STEPS + 1);

    fprintf(stderr, " * Call %d steps\n", NUM_STEPS);
    for (int i = 0; i < NUM_STEPS; i++) {
        e_env_loopstep(&env, NUM_STEPS);
        fprintf(stderr, "   - Env loop step #%d, inputs->output: (%f, %f) -> %f (fitness %f)\n", i + 1, env.steps, inputs[0], inputs[1], output, env.fitness);
    }

    // step 5. test our trained network

    fprintf(stderr, " * Run network for the last time\n");

    for (int i = 0; i < 4; i++) {
        inputs[0] = our_inputs[i * 2];
        inputs[1] = our_inputs[i * 2 + 1];

        n_network_process_to(env.net, inputs, &output);

        fprintf(stderr, "   - Final net result: (%f,%f) -> %f\n", inputs[0], inputs[1], output);
    }

    // finally, deinit

    cleanup(&env, &net);

    return 0;
}
