#pragma once


struct bm_state {
    float a, b;
    float mu, sigma;
};

void bm_state_init(struct bm_state *state, float mu, float sigma);
float bm_next(struct bm_state *state);
