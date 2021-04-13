#include <stdlib.h>

#include "pool.h"


void p_init_pool(struct p_info *pool, struct p_info *prev, int item_size, int capacity) {
    pool->data = malloc(item_size * capacity);
    pool->items = malloc(sizeof(struct p_item) * capacity);

    pool->item_size = item_size;
    pool->capacity = capacity;
    pool->first_free = 0;
    pool->num_used = 0;
    pool->next = NULL;

    if (prev) {
        pool->prev = prev;
        prev->next = pool;
    }

    else {
        pool->prev = NULL;
    }

    for (int i = 0; i < pool->capacity; i++) {
        pool->items[i].occupied = 0;
        pool->items[i].index = i;
        pool->items[i].pool = pool;
        pool->items[i].data = pool->data + i * pool->item_size;
    }
}


void p_free_pool(struct p_info *pool) {
    if (pool->next != NULL) {
        pool->next->prev = pool->prev;
    }

    if (pool->prev != NULL) {
        pool->prev->next = pool->next;
    }

    free(pool->items);
    free(pool->data);
}

struct p_item *p_alloc_item_in_pool(struct p_info *pool, int **index) {
    struct p_item *res = &pool->items[pool->first_free];
    *index += pool->first_free;

    pool->num_used++;

    if (pool->num_used == pool->capacity) {
        pool->first_free = pool->capacity;

        return res;
    }

    do {
        pool->first_free++;
    } while (pool->items[pool->first_free].occupied && pool->first_free < pool->capacity);

    return res;
}

struct p_item *p_alloc_item(struct p_info *pool, int **index) {
    if (index) {
        *index = 0;
    }

    while (pool) {
        if (pool->num_used < pool->capacity) {
            return p_alloc_item_in_pool(pool, index);
        }

        if (pool->next) {
            if (index) {
                *index += pool->capacity;
            }

            pool = pool->next;
        }
    }

    if (pool->num_used == pool->capacity) {
        // pool->next is NULL
        pool->next = malloc(sizeof(struct p_info));
        p_init_pool(pool->next, pool, pool->item_size, pool->capacity);
    }

    return p_alloc_item_in_pool(pool, index);
}

void p_free_item(struct p_item *item) {
    struct p_info *pool = item->pool;

    item->occupied = 0;

    if (item->index < pool->first_free) {
        pool->first_free = item->index;
    }

    if (pool->num_used == 0 && pool->prev) {
        pool->prev->next = pool->next;

        if (pool->next) {
            pool->next->prev = pool->prev;
        }

        free(pool);
    }
}

struct p_item *p_get_item(struct p_info *pool, int index) {
    int after = index / pool->capacity;
    index %= pool->capacity;

    while (after--) {
        pool = pool->next;

        if (!pool) {
            return NULL;
        }
    }

    return &pool->items[index];
}
