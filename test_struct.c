//
// Created by vsevolod on 14.05.17.
//

#include <stddef.h>

#include "test_struct.h"
#include "mem_mgr.h"

const debug_typeid_t A_debug_typeid = 0;

A* A_new() {
    A* ret = (A*) add_element(A_debug_typeid, sizeof(A));
    return ret;
}
