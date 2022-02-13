#pragma once

#include <memory>
#include <cassert>

template<typename T, std::size_t AllocChunkSize>
struct MyAllocator {
    using value_type = T;

//    using reference = T&;
//    using const_reference = const T&;
//    using pointer = T*;
//    using const_pinter = const T*;
//
//    template<typename U, typename... Args>
//    void construct(U* p, Args&& ...args) {
//        new(p) U(std::forward<Args>(args)...);
//    };
//
//    template<typename U>
//    void destroy(U* p) {
//        p->~U();
//    }

    template<typename U>
    struct rebind {
        using other = MyAllocator<U, AllocChunkSize>;
    };

    MyAllocator() = default;

    ~MyAllocator() {
        for (auto curNode = m_listNodeHead; curNode != nullptr;) {
            std::free(curNode->buffer);
            auto next = curNode->next;
            std::free(curNode);
            curNode = next;
        }
    }

    T* allocate(std::size_t n) {
        if (n > AllocChunkSize) {
            throw std::bad_alloc();
        }

        if (!m_curBuffer || n > AllocChunkSize - m_usedBytesInCurChunk) {
            allocateNewChunk();
        }

        auto p = m_curBuffer + m_usedBytesInCurChunk;
        m_usedBytesInCurChunk += n;
        return p;
    }

    // Don't support.
    //
    void deallocate(T* /*p*/, std::size_t /*n*/) {
    }

private:
    void allocateNewChunk() {
        auto* newNode = reinterpret_cast<LinkedListNode*>(malloc(sizeof(LinkedListNode)));
        if (!newNode) {
            throw std::bad_alloc();
        }
//        newNode->buffer = new T[AllocChunkSize]; // Compile-Error: no tree_node constructor
        newNode->buffer = reinterpret_cast<T*>(malloc(sizeof(T) * AllocChunkSize));
        if (!newNode->buffer) {
            throw std::bad_alloc();
        }
        newNode->next = nullptr;

        if (!m_listNodeHead) {
            m_listNodeHead = newNode;
        } else {
            m_listNodeLast->next = newNode;
        }

        m_listNodeLast = newNode;
        m_usedBytesInCurChunk = 0;
        m_curBuffer = newNode->buffer;
    }


    struct LinkedListNode {
        LinkedListNode() : next(nullptr), buffer(nullptr) {}

        LinkedListNode* next;
        T* buffer;
    };

    LinkedListNode* m_listNodeHead{nullptr};
    LinkedListNode* m_listNodeLast{nullptr};
    size_t m_usedBytesInCurChunk{0};
    T* m_curBuffer{nullptr};
};
