//
// Created by vsevolod on 14.05.17.
//

#ifndef C_MEM_MGR_TEST_STRUCT_H
#define C_MEM_MGR_TEST_STRUCT_H

#include <stdint.h>

#include "util.h"

extern const debug_typeid_t A_debug_typeid;
extern const debug_typeid_t B_debug_typeid;

typedef struct A {
    int memb1;
    double memb2;
    debug_typeid_t debug_typeid;
} A;

A* A_new ();

typedef struct B {
    uint64_t memb1;
    debug_typeid_t debug_typeid;
} B;

B* B_new ();

#endif //C_MEM_MGR_TEST_STRUCT_H
