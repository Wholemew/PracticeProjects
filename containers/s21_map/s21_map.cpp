namespace s21 {

template <typename K, typename T>
map<K, T>::map() noexcept : s21Tree<K, T>(), s(0) {}

template <typename K, typename T>
map<K, T>::map(std::initializer_list<value_type> const &items) noexcept {
  for (auto it = items.begin(); it != items.end(); it++) insert(*it);
}

template <typename K, typename T>
map<K, T>::map(const map &m) noexcept : s21Tree<K, T>(m), s(m.s) {}

template <typename K, typename T>
map<K, T>::map(map &&m) noexcept : s21Tree<K, T>(std::move(m)), s(m.s) {
  m.~map();
}

template <typename K, typename T>
map<K, T> &map<K, T>::operator=(map &&m) noexcept {
  s21Tree<K, T>::FREEtree();
  s = m.s;
  s21Tree<K, T>::root = m.root;
  m.root = NULL;
  return *this;
}

template <typename K, typename T>
typename map<K, T>::mapped_type &map<K, T>::at(const key_type &k) {
  try {
    return s21Tree<K, T>::search(k);
  } catch (const char *msg) {
    throw "Not found";
  }
}

template <typename K, typename T>
typename map<K, T>::mapped_type &map<K, T>::operator[](
    const key_type &k) noexcept {
  std::pair<const key_type, mapped_type> ins(k, (mapped_type)0);
  std::pair<typename s21Tree<K, T>::Titerator, bool> res =
      s21Tree<K, T>::put(ins);
  if (res.second) s++;
  return std::get<0>(res).val->v.second;
}

template <typename K, typename T>
typename map<K, T>::iterator map<K, T>::begin() {
  if (s21Tree<K, T>::root == NULL)
    throw std::runtime_error("Iterating empty map");

  typename s21Tree<K, T>::node *tmp = s21Tree<K, T>::root;
  while (tmp->left != NULL) tmp = tmp->left;

  iterator result(tmp);
  return result;
}

template <typename K, typename T>
typename map<K, T>::iterator map<K, T>::end() {
  if (s21Tree<K, T>::root == NULL)
    throw std::runtime_error("Iterating empty map");

  typename s21Tree<K, T>::node *tmp = s21Tree<K, T>::root;
  while (tmp->right != NULL) tmp = tmp->right;

  iterator result(tmp);

  result.end = result.val;
  result.val = NULL;
  return result;
}

template <typename K, typename T>
void map<K, T>::clear() noexcept {
  if (s > 0) {
    s = 0;
    s21Tree<K, T>::FREEtree();
  }
}

template <typename K, typename T>
typename std::pair<typename map<K, T>::iterator, bool> map<K, T>::insert(
    const value_type &value) noexcept {
  std::pair<typename s21Tree<K, T>::Titerator, bool> res =
      s21Tree<K, T>::put(value);
  std::pair<iterator, bool> ret =
      std::make_pair(iterator(res.first.val), res.second);
  if (std::get<1>(ret) == true) s++;
  return ret;
}

template <typename K, typename T>
typename std::pair<typename map<K, T>::iterator, bool> map<K, T>::insert(
    const key_type k, const mapped_type obj) noexcept {
  std::pair<key_type, mapped_type> value(k, obj);
  std::pair<typename s21Tree<K, T>::Titerator, bool> res =
      s21Tree<K, T>::put(value);
  std::pair<iterator, bool> ret =
      std::make_pair(iterator(res.first.val), res.second);
  if (std::get<1>(res) == true) s++;
  return ret;
}

template <typename K, typename T>
typename std::pair<typename map<K, T>::iterator, bool>
map<K, T>::insert_or_assign(const key_type k, const mapped_type obj) noexcept {
  std::pair<key_type, mapped_type> value(k, obj);
  std::pair<typename s21Tree<K, T>::Titerator, bool> res =
      s21Tree<K, T>::put(value);
  if (std::get<1>(res) == false)
    res.first.val->v.second = obj;
  else
    s++;
  std::pair<iterator, bool> ret =
      std::make_pair(iterator(res.first.val), res.second);
  return ret;
}

template <typename K, typename T>
template <class... Args>
std::vector<typename std::pair<typename map<K, T>::iterator, bool>>
map<K, T>::insert_many(Args &&...args) {
  std::vector<typename std::pair<typename map<K, T>::iterator, bool>> res;
  for (const auto &arg : {args...}) {
    res.push_back(insert(arg));
  }
  return res;
}

template <typename K, typename T>
void map<K, T>::erase(iterator pos) {
  bool erased;
  try {
    erased = s21Tree<K, T>::erase(pos);
  } catch (const std::runtime_error &e) {
    throw e;
  }
  if (erased) s--;
}

template <typename K, typename T>
void map<K, T>::modify(iterator pos, mapped_type value) noexcept {
  pos.val->Mutator(value);
}

template <typename K, typename T>
void map<K, T>::swap(map &other) noexcept {
  map<K, T> tmp(other);
  other = std::move(*this);
  *this = std::move(tmp);
}

template <typename K, typename T>
void map<K, T>::merge(map &other) noexcept {
  std::pair<iterator, bool> out;
  while (other.s > 0) {
    iterator i(other.s21Tree<K, T>::root);
    out = this->insert(i.val->v.first, i.val->v.second);
    if (std::get<1>(out)) s++;
    other.erase(i);
  }
}

template <typename K, typename T>
bool map<K, T>::empty() noexcept {
  if (s == 0)
    return true;
  else
    return false;
}

template <typename K, typename T>
typename map<K, T>::size_type map<K, T>::size() noexcept {
  return s;
}

template <typename K, typename T>
typename map<K, T>::size_type map<K, T>::max_size() noexcept {
  return std::numeric_limits<size_type>::max();
}

template <typename K, typename T>
bool map<K, T>::contains(const key_type &k) noexcept {
  try {
    s21Tree<K, T>::search(k);
  } catch (const std::runtime_error &e) {
    return false;
  }
  return true;
}

}  // namespace s21