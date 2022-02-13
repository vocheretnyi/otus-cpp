#pragma once

#include <utility>
#include <memory>

template<typename T, typename Allocator = std::allocator<T>>
class MyList {
 private:
    struct Node {
        explicit Node(T* data_) : data(data_), next(nullptr) {}

        T* data;
        Node* next;
    };

    struct Iterator {
     public:
        Iterator(Node* ptr) : m_ptr(ptr) {}

        Iterator operator++() {
            m_ptr = m_ptr->next;
            return *this;
        }

        bool operator!=(const Iterator& other) const { return m_ptr != other.m_ptr; }

        bool operator==(const Iterator& other) const { return m_ptr == other.m_ptr; }

        const T& operator*() const { return *m_ptr->data; }

     private:
        Node* m_ptr;
    };

 public:
    MyList() : m_head(nullptr), m_tail(nullptr), m_size(0) {}

    void push(T val) {
        m_size++;
        T* data = m_alloc.allocate(1);
        *data = std::move(val);
        Node* node = new Node(data);

        if (!m_head) {
            m_head = m_tail = node;
            return;
        }

        m_tail->next = node;
        m_tail = m_tail->next;
    }

    const T& front() const {
        return *m_head->data;
    }

    const T& back() const {
        return *m_tail->data;
    }

    Iterator begin() {
        return Iterator(m_head);
    }

    Iterator end() {
        return m_tail ? Iterator(m_tail->next) : nullptr;
    }

    size_t size() const {
        return m_size;
    }

    bool empty() const {
        return m_size == 0;
    }

 private:
    Node* m_head;
    Node* m_tail;
    size_t m_size;
    Allocator m_alloc;
};
