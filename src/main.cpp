#include <iostream>
#include <map>
#include <memory>
#include "pool_allocator.h"
#include "my_list.h"

// Вспомогательная функция вычисления факториала
constexpr int factorial(int n) {
    if (n <= 1) return 1;
    int res = 1;
    for (int i = 2; i <= n; ++i) {
        res *= i;
    }
    return res;
}

int main() {
    // создание экземпляра std::map<int, int>
    std::map<int, int> m1;

    // заполнение 10 элементами, где ключ - число от 0 до 9, значение - факториал ключа
    for (int i = 0; i < 10; ++i) {
        m1[i] = factorial(i);
    }

    // создание экземпляра std::map<int, int> с новым аллокатором, ограниченным 10 элементами
    using Pair = std::pair<const int, int>;
    std::map<int, int, std::less<int>, PoolAllocator<Pair, 10>> m2;

    // заполнение 10 элементами, где ключ - число от 0 до 9, значение - факториал ключа
    for (int i = 0; i < 10; ++i) {
        m2[i] = factorial(i);
    }

    // вывод на экран всех значений (ключ и значение разделены пробелом), хранящихся в контейнере
    for (const auto& [key, value] : m2) {
        std::cout << key << " " << value << "\n";
    }

    // создание экземпляра своего контейнера для хранения значений типа int
    MyList<int> list1;

    // заполнение 10 элементами от 0 до 9
    for (int i = 0; i < 10; ++i) {
        list1.push_back(i);
    }

    // создание экземпляра своего контейнера для хранения int с новым аллокатором на 10 элементов
    MyList<int, PoolAllocator<int, 10>> list2;

    // заполнение 10 элементами от 0 до 9
    for (int i = 0; i < 10; ++i) {
        list2.push_back(i);
    }

    // вывод на экран всех значений, хранящихся в контейнере
    for (int v : list2) {
        std::cout << v << " ";
    }
    std::cout << "\n";

    return 0;
}