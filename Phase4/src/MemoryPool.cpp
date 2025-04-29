#include "../include/MemoryPool.hpp"
#include <cassert>

MemoryPool::MemoryPool(size_t blockSize, size_t blockCount)
 : m_blockSize(blockSize)
 , m_buffer(blockSize * blockCount)
{
    m_freeList.reserve(blockCount);
    for(size_t i=0;i<blockCount;i++){
        m_freeList.push_back(m_buffer.data() + i*blockSize);
    }
}

MemoryPool::~MemoryPool() {
    // buffer freed automatically
}

void* MemoryPool::allocate() {
    assert(!m_freeList.empty());
    void* ptr = m_freeList.back();
    m_freeList.pop_back();
    return ptr;
}

void MemoryPool::deallocate(void* ptr) {
    m_freeList.push_back(ptr);
}
