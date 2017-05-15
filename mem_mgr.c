//
// Created by vsevolod on 14.05.17.
//

#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>

#include "mem_mgr.h"

///////////////////////////////////////////////////////////////////////////////

const uint32_t DEFAULT_POOL_CHUNK_SIZE = 1; // Data in chunk
const uint32_t DEFAULT_POOL_TABLE_SIZE = 1;

void init_memory_manager() {
    memory_manager.table_size = 0;
    memory_manager.pool_list = NULL;
}

void terminate_memory_manager() {
    Pool* cur_pool = memory_manager.pool_list;
    Pool* next_pool = NULL;
    do {
        if (cur_pool) {
            free_pool(cur_pool);
            next_pool = cur_pool->next_pool;
        }
        free(cur_pool);
        cur_pool = next_pool;
    } while (next_pool);
}

void* add_element(debug_typeid_t debug_typeid, size_t struct_size) {
    Pool* cur_pool = memory_manager.pool_list;
    size_t count = 0;
    while (cur_pool && cur_pool->next_pool && count < memory_manager.table_size) {
        if (cur_pool->debug_typeid == debug_typeid)
            return add_element_to_pool(cur_pool);
        cur_pool = cur_pool->next_pool;
        count++;
    }

    Pool* new_pool = calloc(1, sizeof(Pool));
    *new_pool = init_pool(debug_typeid, struct_size);
    if (cur_pool == NULL)
        memory_manager.pool_list = new_pool;
    else
        cur_pool->next_pool = new_pool;
    memory_manager.table_size++;
    return add_element_to_pool(new_pool);
}

void dump_all_pools() {
    Pool* cur_pool = memory_manager.pool_list;
    while (cur_pool) {
        dump_raw_pool(cur_pool);
        cur_pool = cur_pool->next_pool;
    }
}

///////////////////////////////////////////////////////////////////////////////

Pool init_pool(debug_typeid_t debug_typeid, size_t data_size) {
    Pool result;
    result.debug_typeid = debug_typeid;
    result.size_of_elem = sizeof(bool) + data_size + 2 * sizeof(uint32_t);
    result.total_elem_num = DEFAULT_POOL_CHUNK_SIZE;
    result.ptr_to_chunk = calloc(1, sizeof(PoolChunk));
    result.ptr_to_chunk->data = calloc(DEFAULT_POOL_CHUNK_SIZE, result.size_of_elem);
    result.ptr_to_last_chunk = result.ptr_to_chunk;
    result.occupied_elem_num = 0;
    result.next_pool = NULL;
    return result;
}

void free_pool(Pool* pool) {
    if (pool == NULL)
        ERROR("pool is NULL");

    PoolChunk* cur_chunk = pool->ptr_to_chunk;
    PoolChunk* next_chunk = NULL;
    do {
        if (cur_chunk) {
            free(cur_chunk->data);
            next_chunk = cur_chunk->next_chunk;
        }
        free(cur_chunk);
        cur_chunk = next_chunk;
    } while (cur_chunk);
}

void* add_element_to_pool (Pool* pool) {
    if (pool->total_elem_num == pool->occupied_elem_num) {
        PoolChunk* new_pool_chunk = calloc(1, sizeof(PoolChunk));
        new_pool_chunk->data = calloc(DEFAULT_POOL_CHUNK_SIZE, pool->size_of_elem);
        pool->ptr_to_last_chunk->next_chunk = new_pool_chunk;
        pool->ptr_to_last_chunk = new_pool_chunk;
        pool->total_elem_num += DEFAULT_POOL_CHUNK_SIZE;
    }

    void* ret = NULL;
    PoolChunk* cur_chunk = pool->ptr_to_chunk;
    while (cur_chunk) {
        for (int i = 0; i < DEFAULT_POOL_CHUNK_SIZE; ++i) {
            ret = cur_chunk->data + pool->size_of_elem * i;
            if (!*(bool *) ret) {
                *(bool *) ret = true;
                *(int *) (ret + sizeof(bool)) = 0xDEADBEEF;
                *(int *) (ret + pool->size_of_elem - sizeof(uint32_t)) = 0xDEADBEEF;
                pool->occupied_elem_num++;
                return ret + sizeof(bool) + sizeof(uint32_t);
            }
        }
        cur_chunk = cur_chunk->next_chunk;
    }
    assert("Whops...Chunk is Null");
}

void dump_raw_pool(Pool *pool) {
    printf("Pool start\n");
    PoolChunk* cur_chunk = pool->ptr_to_chunk;
    while (cur_chunk) {
        printf("Chunk start\n");
        for (int i = 0; i < DEFAULT_POOL_CHUNK_SIZE * pool->size_of_elem; ++i) {
            printf("%p : %x\n", cur_chunk->data, *(char*)(cur_chunk->data + i));
        }
        printf("Chunk end\n");
        cur_chunk = cur_chunk->next_chunk;
    }
    printf("Pool end\n");
}
