#pragma once
#include <cstddef>
#include <memory>
#include <vector>
#include <new>
#include <iostream>

template <typename T, std::size_t BlockSize = 10>
class PoolAllocator {
    struct Pool {
        std::size_t slotSize;
        std::vector<char*> chunks;
        std::vector<void*> freeSlots;
        std::size_t allocated = 0;
        std::size_t deallocated = 0;

        explicit Pool(std::size_t sz) : slotSize(sz) {
            std::cout << "Пул создан. Размер ячейки: " << sz
                      << " байт, блок: " << BlockSize << "\n";
        }

        ~Pool() {
            std::cout << "Пул уничтожается. Чанков: " << chunks.size()
                      << ", allocate: " << allocated
                      << ", deallocate: " << deallocated
                      << ", утечка: " << (allocated - deallocated) << "\n";
            for (char* c : chunks) {
                std::cout << "  освобождаю чанк " << (void*)c << "\n";
                ::operator delete(c);
            }
        }

        void* allocate() {
            if (freeSlots.empty()) {
                char* chunk = static_cast<char*>(
                    ::operator new(BlockSize * slotSize));
                chunks.push_back(chunk);
                for (std::size_t i = 0; i < BlockSize; ++i)
                    freeSlots.push_back(chunk + i * slotSize);
                std::cout << "  Взят новый чанк #" << chunks.size()
                          << " (" << BlockSize << " ячеек по "
                          << slotSize << " байт)\n";
            }
            void* p = freeSlots.back();
            freeSlots.pop_back();
            ++allocated;
            std::size_t total = chunks.size() * BlockSize;
            std::cout << "    allocate  -> " << p
                      << " (занято " << (total - freeSlots.size())
                      << "/" << total << ")\n";
            return p;
        }

        void deallocate(void* p) {
            freeSlots.push_back(p);
            ++deallocated;
            std::size_t total = chunks.size() * BlockSize;
            std::cout << "    deallocate <- " << p
                      << " (занято " << (total - freeSlots.size())
                      << "/" << total << ")\n";
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