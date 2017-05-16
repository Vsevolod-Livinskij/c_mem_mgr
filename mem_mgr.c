//
// Created by vsevolod on 14.05.17.
//

#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>

#include "mem_mgr.h"

///////////////////////////////////////////////////////////////////////////////

#define GET_IS_OCCUPIED(ptr) (*(bool *) ptr)
#define GET_START_BUF(ptr) (*(int *) (ptr + sizeof(bool)))
#define GET_DATA_PTR(ptr) (ptr + sizeof(bool) + sizeof(uint32_t))
#define GET_END_BUF(ptr, pool) (*(int *) (ptr + (pool->size_of_elem - sizeof(uint32_t))))

const uint32_t guard_value = 0xDEADBEEF;

const uint32_t DEFAULT_POOL_CHUNK_SIZE = 3; // Data in chunk

void init_memory_manager() {
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
    while (cur_pool) {
        if (cur_pool->debug_typeid == debug_typeid)
            return add_element_to_pool(cur_pool);
        if (cur_pool->next_pool == NULL)
            break;
        cur_pool = cur_pool->next_pool;
    }

    Pool* new_pool = calloc(1, sizeof(Pool));
    *new_pool = init_pool(debug_typeid, struct_size);
    if (cur_pool == NULL)
        memory_manager.pool_list = new_pool;
    else
        cur_pool->next_pool = new_pool;
    return add_element_to_pool(new_pool);
}

void remove_element(debug_typeid_t debug_typeid, void *ptr_to_struct) {
    Pool* cur_pool = memory_manager.pool_list;
    while (cur_pool) {
        if (cur_pool->debug_typeid == debug_typeid) {
            remove_element_from_pool(cur_pool, ptr_to_struct);
            return;
        }
        if (cur_pool->next_pool == NULL)
            break;
        cur_pool = cur_pool->next_pool;
    }
    ERROR_AND_ABORT("Pool doesn't exist!");
}

void dump_all_pools() {
    Pool* cur_pool = memory_manager.pool_list;
    while (cur_pool) {
        dump_raw_pool(cur_pool);
        cur_pool = cur_pool->next_pool;
    }
}

bool is_ptr_in_use(debug_typeid_t debug_typeid, void *ptr_to_struct) {
    if (ptr_to_struct == NULL)
        return false;
    Pool* cur_pool = memory_manager.pool_list;
    while (cur_pool) {
        if (cur_pool->debug_typeid == debug_typeid) {
            return is_ptr_belongs_to_pool(cur_pool, ptr_to_struct);
        }
        if (cur_pool->next_pool == NULL)
            break;
        cur_pool = cur_pool->next_pool;
    }
    ERROR_AND_ABORT("Pool doesn't exist!");
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
        ERROR_AND_ABORT("pool is NULL");

    PoolChunk* cur_chunk = pool->ptr_to_chunk;
    PoolChunk* next_chunk = NULL;
    void* cur_data_ptr = NULL;
    do {
        if (cur_chunk) {
            for (int i = 0; i < DEFAULT_POOL_CHUNK_SIZE; ++i) {
                cur_data_ptr = cur_chunk->data + pool->size_of_elem * i;
                if (GET_IS_OCCUPIED(cur_data_ptr))
                    fprintf(stderr, "You have allocated memory at exit: %p\n", GET_DATA_PTR(cur_data_ptr));
            }
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
    assert(pool->ptr_to_chunk);
    PoolChunk* cur_chunk = pool->ptr_to_chunk;
    while (cur_chunk) {
        for (int i = 0; i < DEFAULT_POOL_CHUNK_SIZE; ++i) {
            ret = cur_chunk->data + pool->size_of_elem * i;
            if (!GET_IS_OCCUPIED(ret)) {
                GET_IS_OCCUPIED(ret) = true;
                GET_START_BUF(ret) = guard_value;
                GET_END_BUF(ret, pool) = guard_value;
                pool->occupied_elem_num++;
                return GET_DATA_PTR(ret);
            }
        }
        cur_chunk = cur_chunk->next_chunk;
    }
    ERROR_AND_ABORT("Whops...Chunk is Null");
}

void remove_element_from_pool (Pool* pool, void* ptr_to_struct) {
    if (ptr_to_struct == NULL)
        return;

    void* cur_ptr = NULL;
    PoolChunk* cur_chunk = pool->ptr_to_chunk;
    while (cur_chunk) {
        for (int i = 0; i < DEFAULT_POOL_CHUNK_SIZE; ++i) {
            cur_ptr = cur_chunk->data + pool->size_of_elem * i;
            if (GET_DATA_PTR(cur_ptr) == ptr_to_struct) {
                if (!GET_IS_OCCUPIED(cur_ptr)) {
                    char buffer [50];
                    sprintf(buffer, "Double free: %p", GET_DATA_PTR(cur_ptr));
                    ERROR_AND_ABORT(buffer);
                }
                GET_IS_OCCUPIED(cur_ptr) = false;
                return;
            }
        }
        cur_chunk = cur_chunk->next_chunk;
    }
    char buffer [50];
    sprintf(buffer, "Can't find ptr in any pool: %p", ptr_to_struct);
    ERROR_AND_ABORT(buffer);
}

void dump_raw_pool(Pool *pool) {
    printf("Pool start: %d\n", pool->debug_typeid);
    PoolChunk* cur_chunk = pool->ptr_to_chunk;
    while (cur_chunk) {
        printf("Chunk start\n");
        for (int i = 0; i < DEFAULT_POOL_CHUNK_SIZE * pool->size_of_elem; ++i) {
            printf("%p : %x\n", cur_chunk->data, *(char*)(cur_chunk->data + i));
        }
        printf("Chunk end\n");
        cur_chunk = cur_chunk->next_chunk;
    }
    printf("Pool end: %d\n", pool->debug_typeid);
}

bool is_ptr_belongs_to_pool(Pool *pool, void *ptr_to_struct) {
    if (ptr_to_struct == NULL)
        return false;

    void* cur_ptr = NULL;
    PoolChunk* cur_chunk = pool->ptr_to_chunk;
    while (cur_chunk) {
        for (int i = 0; i < DEFAULT_POOL_CHUNK_SIZE; ++i) {
            cur_ptr = cur_chunk->data + pool->size_of_elem * i;
            if (GET_IS_OCCUPIED(cur_ptr) && GET_DATA_PTR(cur_ptr) == ptr_to_struct) {
                if (GET_START_BUF(cur_ptr)     != guard_value ||
                    GET_END_BUF(cur_ptr, pool) != guard_value)
                    ERROR_AND_ABORT("Corrupted guard buffers");
                return true;
            }
        }
        cur_chunk = cur_chunk->next_chunk;
    }
    return false;
}
