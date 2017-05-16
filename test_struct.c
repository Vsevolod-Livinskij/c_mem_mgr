//
// Created by vsevolod on 14.05.17.
//

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

#include "test_struct.h"
#include "mem_mgr.h"

///////////////////////////////////////////////////////////////////////////////

const debug_typeid_t A_debug_typeid = 0;
const debug_typeid_t B_debug_typeid = 1;

///////////////////////////////////////////////////////////////////////////////

A* A_new() {
    A* ret = (A*) add_element(A_debug_typeid, sizeof(A));
    ret->debug_typeid =A_debug_typeid;
    return ret;
}

void A_delete(A *a) {
    remove_element(A_debug_typeid, a);
}

static bool A_is_ok(A* a) {
    if (a == NULL)
        ERROR_AND_ABORT("NULL ptr access");
    if (!is_ptr_in_use(A_debug_typeid, a))
        ERROR_AND_ABORT("Invalid access to pointer");
    return true;
}

void A_set_memb1(A *a, int value) {
    A_is_ok(a);
    a->memb1 = value;
}

int A_get_memb1(A *a) {
    A_is_ok(a);
    return a->memb1;
}

bool are_ptrs_to_A_equal(A *ptr1, A *ptr2) {
    return is_ptr_in_use(A_debug_typeid, ptr1) &&
           is_ptr_in_use(A_debug_typeid, ptr2) &&
           ptr1 == ptr2;
}

///////////////////////////////////////////////////////////////////////////////

B* B_new() {
    B* ret = (B*) add_element(B_debug_typeid, sizeof(B));
    ret->debug_typeid = B_debug_typeid;
    return ret;
}
void B_delete (B* b) {
    remove_element(B_debug_typeid, b);
}

static bool B_is_ok(B* b) {
    if (b == NULL)
        ERROR_AND_ABORT("NULL ptr access");
    if (!is_ptr_in_use(B_debug_typeid, b))
        ERROR_AND_ABORT("Invalid access to pointer");
    return true;
}

void B_set_memb1 (B* b, uint64_t value) {
    B_is_ok(b);
    b->memb1 = value;
}

uint64_t B_get_memb1 (B* b) {
    B_is_ok(b);
    return b->memb1;
}

