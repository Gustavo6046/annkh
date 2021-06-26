#include <string.h>
#include "poollist.h"


void pl_initialize(struct pl_pool_list *list, struct p_root *root, int segment_size) {
    list->length = 0;
    list->middle_freed = 0;
    list->items = root;
    list->item_size = root->item_size;

    p_root_initialize(&list->indices, sizeof(int), segment_size);
}

void pl_deinit(struct pl_pool_list *list) {
    for (int li = 0; li < list->length - list->middle_freed; li++) {
        if (p_root_has(&list->indices, li)) {
            p_root_free_at(list->items, *(int*) p_root_get_item(&list->indices, li)->data);
        }
    }

    p_root_empty(&list->indices);
    list->length = 0;
    list->middle_freed = 0;
}

void *pl_allocate(struct pl_pool_list *list, int *index) {
    int new_index = 0;
    struct p_item *item = p_root_alloc_item(list->items, &new_index);
    struct p_item *idx_item = p_root_alloc_item(&list->indices, index);
    *(int *)idx_item->data = new_index;

    if (list->middle_freed) {
        list->middle_freed--;
    }

    else {
        list->length++;
    }

    return item->data;
}

int pl_insert(struct pl_pool_list *list, void *item) {
    int index;
    void *dest = pl_allocate(list, &index);

    memcpy(dest, item, list->item_size);

    return index;
}

void *pl_get(struct pl_pool_list *list, int which) {
    return p_root_get_item(list->items, *(int*) p_root_get_item(&list->indices, which)->data)->data;
}

int pl_has(struct pl_pool_list *list, int which) {
    return which < list->length && p_root_has(&list->indices, which);
}

void pl_free(struct pl_pool_list *list, int which) {
    int item_idx = *(int*) p_root_get_item(&list->indices, which)->data;

    p_root_free_at(list->items, item_idx);
    p_root_free_at(&list->indices, which);

    list->middle_freed++;
}

struct pl_iter pl_iterate(struct pl_pool_list *list, int from, int to) {
    struct pl_iter res = {
        list,
        NULL,
        from,
        to,
    };

    if (pl_has(list, from)) {
        res.item = pl_get(list, from);
    }
    
    return res;
}

int pl_next(struct pl_iter *iterator) {
    do {
        if (iterator->index >= iterator->list->length) {
            iterator->index = -1;
            return 0;
        }

        if (iterator->to != -1 && iterator->index >= iterator->to) {
            iterator->index = -1;
            return 0;
        }

        iterator->index++;
    } while (!pl_has(iterator->list, iterator->index));

    iterator->item = pl_get(iterator->list, iterator->index);
    return 1;
}

int pl_iter_has(struct pl_iter *iterator) {
    return iterator->index != -1 && pl_has(iterator->list, iterator->index);
}

