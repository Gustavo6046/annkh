#ifndef INC_POOL_H
#define INC_POOL_H


struct p_item {
    int occupied;
    int index;
    struct p_info *pool;
    void *data;
};

struct p_info {
    int item_size;
    int capacity;

    int first_free;
    int num_used;

    void *data;
    struct p_item *items;
    struct p_info *next;
    struct p_info *prev;
};

void p_init_pool(struct p_info *pool, struct p_info *prev, int item_size, int capacity);
void p_free_pool(struct p_info *pool);

struct p_item *p_alloc_item(struct p_info *pool, int **index);
void p_free_item(struct p_item *item);


#endif // INC_POOL_H