#pragma once

#include "../environment.h"


struct ec_params {
    float *inputs;
    float *desired_outs;
    int input_size;
    int output_size;
    int num_examples;
};

struct ec_state {
    struct ec_params params;
    int which;
};

extern struct e_environment_type et_classifier;

#define EC_PARMS(insize, outsize, ins, outs) (ins), (outs), (insize), (outsize), (sizeof(ins)/sizeof(ins[0]))
