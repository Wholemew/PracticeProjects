#ifndef S21_SET
#define S21_SET

#include <limits>

#include "../s21_tree/s21_tree.h"

namespace s21 {

// Set class that stores only unique values
template <typename K>
class set : public s21Tree<K, K> {
 public:
  class Siterator;
  // member types
  using key_type = K;
  using value_type = K;
  using reference = value_type &;
  using const_reference = const value_type &;
  using iterator = Siterator;
  using const_iterator = const Siterator;
  using size_type = size_t;

 private:
  size_t si = 0;

 public:
  class Siterator : public s21Tree<K, K>::Titerator {
   public:
    Siterator() : s21Tree<K, K>::Titerator() {}
    Siterator(typename s21Tree<K, K>::node *t) : s21Tree<K, K>::Titerator(t) {}
    value_type &operator*() { return s21Tree<K, K>::Titerator::val->v.second; }
  };
  // Constructors and destructor
  set();
  set(std::initializer_list<value_type> const &items);
  set(const set &s);
  set(set &&s);
  ~set() { clear(); }
  set<K> operator=(set &&s) noexcept;

  iterator begin();
  iterator end();

  // Checks if set is empty
  bool empty();
  // Returns number of elements
  size_type size();
  // Returns maximum possible number of elements
  size_type max_size();

  // Clears the contents
  void clear();
  // Inserts a node, returns iterator to inserted node and bool whether the
  // insertion took place
  std::pair<iterator, bool> insert(const value_type &value);

  // Tries to insert several values listed in its arguements and returns vector
  // of insert result in the same order
  template <class... Args>
  std::vector<std::pair<iterator, bool>> insert_many(Args &&...args);

  // Erases an element at pos
  void erase(iterator pos);
  // Swaps the contents
  void swap(set &other);
  // Merges two sets
  void merge(set &other);

  // Finds an element with specidied key, returns iterator to it or to NULL if
  // not found
  iterator find(const key_type &key);
  // Checks if set contains element with specified key
  bool contains(const key_type &key);
};

}  // namespace s21

#include "s21_set.cpp"

#endif