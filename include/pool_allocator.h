#pragma once
#include <cstddef>
#include <memory>
#include <vector>
#include <new>

template <typename T, std::size_t BlockSize = 10>
class PoolAllocator {
    struct Pool {
        std::size_t slotSize;
        std::vector<char*> chunks;
        std::vector<void*> freeSlots;

        explicit Pool(std::size_t sz) : slotSize(sz) {}

        ~Pool() {
            for (char* c : chunks)
                ::operator delete(c);
        }

        void* allocate() {
            if (freeSlots.empty()) {
                char* chunk = static_cast<char*>(::operator new(BlockSize * slotSize));
                chunks.push_back(chunk);
                for (std::size_t i = 0; i < BlockSize; ++i)
                    freeSlots.push_back(chunk + i * slotSize);
            }
            void* p = freeSlots.back();
            freeSlots.pop_back();
            return p;
        }

        void deallocate(void* p) {
            freeSlots.push_back(p);
        }
    };

    std::shared_ptr<Pool> pool_;

public:
    using value_type = T;

    PoolAllocator() : pool_(std::make_shared<Pool>(sizeof(T))) {}

    template <typename U>
    PoolAllocator(const PoolAllocator<U, BlockSize>&)
        : pool_(std::make_shared<Pool>(sizeof(T))) {}

    template <typename U>
    struct rebind { using other = PoolAllocator<U, BlockSize>; };

    T* allocate(std::size_t n) {
        if (n == 1) return static_cast<T*>(pool_->allocate());
        return static_cast<T*>(::operator new(n * sizeof(T)));
    }

    void deallocate(T* p, std::size_t n) {
        if (n == 1) pool_->deallocate(p);
        else        ::operator delete(p);
    }

    template <typename U>
    bool operator==(const PoolAllocator<U, BlockSize>&) const noexcept { return false; }
    template <typename U>
    bool operator!=(const PoolAllocator<U, BlockSize>&) const noexcept { return true; }
};