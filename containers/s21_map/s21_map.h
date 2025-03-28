#ifndef S21_MAP
#define S21_MAP

#include <iostream>
#include <limits>

#include "../s21_tree/s21_tree.h"

namespace s21 {

// Map class that stores values associated with names in binary tree structure
template <typename K, typename T>
class map : public s21Tree<K, T> {
 public:
  class Miterator;
  // member types
  using key_type = K;
  using mapped_type = T;
  using value_type = std::pair<const key_type, mapped_type>;
  using reference = value_type &;
  using const_reference = const value_type &;
  using iterator = Miterator;
  using const_iterator = const Miterator;
  using size_type = size_t;

 private:
  size_type s = 0;

 public:
  class Miterator : public s21Tree<K, T>::Titerator {
   public:
    Miterator() : s21Tree<K, T>::Titerator() {}
    Miterator(typename s21Tree<K, T>::node *t) : s21Tree<K, T>::Titerator(t) {}
    value_type *operator->() { return &s21Tree<K, T>::Titerator::val->v; }
    value_type operator*() { return s21Tree<K, T>::Titerator::val->v; }
  };
  // Constructors and destructor
  map() noexcept;
  map(std::initializer_list<value_type> const &items) noexcept;
  map(const map &m) noexcept;
  map(map &&m) noexcept;
  ~map() { clear(); }
  map<K, T> &operator=(map &&m) noexcept;
  // Element access
  // With bounds check
  mapped_type &at(const key_type &k);
  // With element insert
  mapped_type &operator[](const key_type &k) noexcept;
  // Iterator handling
  iterator begin();
  iterator end();
  // Clear the contents
  void clear() noexcept;
  // Inserts a node, returns a pair of iterator to this element in the container
  // and whether it was inserted or already existed
  std::pair<iterator, bool> insert(const value_type &value) noexcept;
  std::pair<iterator, bool> insert(const key_type k,
                                   const mapped_type obj) noexcept;
  // Tries to insert a node, if it exists returns bool = false and reassignes
  // the value in existing node to obj
  std::pair<iterator, bool> insert_or_assign(const key_type k,
                                             const mapped_type obj) noexcept;

  // Tries to insert several values listed in its arguements and returns vector
  // of insert result in the same order
  template <class... Args>
  std::vector<std::pair<iterator, bool>> insert_many(Args &&...args);
  // Erases the node and rebuilds the tree accordingly
  void erase(iterator pos);
  // Modifies value with a key pointed to by iterator
  void modify(iterator pos, mapped_type value) noexcept;
  // Swaps the contents of two maps
  void swap(map &other) noexcept;
  // Moves nodes from other map into the map without rewriting existing nodes
  void merge(map &other) noexcept;
  // Checks if map is empty
  bool empty() noexcept;
  // Returns number of elements
  size_type size() noexcept;
  // Returns maximum possible number of elements
  size_type max_size() noexcept;
  // Checks if map contains the element with specified key
  bool contains(const key_type &k) noexcept;
};

}  // namespace s21

#include "s21_map.cpp"

#endif