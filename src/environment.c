#include <stdlib.h>

#include "environment.h"


void e_env_init(struct e_environment *env, struct e_environment_type *type, struct n_network *net, float *inputs, float *outputs) {
    env->net = net;
    env->inputs = inputs;
    env->outputs = outputs;

    env->fitness = 0;
    env->state = ENV_STOPPED;
    env->steps = 0;

    env->type = type;
    env->trainer = NULL;
}

static void e_env_deinit_trainer(struct e_environment *env) {
    if (env->trainer) {
        e_trainer_apply(env->trainer);
        e_trainer_deinit(env->trainer);
        free(env->trainer);
    }
}

void e_trainer_apply(struct e_trainer *trainer) {
    trainer->env->net = trainer->reference;
}

void e_env_init_trainer(struct e_environment *env, struct e_trainer_type *type, void *params) {
    e_env_deinit_trainer(env);

    struct e_trainer *trainer = env->trainer = malloc(sizeof(struct e_trainer));
    
    e_trainer_init(trainer, type, env, params);
}

void e_env_deinit(struct e_environment *env) {
    e_env_deinit_trainer(env);
}

////

void e_trainer_init(struct e_trainer *trainer, struct e_trainer_type *type, struct e_environment *env, void *params) {
    trainer->env = env;
    trainer->epochs = 0;
    trainer->reference = env->net;
    trainer->type = type;
    trainer->active = 0;

    trainer->state = NULL;
    trainer->params = params;

    if (type->init) {
        type->init(trainer);
    }
}

void e_trainer_deinit(struct e_trainer *trainer) {
    if (trainer->type->deinit) {
        trainer->type->deinit(trainer);
    }

    trainer->env->trainer = NULL;
}

void e_trainer_use_net(struct e_trainer *trainer, struct n_network *net) {
    trainer->env->net = net;
}

////

void e_env_call_network(struct e_environment *env) {
    n_network_process_to(env->net, env->inputs, env->outputs);
}

void e_env_step(struct e_environment *env) {
    if (env->state == ENV_STARTED) {
        env->type->step(env);
    }

    struct e_trainer *trainer = env->trainer;

    if (trainer && trainer->active) {
        switch (env->state) {
            case ENV_STARTED:
                trainer->type->step(trainer);
                break;

            case ENV_DEAD:
                trainer->type->step(trainer);

                if (trainer->type->finished) {
                    trainer->type->finished(trainer);
                }
                break;

            case ENV_STOPPED:
                if (trainer->type->finished) {
                    trainer->type->finished(trainer);
                }
                break;
        }

        if (!trainer->type->is_done || trainer->type->is_done(trainer)) {
            trainer->epochs++;

            e_trainer_apply(trainer);
        }

        if (trainer->epochs > trainer->max_epochs) {
            e_env_training_deactivate(env);
        }
    }

    env->steps++;
}

void e_env_start(struct e_environment *env) {
    env->state = ENV_STARTED;
    env->type->start(env);
    env->steps = 0;
}

void e_env_respawn(struct e_environment *env) {
    env->state = ENV_STARTED;

    if (env->type->on_respawn) {
        env->type->on_respawn(env);
    }
}

void e_env_died(struct e_environment *env) {
    env->state = ENV_DEAD;
}

void e_env_stop(struct e_environment *env) {
    if (env->type->on_stop) {
        env->type->on_stop(env);
    }

    env->state = ENV_STOPPED;
    env->steps = 0;
    env->fitness = 0.0;
}

void e_env_training_activate(struct e_environment *env, int max_epochs) {
    env->trainer->active = 1;
    env->trainer->epochs = 0;
    env->trainer->max_epochs = max_epochs;
}

void e_env_training_deactivate(struct e_environment *env) {
    env->trainer->active = 0;
    env->trainer->epochs = 0;
}

error_code_t e_env_loop(struct e_environment *env, int max_steps) {
    if (env->state == ENV_STARTED) {
        ERR_RET(ES_ENV_ALREADY_STARTED);
    }

    e_env_start(env);

    while (env->state != ENV_STOPPED) {
        if (env->state == ENV_DEAD) {
            e_env_respawn(env);
        }

        if (env->state == ENV_STARTED) {
            e_env_step(env);

            if (env->steps > max_steps) {
                e_env_stop(env);
                break;
            }

            if (env->state == ENV_STOPPED) {
                e_env_stop(env);
                break;
            }
        }
    }

    SUCCESS;
}

void e_env_loopstep(struct e_environment *env, int max_steps) {
    if (env->state == ENV_STOPPED) {
        e_env_start(env);
    }

    if (env->state == ENV_DEAD) {
        e_env_respawn(env);
    }

    if (env->state == ENV_STARTED) {
        e_env_step(env);

        if (env->steps > max_steps) {
            e_env_stop(env);
            return;
        }
    }
}
