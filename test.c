//
// Created by vsevolod on 16.05.17.
//

#include <stdio.h>
#include <stdlib.h>
#include <search.h>

#include "test.h"
#include "mem_mgr.h"
#include "test_struct.h"

#ifndef TEST_NUM
    #define TEST_NUM 0
#endif

#define CAT_TMP(a,b) a##b
#define CAT(a,b) CAT_TMP(a, b)
#define CALL_CHOSEN_TEST do { CAT(test_, TEST_NUM)(); } while (0)

#define SHOULD_FAIL do { printf("Should fail\n"); } while (0)
#define SHOULD_NOT_FAIL do { printf("Should NOT fail\n"); } while (0)

void call_test() {
    CALL_CHOSEN_TEST;
}

void test_0() {
    SHOULD_NOT_FAIL;
    init_memory_manager();
    A* a = A_new();
    A_set_memb1(a, 10);
    printf("%d\n", A_get_memb1(a));
    A_delete(a);
    terminate_memory_manager();
}

void test_1() {
    SHOULD_FAIL;
    init_memory_manager();
    A_set_memb1(NULL, 101);
    terminate_memory_manager();
}

void test_2() {
    SHOULD_FAIL;
    init_memory_manager();
    A* a = A_new();
    A_set_memb1(a, 101);
    printf("%d\n", A_get_memb1(a));
    A_delete(a);
    A_set_memb1(a, 202);
    terminate_memory_manager();
}

void test_3() {
    SHOULD_FAIL;
    init_memory_manager();
    A* a = (A*) 0x01;
    A_set_memb1(a, 101);
    terminate_memory_manager();
}

void test_4() {
    SHOULD_FAIL;
    init_memory_manager();
    A* a = A_new();
    A_set_memb1(a, 101);
    printf("%d\n", A_get_memb1(a));
    B* b = (B*) a;
    B_set_memb1(b, 202);
    printf("%lu\n", B_get_memb1(b));
    terminate_memory_manager();
}

void test_5() {
    SHOULD_NOT_FAIL;
    init_memory_manager();

    A* a1 = A_new();
    A* a2 = a1;
    printf("Equal: %d\n", are_ptrs_to_A_equal(a1, a2));
    fflush(stdout);
    A_delete(a1);

    A* a3 = A_new();
    A* a4 = A_new();
    printf("Not equal #1: %d\n", are_ptrs_to_A_equal(a3, a4));
    fflush(stdout);
    A_delete(a3);
    A_delete(a4);

    A* a5 = A_new();
    B* b2 = B_new();
    printf("Not equal #2: %d\n", are_ptrs_to_A_equal(a5, (A*) b2));
    fflush(stdout);
    A_delete(a5);
    B_delete(b2);

    A* a6 = A_new();
    printf("Not equal #3: %d\n", are_ptrs_to_A_equal(a6, NULL));
    fflush(stdout);
    A_delete(a6);

    A* a7 = A_new();
    printf("Not equal #4: %d\n", are_ptrs_to_A_equal(a7, (void*) 0x1));
    fflush(stdout);
    A_delete(a7);

    A* a8 = A_new();
    A* a9 = A_new();
    A_set_memb1(a9, 1);
    A_delete(a9);
    printf("Not equal #5: %d\n", are_ptrs_to_A_equal(a8, a9));
    fflush(stdout);
    A_delete(a8);

    terminate_memory_manager();
}

void test_6() {
    SHOULD_FAIL;
    init_memory_manager();
    A* a = A_new();
    A_set_memb1(a, 10);
    printf("%d\n", A_get_memb1(a));
    *(int*)(a + 1) = 100;
    printf("%d\n", A_get_memb1(a));
    terminate_memory_manager();
}

void test_7() {
    SHOULD_FAIL;
    init_memory_manager();
    A* a = A_new();
    A_set_memb1(a, 101);
    printf("%d\n", A_get_memb1(a));
    terminate_memory_manager();
}

void test_8() {
    SHOULD_NOT_FAIL;
    init_memory_manager();
    terminate_memory_manager();
}

void test_9() {
    SHOULD_NOT_FAIL;
    init_memory_manager();
    int size = 100;
    A* a [size];
    B* b [size];
    for (int i = 0; i < size; ++i) {
        a [i] = A_new();
        A_set_memb1(a[i], i);
        b [i] = B_new();
        B_set_memb1(b[i], size - i);
    }
    dump_all_pools();
    for (int i = 0; i < size; ++i) {
        A_delete(a[i]);
        B_delete(b[i]);
    }
    terminate_memory_manager();
}

