#pragma once

#include "pool.h"


struct pl_pool_list {
    struct p_root *items;
    struct p_root indices;
    int length;
    int middle_freed;
    int item_size;
};

struct pl_iter {
    struct pl_pool_list *list;
    void *item;
    int index;
    int to;
};


void pl_initialize(struct pl_pool_list *list, struct p_root *root, int segment_size);
void pl_deinit(struct pl_pool_list *list);

void *pl_allocate(struct pl_pool_list *list, int *index);
int pl_insert(struct pl_pool_list *list, void *item);
void *pl_get(struct pl_pool_list *list, int which);
int pl_has(struct pl_pool_list *list, int which);
void pl_free(struct pl_pool_list *list, int which);

struct pl_iter pl_iterate(struct pl_pool_list *list, int from, int to);
int pl_next(struct pl_iter *iterator);
int pl_iter_has(struct pl_iter *iterator);
