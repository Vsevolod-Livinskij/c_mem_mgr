//
// Created by vsevolod on 15.05.17.
//

#ifndef C_MEM_MGR_UTIL_H
#define C_MEM_MGR_UTIL_H

#include <stdio.h>
#include <stdint.h>

typedef uint32_t debug_typeid_t;

#define ERROR(err_message) \
    do { fprintf(stderr,"%s:%d, function %s(): " err_message "\n",\
    __FILE__, __LINE__, __PRETTY_FUNCTION__); } while (0)

#endif //C_MEM_MGR_UTIL_H
