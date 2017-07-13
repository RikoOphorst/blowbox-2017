#include "eastl.h"

void* operator new[](size_t size, const char* /*name*/, int /*flags*/,
    unsigned /*debugFlags*/, const char* /*file*/, int /*line*/) THROW_SPEC_1(std::bad_alloc)
{
    return malloc(size);
}

void* operator new[](size_t size, size_t alignment, size_t alignmentOffset, const char* /*name*/,
    int flags, unsigned /*debugFlags*/, const char* /*file*/, int /*line*/) THROW_SPEC_1(std::bad_alloc)
{
    // Substitute your aligned malloc. 
    return malloc(size);
}

void* operator new(size_t size) THROW_SPEC_1(std::bad_alloc)
{
    return malloc(size);
}

void* operator new[](size_t size) THROW_SPEC_1(std::bad_alloc)
{
    return malloc(size);
}

void operator delete(void* p) THROW_SPEC_0
{
    if (p) // The standard specifies that 'delete NULL' is a valid operation.
        free(p);
}

void operator delete[](void* p) THROW_SPEC_0
{
    if (p)
    free(p);
}