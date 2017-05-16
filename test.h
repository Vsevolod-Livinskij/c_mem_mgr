//
// Created by vsevolod on 16.05.17.
//

#ifndef C_MEM_MGR_TEST_H
#define C_MEM_MGR_TEST_H

void call_test();

void test_0 (); // Normal
void test_1 (); // Null set_member
void test_2 (); // Set after free
void test_3 (); // Set with uninit ptr
void test_4 (); // Cast to wrong type
void test_5 (); // Ptr comparison
void test_6 (); // Ptr arithmetic
void test_7 (); // Malloc without free
void test_8 (); // Init and terminate
void test_9 (); // Massive usage

#endif //C_MEM_MGR_TEST_H
