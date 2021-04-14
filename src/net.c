#include "net.h"
#include "pool.h"


struct p_root all_layers = POOL_ROOT(struct l_layer, LAYER_POOL_SIZE);

void n_network_init(struct n_network *net) {
    p_root_initialize(&net->layers, sizeof(int), 16);
    p_root_initialize(&net->layer_connections, sizeof(struct n_layer_connection), 16);
}
