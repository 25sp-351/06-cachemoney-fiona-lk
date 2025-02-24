#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "heap.h"

/*
Assignment 2: integer minheap
*/

unsigned long long rand_between(unsigned long long min, unsigned long long max)
{
    unsigned long long range = max - min;
    return min + (rand() % range);
}

void test_heap(void)
{
    heap_t* heap = heap_create(200);
    for (heap_key_t ix = 0; ix < 20; ix++) {
        heap_key_t key = rand_between(0, 1000);
        heap_insert(heap, key, (heap_value_t) key*5 );
        heap_print(heap);
    }
    for (int ix = 0; ix < 10; ix++) {
        heap_value_t val = heap_remove_min(heap);
        printf("Removed %d\n", val);
        heap_print(heap);
    }

    for (int ix = 0; ix < 10; ix++) {
        // for look so we're not randomly probing forever
        for (heap_key_t key = rand_between(0, 1000); key < 1000; key++) {
            if (heap_find_key(heap, key) == KEY_NOT_PRESENT) {
                continue;
            }
            heap_key_t new_key = rand_between(0, 1000);
            heap_update_key(heap, key, new_key);

            printf("Updated %llu to %llu\n", key, new_key);
            heap_print(heap);
            break;
        }
    }
    heap_free(heap);
    exit(0);
}
int main(int argc, char* argv[])
{
    srand(time(NULL));

    test_heap();
}
