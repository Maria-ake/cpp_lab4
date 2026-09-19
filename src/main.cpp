#include <iostream>
#include <map>
#include <memory>
#include "pool_allocator.h"
#include "my_list.h"

int main() {
    std::map<int, int> m1;
    for (int i = 0; i < 10; ++i)
        m1[i] = i * 10;

    std::cout << "=== std::map, std::allocator ===\n";
    for (const auto& [k, v] : m1)
        std::cout << k << " -> " << v << "\n";

    using Pair = std::pair<const int, int>;
    std::map<int, int, std::less<int>, PoolAllocator<Pair, 10>> m2;
    for (int i = 0; i < 10; ++i)
        m2[i] = i * 100;

    std::cout << "\n=== std::map, PoolAllocator ===\n";
    for (const auto& [k, v] : m2)
        std::cout << k << " -> " << v << "\n";

    MyList<int, std::allocator<int>> list1;
    for (int i = 0; i < 10; ++i)
        list1.push_back(i);

    std::cout << "\n=== MyList, std::allocator (size=" << list1.size() << ") ===\n";
    for (int v : list1) std::cout << v << " ";
    std::cout << "\n";

    MyList<int, PoolAllocator<int, 10>> list2;
    for (int i = 10; i < 20; ++i)
        list2.push_back(i);

    std::cout << "\n=== MyList, PoolAllocator (size=" << list2.size() << ") ===\n";
    for (int v : list2) std::cout << v << " ";
    std::cout << "\n";

    std::cout << "\nempty list1 = " << list1.empty()
              << ", empty list2 = " << list2.empty() << "\n";

    return 0;
}