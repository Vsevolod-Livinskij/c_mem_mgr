//
// Created by vsevolod on 15.05.17.
//

#ifndef C_MEM_MGR_UTIL_H
#define C_MEM_MGR_UTIL_H

#include <stdio.h>
#include <stdint.h>

typedef uint32_t debug_typeid_t;

#define ERROR_AND_ABORT(err_message) \
    do { fprintf(stderr,"%s:%d, function %s(): %s \n",\
    __FILE__, __LINE__, __PRETTY_FUNCTION__, err_message); abort(); } while (0)

#endif //C_MEM_MGR_UTIL_H
