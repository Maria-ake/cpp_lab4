#pragma once
#include <memory>
#include <iterator>
#include <cstddef>

template <typename T, typename Allocator = std::allocator<T>>
class MyList {
    struct Node {
        T value;
        Node* next;
        explicit Node(const T& v) : value(v), next(nullptr) {}
    };

    using NodeAlloc = typename std::allocator_traits<Allocator>::template rebind_alloc<Node>;

    Node* head_ = nullptr;
    Node* tail_ = nullptr;
    std::size_t size_ = 0;
    NodeAlloc alloc_;

public:
    MyList() = default;

    ~MyList() { clear(); }

    void push_back(const T& value) {
        Node* node = alloc_.allocate(1);
        std::allocator_traits<NodeAlloc>::construct(alloc_, node, value);
        if (!head_) head_ = tail_ = node;
        else { tail_->next = node; tail_ = node; }
        ++size_;
    }

    void clear() {
        Node* cur = head_;
        while (cur) {
            Node* next = cur->next;
            std::allocator_traits<NodeAlloc>::destroy(alloc_, cur);
            alloc_.deallocate(cur, 1);
            cur = next;
        }
        head_ = tail_ = nullptr;
        size_ = 0;
    }

    bool empty() const { return size_ == 0; }
    std::size_t size() const { return size_; }

    class iterator {
        Node* node_;
    public:
        using iterator_category = std::forward_iterator_tag;
        using value_type        = T;
        using difference_type   = std::ptrdiff_t;
        using pointer           = T*;
        using reference         = T&;

        explicit iterator(Node* n = nullptr) : node_(n) {}
        T& operator*()  const { return node_->value; }
        iterator& operator++() { node_ = node_->next; return *this; }
        bool operator==(const iterator& o) const { return node_ == o.node_; }
        bool operator!=(const iterator& o) const { return node_ != o.node_; }
    };

    iterator begin() { return iterator(head_); }
    iterator end()   { return iterator(nullptr); }
};