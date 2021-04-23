#include "errorc.h"


const char *const error_names[] = {
    "Success",
    "Success without return value",
    "Unknown failure",
    "Tried connecting network input and output directly, which is invalid",
    "Source/destination layer does not match network's defined input/output size",
    "Source layer's output does not fit after occupied inputs of destination layer",
    "Network has no input layer connection to feed values into",
    "Network has no output layer connection to fetch values from",
    "Environment already started; can only call e_env_loop once at a time"
};


const char *const es_get_string(error_code_t code) {
    return error_names[-code];
}

enum error_sig_t es_get_signal(error_code_t code) {
    return (enum error_sig_t) -code;
}

int es_signal_is_success(enum error_sig_t sig) {
    return sig <= ES_SUCCESS_NO_VALUE;
}

int es_code_is_success(error_code_t code) {
    return es_signal_is_success(es_get_signal(code));
}
