#ifndef S21_TREE
#define S21_TREE

#include <cstddef>
#include <initializer_list>
#include <iostream>
#include <stdexcept>
#include <utility>

namespace s21 {

// Auxiliary class for value storage in map and set. Cannot be used outside
// parent classes because it lacks destructor
template <typename K, typename T>
class s21Tree {
 public:
  class node {
   public:
    std::pair<const K, T> v;
    node* left;
    node* right;
    node* parent;

    // Constructors and destructor
    node() : left(NULL), right(NULL), parent(NULL) {}
    node(const node& n);
    node(node&& n);
    node(K first, T second)
        : v(std::make_pair(first, second)),
          left(NULL),
          right(NULL),
          parent(NULL) {}
    // Destructor destroys whole branch!
    void FREEnode();

    // Assigns the whole branch and makes recipient parentless (root)!
    node& operator=(node&& src);

    std::pair<typename s21Tree<K, T>::Titerator, bool> Nput(
        std::pair<const K, T>);
    T& Nsearch(const K& k);
    typename s21Tree<K, T>::Titerator searchIT(const K& k);

    friend class s21Tree<K, T>;
  };

 public:
  node* root;

  // Constructors and destructor
  s21Tree();
  s21Tree(K k, T v);
  s21Tree(const s21Tree& tree);
  s21Tree(s21Tree&& tree);
  s21Tree(std::initializer_list<std::pair<const K, T>> const& items);
  void FREEtree();
  s21Tree& operator=(s21Tree&& src);

  class Titerator {
   public:
    s21Tree<K, T>::node* val;
    s21Tree<K, T>::node* end = NULL;
    Titerator();
    Titerator(node* target);
    Titerator& operator++();
    Titerator& operator--();
    Titerator& operator++(int) { return operator++(); }
    Titerator& operator--(int) { return operator--(); }
    bool operator==(Titerator other) {
      return ((val == other.val) && (end == other.end));
    }
    bool operator!=(Titerator other) {
      return !((val == other.val) && (end == other.end));
    }
  };

  // Puts value v into the corresponding position in the tree without
  // overwrighting. Returns pointer to inserted position if put and true, or
  // pointer to matching key and false if not put
  std::pair<s21Tree<K, T>::Titerator, bool> put(std::pair<const K, T>);

  // Puts value v into the corresponding position in tree. If it is already in
  // the tree, creates new node as if inserted was bigger than existing
  typename s21Tree<K, T>::Titerator force_put(std::pair<const K, T>);

  // Searches for element with specified key
  T& search(const K& k);
  // Searches and returns iterator to found node or NULL
  Titerator searchIT(const K& k);
  // Erases the node and rebuilds the tree accordingly
  bool erase(Titerator pos);

  // Attaches hanging node to the left side of subtree that pushed it
  void RebindLeft(Titerator pos);
};

}  // namespace s21

#include "s21_tree.cpp"

#endif