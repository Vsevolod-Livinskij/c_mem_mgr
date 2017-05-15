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
    for (int i = 0; i < 400; ++i) {
        A_new();
    }
    dump_all_pools();
    terminate_memory_manager();
    return 0;
}
