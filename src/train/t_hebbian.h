#ifndef INC_TRAIN_HEBBIAN_H
#define INC_TRAIN_HEBBIAN_H

#include "../environment.h"
#include "../poollist.h"
#include "../layer.h"


struct t_params_hebbian {
    float alpha;
    int update_quadratic;
};

extern struct e_trainer_type t_trainer_hebbian;


#endif //INC_TRAIN_HEBBIAN_H