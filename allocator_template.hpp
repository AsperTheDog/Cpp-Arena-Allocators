#pragma once
#include <cstdint>


template<class T, class C>
struct AllocatorTemplate
{
    typedef T value_type;

    AllocatorTemplate();

    template<class U>
    explicit constexpr AllocatorTemplate(const AllocatorTemplate <U, C>&) noexcept {}

    [[nodiscard]] T* allocate(size_t allocSize);
    void deallocate(T* allocation, size_t allocSize) noexcept;
};


template<class T, class U, class C>
bool operator==(const AllocatorTemplate <T, C>&, const AllocatorTemplate <U, C>&) { return true; }

template<class T, class U, class C>
bool operator!=(const AllocatorTemplate <T, C>&, const AllocatorTemplate <U, C>&) { return false; }

template<class T, class C>
AllocatorTemplate<T, C>::AllocatorTemplate()
{
    if (!C::isInitialized()) 
        C::init();
}

template<class T, class C>
T* AllocatorTemplate<T, C>::allocate(const size_t p_allocSize)
{
    return reinterpret_cast<T*>(C::allocate(p_allocSize * sizeof(T)));
}

template<class T, class C>
void AllocatorTemplate<T, C>::deallocate(T* p_allocation, const size_t p_allocSize) noexcept
{
    C::deallocate(reinterpret_cast<uint8_t*>(p_allocation), p_allocSize * sizeof(T));
}