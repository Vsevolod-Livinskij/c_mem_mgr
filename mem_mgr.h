//
// Created by vsevolod on 14.05.17.
//

#ifndef C_MEM_MGR_MEM_MGR_H
#define C_MEM_MGR_MEM_MGR_H

#include <stddef.h>

#include "util.h"

///////////////////////////////////////////////////////////////////////////////

extern const uint32_t DEFAULT_POOL_CHUNK_SIZE;

///////////////////////////////////////////////////////////////////////////////

typedef struct PoolChunk {
    void* data;
    struct PoolChunk* next_chunk;
} PoolChunk;

///////////////////////////////////////////////////////////////////////////////

typedef struct Pool {
    debug_typeid_t debug_typeid;
    size_t total_elem_num;
    size_t occupied_elem_num;
    size_t size_of_elem;
    PoolChunk* ptr_to_chunk;
    PoolChunk* ptr_to_last_chunk;
    struct Pool* next_pool;
} Pool;

Pool init_pool (debug_typeid_t debug_typeid, size_t data_size);
void free_pool (Pool* pool);
void* add_element_to_pool (Pool* pool);

void dump_raw_pool (Pool* pool);

///////////////////////////////////////////////////////////////////////////////

typedef struct MemoryManager {
    Pool* pool_list;
} MemoryManager;

MemoryManager memory_manager;

void init_memory_manager ();
void terminate_memory_manager ();
void* add_element (debug_typeid_t debug_typeid, size_t struct_size);


void dump_all_pools();

#endif //C_MEM_MGR_MEM_MGR_H
