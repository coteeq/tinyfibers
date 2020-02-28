#pragma once

#include <tinyfiber/support/assert.hpp>

#include <cstddef>
#include <stdexcept>
#include <iterator>

namespace tinyfiber {

// Introduction to intrusive containers:
// https://www.boost.org/doc/libs/1_67_0/doc/html/intrusive/intrusive_vs_nontrusive.html

// Represents node of circular doubly-linked list

template <typename T>
struct IntrusiveListNode {
  using Node = IntrusiveListNode;

  Node* prev_ = nullptr;
  Node* next_ = nullptr;

  // Links this node before next in list

  void LinkBefore(Node* next) noexcept {
    TINY_VERIFY(!IsLinked(), "cannot link already linked node");

    prev_ = next->prev_;
    prev_->next_ = this;
    next_ = next;
    next->prev_ = this;
  }

  // Is this node linked in a circular list?

  bool IsLinked() const noexcept {
    return next_ != nullptr;
  }

  // Unlink this node from current list

  void Unlink() noexcept {
    if (next_) {
      next_->prev_ = prev_;
    }
    if (prev_) {
      prev_->next_ = next_;
    }
    next_ = prev_ = nullptr;
  }

  T* AsItem() {
    return static_cast<T*>(this);
  }
};

// Implemented as circular doubly-linked list with sentinel node

template <typename T>
class IntrusiveList {
  using Node = IntrusiveListNode<T>;

 public:
  void PushBack(Node* node) noexcept {
    node->LinkBefore(&head_);
  }

  void PushFront(Node* node) noexcept {
    node->LinkBefore(head_.next_);
  }

  T* PopFront() {
    if (IsEmpty()) {
      throw std::runtime_error("IntrusiveList is empty");
    }
    Node* front = head_.next_;
    front->Unlink();
    return front->AsItem();
  }

  void Append(IntrusiveList& that) noexcept {
    if (that.IsEmpty()) {
      return;
    }

    Node* that_front = that.head_.next_;
    Node* that_back = that.head_.prev_;

    that_back->next_ = &head_;
    that_front->prev_ = head_.prev_;

    Node* back = head_.prev_;

    head_.prev_ = that_back;
    back->next_ = that_front;

    that.head_.next_ = that.head_.prev_ = &that.head_;
  }

  bool IsEmpty() const noexcept {
    return head_.next_ == &head_;
  }

  IntrusiveList() {
    InitEmpty();
  }

  IntrusiveList(IntrusiveList&& that) {
    InitEmpty();
    Append(that);
  }

  IntrusiveList(const IntrusiveList& that) = delete;

  // linear complexity
  size_t Size() const {
    return std::distance(begin(), end());
  }

  void Clear() {
    Node* current = head_.next_;
    while (current != &head_) {
      Node* next = current->next_;
      current->Unlink();
      current = next;
    }
  }

  // Iteration

  // See "The Standard Librarian : Defining Iterators and Const
  // Iterators"

  template <class NodeT, class ItemT>
  class IteratorImpl {
    using Iterator = IteratorImpl<NodeT, ItemT>;

   public:
    using value_type = ItemT;                             // NOLINT
    using pointer = value_type*;                          // NOLINT
    using reference = value_type&;                        // NOLINT
    using difference_type = ptrdiff_t;                    // NOLINT
    using iterator_category = std::forward_iterator_tag;  // NOLINT

   public:
    IteratorImpl(NodeT* start, NodeT* end) : current_(start), end_(end) {
    }

    // prefix increment
    Iterator& operator++() {
      current_ = current_->next_;
      return *this;
    }

    bool operator==(const Iterator& that) const {
      return current_ == that.current_;
    }

    bool operator!=(const Iterator& that) const {
      return !(*this == that);
    }

    ItemT& operator*() const {
      return *Item();
    }

    ItemT* operator->() const {
      return Item();
    }

    ItemT* Item() const {
      return static_cast<ItemT*>(current_);
    }

   private:
    NodeT* current_;
    NodeT* end_;
  };

  using Iterator = IteratorImpl<Node, T>;
  using ConstIterator = IteratorImpl<const Node, const T>;

  Iterator begin() {  // NOLINT
    return Iterator(head_.next_, &head_);
  }

  Iterator end() {  // NOLINT
    return Iterator(&head_, &head_);
  }

  ConstIterator begin() const {  // NOLINT
    return ConstIterator(head_.next_, &head_);
  }

  ConstIterator end() const {  // NOLINT
    return ConstIterator(&head_, &head_);
  }

 private:
  void InitEmpty() {
    head_.next_ = head_.prev_ = &head_;
  }

 private:
  Node head_;  // sentinel node
};

}  // namespace tinyfiber
