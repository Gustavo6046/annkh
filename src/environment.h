#ifndef INC_ENVIRONMENT_H
#define INC_ENVIRONMENT_H

#include "layer.h"
#include "pool.h"
#include "net.h"

#define FITNESS_POOL_SIZE 16


struct e_trainer;
struct e_environment;

typedef void (*f_callback_trainer)(struct e_trainer *trainer);
typedef int (*f_callback_trainer_value)(struct e_trainer *trainer);
typedef void (*f_callback_env)(struct e_environment *env);

struct e_trainer_type {
    f_callback_trainer init, step, finished;
    f_callback_trainer_value is_done;
    f_callback_trainer deinit;
};

struct e_trainer {
    struct e_environment *env;
    struct e_trainer_type *type;
    struct n_network *reference;
    void *params;
    void *state;

    int epochs;
    int max_epochs;
    int active;
};

enum e_env_state {
    ENV_STARTED,    // e.g. agent still going
    ENV_DEAD,       // e.g. agent is dead and respawning
    ENV_STOPPED     // e.g. agent has despawned entirely
};

struct e_environment_type {
    f_callback_env start, step, on_respawn, on_stop;
};

struct e_environment {
    struct n_network *net;  // can be NULL
    struct e_trainer *trainer; // can be NULL

    float *inputs;
    float *outputs;

    struct e_environment_type *type;

    float fitness;
    enum e_env_state state;
    int steps;
};

void e_env_init(struct e_environment *env, struct e_environment_type *type, struct n_network *net, float *inputs, float *outputs);
void e_env_deinit(struct e_environment *env);
void e_env_init_trainer(struct e_environment *env, struct e_trainer_type *type, void *params);

void e_trainer_init(struct e_trainer *trainer, struct e_trainer_type *type, struct e_environment *env, void *params);
void e_trainer_deinit(struct e_trainer *trainer);

void e_trainer_use_net(struct e_trainer *trainer, struct n_network *net);

void e_env_call_network(struct e_environment *env);

void e_env_step(struct e_environment *env);

void e_env_training_activate(struct e_environment *env, int max_epochs);
void e_env_training_deactivate(struct e_environment *env);

void e_env_start(struct e_environment *env);
void e_env_died(struct e_environment *env);
void e_env_stop(struct e_environment *env);

void e_trainer_apply(struct e_trainer *trainer);

void e_env_loop(struct e_environment *env, int max_steps);


#endif // INC_ENVIRONMENT_H