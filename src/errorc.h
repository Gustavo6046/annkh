#pragma once

#define ERR_RET(errsig) { return -errsig; }
#define CALL_RET(val) { int err = (val); if (!es_code_is_success(err)) { return err; } }
#define SUCCESS { return ES_SUCCESS; }
#define EMPTY_SUCCESS { return ES_SUCCESS_NO_VALUE; }


enum error_sig_t {
    ES_SUCCESS = 0,
    ES_SUCCESS_NO_VALUE,
    ES_GENERAL_FAILURE,
    ES_NET_CONN_INPUT_OUTPUT,
    ES_NET_CONN_NET_SIZE_MISMATCH,
    ES_NET_CONN_LAYER_EXCEEDS,
    ES_NET_PROC_NO_IN_LAYER,
    ES_NET_PROC_NO_OUT_LAYER,
    ES_ENV_ALREADY_STARTED,
};

typedef signed int error_code_t;

extern const char *const error_names[];

const char *const es_get_string(error_code_t code);
enum error_sig_t es_get_signal(error_code_t code);
int es_signal_is_success(enum error_sig_t sig);
int es_code_is_success(error_code_t code);
