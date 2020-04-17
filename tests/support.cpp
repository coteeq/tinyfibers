#include "gtest/gtest.h"

#include <tinyfiber/support/intrusive_list.hpp>
#include <tinyfiber/support/string_builder.hpp>

using namespace tiny::fiber;

TEST(StringBuilder, HelloWorld) {
  std::string greeting = StringBuilder() << "Hello" << ", " << "World" << "!";
  ASSERT_EQ(greeting, "Hello, World!");
}

TEST(StringBuilder, NotOnlyStrings) {
  char chr = 'X';
  int answer = 42;
  bool locked = true;

  std::string result = StringBuilder() << "char = " << chr
      << ", answer = " << answer << ", locked = " << locked;

  // TODO: booleans
  ASSERT_EQ(result, "char = X, answer = 42, locked = 1");
}

struct Item
    : public IntrusiveListNode<Item> {
  std::string data_;

  Item(std::string data) : data_(std::move(data)) {
  }
};

TEST(IntrusiveList, PushThenPop) {
  Item first("hello");
  Item second("world");
  Item third("!");

  IntrusiveList<Item> items;

  ASSERT_TRUE(items.IsEmpty());

  items.PushBack(&first);
  items.PushBack(&second);
  items.PushBack(&third);

  ASSERT_FALSE(items.IsEmpty());

  Item *pop_first = items.PopFront();
  ASSERT_EQ(pop_first->data_, "hello");

  Item *pop_second = items.PopFront();
  ASSERT_EQ(pop_second->data_, "world");

  Item *pop_third = items.PopFront();
  ASSERT_EQ(pop_third->data_, "!");

  ASSERT_TRUE(items.IsEmpty());

  ASSERT_THROW(items.PopFront(), std::runtime_error);
}

TEST(IntrusiveList, Iterator) {
  Item first("hello");
  Item second("world");
  Item third("!");

  IntrusiveList<Item> items;

  ASSERT_EQ(items.begin(), items.end());

  items.PushBack(&first);
  items.PushBack(&second);
  items.PushBack(&third);

  auto iterator = items.begin();
  auto end = items.end();

  ASSERT_NE(iterator, end);
  ASSERT_EQ((*iterator).data_, "hello");

  ++iterator;
  ASSERT_NE(iterator, end);

  ++iterator;
  ++iterator;
  ASSERT_EQ(iterator, end);
}

TEST(IntrusiveList, RangeBasedForLoop) {
  Item first("hello");
  Item second("world");
  Item third("!");

  IntrusiveList<Item> items;
  items.PushBack(&first);
  items.PushBack(&second);
  items.PushBack(&third);

  StringBuilder builder;
  for (auto&& item : items) {
    builder << item.data_ << " ";
  }
  std::string message = builder;
  ASSERT_EQ(message, "hello world ! ");
}

TEST(IntrusiveList, AppendList) {
  Item first("hello");
  Item second("world");
  Item third("!");

  IntrusiveList<Item> items;

  ASSERT_EQ(items.begin(), items.end());

  items.PushBack(&first);
  items.PushBack(&second);
  items.PushBack(&third);

  Item foo("foo");
  Item bar("bar");

  IntrusiveList<Item> another_items;
  another_items.PushBack(&foo);
  another_items.PushBack(&bar);

  items.Append(another_items);

  ASSERT_TRUE(another_items.IsEmpty());

  ASSERT_EQ(items.PopFront()->data_, "hello");
  ASSERT_EQ(items.PopFront()->data_, "world");
  ASSERT_EQ(items.PopFront()->data_, "!");
  ASSERT_EQ(items.PopFront()->data_, "foo");
  ASSERT_EQ(items.PopFront()->data_, "bar");
}

TEST(IntrusiveList, MoveCtor) {
  Item first("hello");
  Item second("world");
  Item third("!");

  IntrusiveList<Item> items;

  ASSERT_EQ(items.begin(), items.end());

  items.PushBack(&first);
  items.PushBack(&second);

  IntrusiveList<Item> another_items = std::move(items);

  ASSERT_TRUE(items.IsEmpty());
  items.PushBack(&third);
  ASSERT_FALSE(items.IsEmpty());
}
