//
// Created by vsevolod on 14.05.17.
//

#include <stddef.h>

#include "test_struct.h"
#include "mem_mgr.h"

const debug_typeid_t A_debug_typeid = 0;
const debug_typeid_t B_debug_typeid = 1;

A* A_new() {
    A* ret = (A*) add_element(A_debug_typeid, sizeof(A));
    return ret;
}

B* B_new() {
    B* ret = (B*) add_element(B_debug_typeid, sizeof(B));
    return ret;
}
