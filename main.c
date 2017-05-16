//
// Created by vsevolod on 15.05.17.
//
#include "mem_mgr.h"
#include "test_struct.h"

int main () {
    init_memory_manager();
//    A* a = A_new();
//    printf("%p\n", a);
//    dump_all_pools();
//    a->memb1 = 0xAAAAAAAA;
//    dump_all_pools();
    for (int i = 0; i < 10; ++i) {
        A* a = A_new();
        a->memb1 = i;
        B* b = B_new();
        b->memb1 = 10 - i;
    }
    dump_all_pools();
    terminate_memory_manager();
    return 0;
}
