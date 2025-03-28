namespace s21 {

template <typename K>
set<K>::set() : s21Tree<K, K>(), si(0) {}

template <typename K>
set<K>::set(std::initializer_list<value_type> const &items) {
  for (auto it = items.begin(); it != items.end(); it++) insert(*it);
}

template <typename K>
set<K>::set(const set &s) : s21Tree<K, K>(s), si(s.si) {}

template <typename K>
set<K>::set(set &&s) : s21Tree<K, K>(std::move(s)), si(s.si) {}

template <typename K>
set<K> set<K>::operator=(set &&s) noexcept {
  s21Tree<K, K>::FREEtree();
  si = s.si;
  s21Tree<K, K>::root = s.root;
  s.root = NULL;
  return *this;
}

template <typename K>
bool set<K>::empty() {
  if (si == 0)
    return true;
  else
    return false;
}

template <typename K>
typename set<K>::size_type set<K>::size() {
  return si;
}

template <typename K>
typename set<K>::size_type set<K>::max_size() {
  return std::numeric_limits<size_type>::max();
}

template <typename K>
typename set<K>::iterator set<K>::begin() {
  if (s21Tree<K, K>::root == NULL)
    throw std::runtime_error("Iterating empty map");

  typename s21Tree<K, K>::node *tmp = s21Tree<K, K>::root;
  while (tmp->left != NULL) tmp = tmp->left;

  iterator result(tmp);
  // iterator result(s21Tree<K, K>::root);
  // while (result.val->left != NULL) result--;
  return result;
}

template <typename K>
typename set<K>::iterator set<K>::end() {
  if (s21Tree<K, K>::root == NULL)
    throw std::runtime_error("Iterating empty map");

  typename s21Tree<K, K>::node *tmp = s21Tree<K, K>::root;
  while (tmp->right != NULL) tmp = tmp->right;

  iterator result(tmp);

  // iterator result(s21Tree<K, K>::root);
  // while (result.val->right != NULL) result++;
  result.end = result.val;
  result.val = NULL;
  return result;
}

template <typename K>
void set<K>::clear() {
  if (si > 0) {
    si = 0;
    s21Tree<K, K>::FREEtree();
  }
}

template <typename K>
std::pair<typename set<K>::iterator, bool> set<K>::insert(
    const value_type &value) {
  std::pair<typename set<K>::value_type, typename set<K>::value_type> v(value,
                                                                        value);
  std::pair<typename s21Tree<K, K>::Titerator, bool> res =
      s21Tree<K, K>::put(v);
  std::pair<iterator, bool> ret =
      std::make_pair(iterator(res.first.val), res.second);
  if (std::get<1>(res) == true) si++;
  return ret;
}

template <typename K>
template <class... Args>
std::vector<typename std::pair<typename set<K>::iterator, bool>>
set<K>::insert_many(Args &&...args) {
  std::vector<typename std::pair<typename set<K>::iterator, bool>> res;
  for (const auto &arg : {args...}) {
    res.push_back(insert(arg));
  }
  return res;
}

template <typename K>
void set<K>::erase(iterator pos) {
  bool erased;
  try {
    erased = s21Tree<K, K>::erase(pos);
  } catch (const std::runtime_error &e) {
    throw e;
  }
  if (erased) si--;
}

template <typename K>
void set<K>::swap(set &other) {
  set<K> tmp(other);
  other = std::move(*this);
  *this = std::move(tmp);
}

template <typename K>
void set<K>::merge(set &other) {
  std::pair<iterator, bool> out;
  while (other.si > 0) {
    iterator i(other.s21Tree<K, K>::root);
    out = this->insert(i.val->v.second);
    if (std::get<1>(out)) si++;
    other.erase(i);
  }
}

template <typename K>
typename set<K>::iterator set<K>::find(const key_type &key) {
  return iterator((s21Tree<K, K>::root->searchIT(key)).val);
}

template <typename K>
bool set<K>::contains(const key_type &k) {
  try {
    s21Tree<K, K>::root->Nsearch(k);
  } catch (const std::runtime_error &e) {
    return false;
  }
  return true;
}

}  // namespace s21