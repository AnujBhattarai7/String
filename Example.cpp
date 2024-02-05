#include <iostream>
#include <string>
#include "String.h"

#define _PRINT_(x)              \
    {                           \
        std::cout << x << "\n"; \
    }

struct MemoryManager
{
    int _Allocation, _Free;
};

static MemoryManager _MemoryManager;

void *operator new(size_t size)
{
    std::cout << "Allocation"
              << "\n";
    _MemoryManager._Allocation += size;
    return malloc(size);
}

void operator delete(void *p, size_t size)
{
    std::cout << "Delete"
              << "\n";
    _MemoryManager._Free += size;

    free(p);
}

static void PrintMemoryMetrics()
{
    _PRINT_("[MEMORY_METRICS]: Allocated: " << _MemoryManager._Allocation << " bytes..")
    _PRINT_("[MEMORY_METRICS]: Freed: " << _MemoryManager._Free << " bytes..")
}

int main(int argc, char const *argv[])
{
    {
        String _S = "Hello";
        _S.Append(" asdasd");
        
        _PRINT_((_S.SubStr(2, 5)).C_Str())
    }

    std::cin.get();
    PrintMemoryMetrics();

    return 0;
}
