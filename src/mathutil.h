#pragma once


typedef void (*f_mu_operation)(float *buffer, int size);

void mu_relu(float *buffer, int size);
void mu_sigmoid_fast(float *buffer, int size);
void mu_softplus(float *buffer, int size);
void mu_softmax(float *buffer, int size);

float mu_operate(f_mu_operation oper, float x);
