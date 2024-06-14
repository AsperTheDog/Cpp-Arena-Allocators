#pragma once
#include <cstdlib>
#include <iostream>
#include <new>


class HeapSimpleArena
{
public:
    static void init(size_t size = s_defaultSize);
    static void deinit();
    static void reset(size_t size = s_size);

    static uint8_t* allocate(size_t allocSize);
    static void deallocate(uint8_t* allocation, size_t allocSize) noexcept;

    static bool isInitialized() noexcept { return s_arena != nullptr; }

private:
    constexpr static size_t s_defaultSize = 2ULL * 1024 * 1024;
    inline static uint8_t* s_arena = nullptr;
    inline static size_t s_size = s_defaultSize;

    inline static uint8_t* s_offset = nullptr;

};

class HeapArena
{
public:
    static void init(size_t size = s_defaultSize);
    static void deinit();
    static void reset(size_t size = s_size);

    constexpr static size_t s_defaultSize = 1024;
    
    static uint8_t* allocate(size_t size);
    static void deallocate(uint8_t* allocation, size_t size) noexcept;

    static bool isInitialized() noexcept { return s_arena != nullptr; }
    
private:
    inline static uint8_t* s_arena = nullptr;
    inline static size_t s_size = s_defaultSize;

    struct Chunk
    {
        Chunk* next;
        size_t size;
    };

    inline static Chunk* s_firstChunk;
};


// ***********************************
//          Implementation
// ***********************************

// Arena containers

inline void HeapSimpleArena::init(const size_t p_size)
{
    s_arena = static_cast<uint8_t*>(malloc(p_size));
    if (!s_arena)
        throw std::bad_alloc();

    std::cout << "Initializing arena with size: " << p_size << "\n";
    s_offset = s_arena;
    s_size = p_size;
}

inline void HeapSimpleArena::deinit()
{
    if (!s_arena) return;
    free(s_arena);
    s_arena = nullptr;
    s_offset = nullptr;
    s_size = 0;
}

inline void HeapSimpleArena::reset(const size_t p_size)
{
    if (!s_arena)
        init(p_size);
    else if (p_size == s_size)
        s_offset = s_arena;
    else if (p_size == 0)
        deinit();
    else
    {
        uint8_t* newPtr = static_cast<uint8_t*>(realloc(s_arena, p_size));
        if (!newPtr) throw std::bad_alloc();
        std::cout << "Reallocating arena to size: " << p_size << "\n";
        s_arena = newPtr;
    }

    s_size = p_size;
}

inline uint8_t* HeapSimpleArena::allocate(const size_t p_allocSize)
{
    if (s_offset + p_allocSize > s_arena + s_size)
        throw std::bad_alloc();
    
    uint8_t* allocation = s_offset;
    s_offset += p_allocSize;

    return allocation;
}

inline void HeapSimpleArena::deallocate(uint8_t* p_allocation, const size_t p_allocSize) noexcept
{
    if (p_allocation + p_allocSize == s_offset)
        s_offset = p_allocation;
}

