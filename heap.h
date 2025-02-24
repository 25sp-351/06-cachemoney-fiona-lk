#ifndef HEAP_H
#define HEAP_H

#include <stdint.h>

typedef int64_t heap_key_t;
typedef int64_t heap_value_t;
#define HEAP_KEY_FORMAT "%ld"
#define KEY_NOT_PRESENT -1

typedef int (*BoolFunc)(heap_key_t);
typedef heap_value_t (*ProviderFunction)(heap_key_t);
typedef void (*VoidFunc)(heap_key_t);

typedef struct {
    BoolFunc is_present;
    ProviderFunction get;
    VoidFunc insert;
} Provider;

typedef struct {
    heap_key_t key;
    heap_value_t value;
} heap_node_t;

typedef struct {
    heap_node_t *data;
    unsigned int size;
    unsigned int capacity;
} heap_t;

heap_t *heap_create(int capacity);
void heap_free(heap_t *heap);
unsigned int heap_size(heap_t *heap);
void heap_print(heap_t *heap);

void heap_insert(heap_t *heap, heap_key_t key, heap_value_t data);
heap_value_t heap_remove_min(heap_t *heap);
heap_value_t heap_find_key(heap_t *heap, heap_key_t aKey);
void heap_update_key(heap_t *heap, heap_key_t old_key, heap_key_t new_key);
void heap_update_value(heap_t *heap, heap_key_t key, heap_value_t new_data);

unsigned int heap_parent(unsigned int index);
unsigned int heap_left_child(unsigned int index);
unsigned int heap_right_child(unsigned int index);
unsigned int heap_level(unsigned int index);
unsigned int heap_find_key_index(heap_t *heap, heap_key_t aKey);

#endif 