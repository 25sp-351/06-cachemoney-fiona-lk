#include "heap.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

// implement cache
#define CACHE_SIZE 1000
static heap_value_t value_cache[CACHE_SIZE] = {0};
static int is_computed[CACHE_SIZE]          = {0};
static heap_value_t current_result          = 0;

int is_present(heap_key_t key) {
    if (key < 0 || key >= CACHE_SIZE)
        return 0;
    return is_computed[key];
}

heap_value_t get_value_from_cache(heap_key_t key) {
    if (key < 0 || key >= CACHE_SIZE)
        return KEY_NOT_PRESENT;
    return value_cache[key];
}

void insert_value_to_cache(heap_key_t key) {
    if (key < 0 || key >= CACHE_SIZE)
        return;
    value_cache[key] = current_result;
    is_computed[key] = 1;
}

heap_value_t cached_operation(heap_key_t key, Provider *provider,
                              heap_value_t (*operation)(heap_key_t)) {
    if ((*provider->is_present)(key))
        return (*provider->get)(key);

    heap_value_t result = (*operation)(key);
    current_result      = result;
    (*provider->insert)(key);
    return result;
}

heap_t *heap_create(int capacity) {
    heap_t *heap   = malloc(sizeof(heap_t));
    heap->data     = malloc(sizeof(heap_node_t) * capacity);
    heap->size     = 0;
    heap->capacity = capacity;
    return heap;
}

void heap_free(heap_t *heap) {
    free(heap->data);
    free(heap);
}

unsigned int heap_size(heap_t *heap) {
    return heap->size;
}

unsigned int heap_parent(unsigned int index) {
    return (index - 1) / 2;
}

unsigned int heap_left_child(unsigned int index) {
    return 2 * index + 1;
}

unsigned int heap_right_child(unsigned int index) {
    return 2 * index + 2;
}

unsigned int heap_level(unsigned int index) {
    return (int)(log2(index + 1));
}

void heap_print(heap_t *heap) {
    for (int ix = 0; ix < heap_size(heap); ix++) {
        printf("%3d - %3d : " HEAP_KEY_FORMAT " (val %lld)\n", heap_level(ix),
               ix, heap->data[ix].key, heap->data[ix].value);
    }
    printf("\n");
}

void heap_swap(heap_t *heap, int index1, int index2) {
    heap_node_t temp   = heap->data[index1];
    heap->data[index1] = heap->data[index2];
    heap->data[index2] = temp;
}

void heap_bubble_up(heap_t *heap, int index) {
    if (index == 0)
        return;

    int parent = heap_parent(index);
    if (heap->data[parent].key > heap->data[index].key) {
        heap_swap(heap, parent, index);
        heap_bubble_up(heap, parent);
    }
}

void heap_bubble_down(heap_t *heap, int index) {
    int min_index = index;
    int left      = heap_left_child(index);
    int right     = heap_right_child(index);

    if (left < heap_size(heap) &&
        heap->data[left].key < heap->data[min_index].key) {
        min_index = left;
    }

    if (right < heap_size(heap) &&
        heap->data[right].key < heap->data[min_index].key) {
        min_index = right;
    }

    if (min_index != index) {
        heap_swap(heap, index, min_index);
        heap_bubble_down(heap, min_index);
    }
}

void heap_insert(heap_t *heap, heap_key_t key, heap_value_t data) {
    if (heap_size(heap) == heap->capacity)
        return;

    Provider provider                 = {.is_present = &is_present,
                                         .get        = &get_value_from_cache,
                                         .insert     = &insert_value_to_cache};

    heap->data[heap_size(heap)].key   = key;
    heap->data[heap_size(heap)].value = data;
    current_result                    = data;
    provider.insert(key);
    heap->size++;

    heap_bubble_up(heap, heap_size(heap) - 1);
}

heap_value_t heap_remove_min(heap_t *heap) {
    if (heap_size(heap) == 0)
        return KEY_NOT_PRESENT;

    heap_value_t min = heap->data[0].value;
    heap->size--;
    heap->data[0] = heap->data[heap_size(heap)];
    heap_bubble_down(heap, 0);

    return min;
}

unsigned int heap_find_key_index(heap_t *heap, heap_key_t aKey) {
    Provider provider = {.is_present = &is_present,
                         .get        = &get_value_from_cache,
                         .insert     = &insert_value_to_cache};

    if (provider.is_present(aKey)) {
        for (unsigned int ix = 0; ix < heap_size(heap); ix++)
            if (heap->data[ix].key == aKey)
                return ix;
    }

    for (unsigned int ix = 0; ix < heap_size(heap); ix++) {
        if (heap->data[ix].key == aKey) {
            current_result = ix;
            provider.insert(aKey);
            return ix;
        }
    }
    return KEY_NOT_PRESENT;
}

heap_value_t heap_find_key(heap_t *heap, heap_key_t aKey) {
    Provider provider = {.is_present = &is_present,
                         .get        = &get_value_from_cache,
                         .insert     = &insert_value_to_cache};

    if (provider.is_present(aKey))
        return provider.get(aKey);

    int index = heap_find_key_index(heap, aKey);
    if (index == KEY_NOT_PRESENT)
        return KEY_NOT_PRESENT;

    current_result = heap->data[index].value;
    provider.insert(aKey);
    return heap->data[index].value;
}

void heap_update_key(heap_t *heap, heap_key_t old_key, heap_key_t new_key) {
    Provider provider = {.is_present = &is_present,
                         .get        = &get_value_from_cache,
                         .insert     = &insert_value_to_cache};

    for (int i = 0; i < heap->size; i++) {
        if (heap->data[i].key == old_key) {
            heap->data[i].key = new_key;
            current_result    = heap->data[i].value;
            provider.insert(new_key);

            if (new_key > old_key)
                heap_bubble_down(heap, i);
            else
                heap_bubble_up(heap, i);
            return;
        }
    }
}

void heap_update_value(heap_t *heap, heap_key_t key, heap_value_t new_data) {
    Provider provider = {.is_present = &is_present,
                         .get        = &get_value_from_cache,
                         .insert     = &insert_value_to_cache};

    for (int i = 0; i < heap->size; i++) {
        if (heap->data[i].key == key) {
            heap->data[i].value = new_data;
            current_result      = new_data;
            provider.insert(key);
            return;
        }
    }
}