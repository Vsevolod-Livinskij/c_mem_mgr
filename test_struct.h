//
// Created by vsevolod on 14.05.17.
//

#ifndef C_MEM_MGR_TEST_STRUCT_H
#define C_MEM_MGR_TEST_STRUCT_H

#include <stdint.h>

#include "util.h"

///////////////////////////////////////////////////////////////////////////////

extern const debug_typeid_t A_debug_typeid;
extern const debug_typeid_t B_debug_typeid;

///////////////////////////////////////////////////////////////////////////////

typedef struct A {
    int memb1;
    debug_typeid_t debug_typeid;
} A;

A* A_new ();
void A_delete (A* a);
void A_set_memb1 (A* a, int value);
int A_get_memb1 (A* a);
bool are_ptrs_to_A_equal(A* ptr1, A* ptr2);

///////////////////////////////////////////////////////////////////////////////

typedef struct B {
    uint64_t memb1;
    debug_typeid_t debug_typeid;
} B;

B* B_new ();
void B_delete (B* b);
void B_set_memb1 (B* b, uint64_t value);
uint64_t B_get_memb1 (B* b);

#endif //C_MEM_MGR_TEST_STRUCT_H
