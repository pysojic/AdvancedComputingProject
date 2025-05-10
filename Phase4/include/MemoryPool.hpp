#pragma once
#include <cstddef>
#include <vector>
#include "config.h"

class MemoryPool {
public:
    MemoryPool(size_t blockSize, size_t blockCount);
    ~MemoryPool();
    void*  allocate();
    void   deallocate(void* ptr);

private:
    size_t                  m_blockSize;
    std::vector<void*>      m_freeList;
    std::vector<char>       m_buffer;
};
