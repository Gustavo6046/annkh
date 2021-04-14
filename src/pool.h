#ifndef INC_POOL_H
#define INC_POOL_H

#include <stdlib.h> // for NULL in macros


struct p_item {
    int occupied;
    int index;
    struct p_list *pool;
    void *data;
};

struct p_list {
    int item_size;
    int capacity;

    int first_free;
    int num_used;

    void *data;
    struct p_item *items;
    struct p_list *next;
    struct p_list *prev;
};

struct p_root {
    struct p_list *list;
    struct p_list *head;
    int head_index_offs;
    int item_size;
    int capacity;
};

#define POOL_ROOT(item_type, capacity) { NULL, NULL, sizeof(item_type), (capacity) }

void p_init_pool(struct p_list *pool, struct p_list *prev, int item_size, int capacity);
void p_free_pool(struct p_list *pool);

struct p_item *p_alloc_item(struct p_list *pool, int *index);
void p_free_item(struct p_item *item);

void p_root_initialize(struct p_root *root, int item_size, int capacity);
struct p_item *p_root_alloc_item(struct p_root *root, int *index);
void p_root_free_item(struct p_root *root, struct p_item *item);
struct p_item *p_root_get_item(struct p_root *root, int which);


#endif // INC_POOL_H