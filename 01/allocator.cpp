#include <iostream>
class Allocator
{
    char* alocMemory = nullptr;
    size_t maxSize = 0;
    size_t offset = 0;
public:
    void makeAllocator(size_t maxSize) {
        if (alocMemory) {
            delete [] alocMemory;
            alocMemory = nullptr;
        }
        this->maxSize = maxSize;
        offset = 0;
        if (maxSize != 0)
            alocMemory = new char[maxSize];
    }

    char* alloc(size_t size) {
        if (size + offset > maxSize || alocMemory == nullptr || size == 0) 
            return nullptr;
        char* block = alocMemory + offset;
        offset += size;
        return block;
    }
    
    void reset() {
        offset = 0;
    }
    
    ~Allocator() { delete [] alocMemory; };
};
