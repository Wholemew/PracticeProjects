namespace s21 {

template <typename K, typename T>
s21Tree<K, T>::node::node(s21Tree<K, T>::node&& n) : v(n.v), parent(NULL) {
  if (n.left != NULL) {
    left = new node(*n.left);
    left->parent = this;
    delete n.left;
    n.left = NULL;
  } else
    left = NULL;
  if (n.right != NULL) {
    right = new node(*n.right);
    right->parent = this;
    delete n.right;
    n.right = NULL;
  } else
    right = NULL;
}

template <typename K, typename T>
s21Tree<K, T>::node::node(const s21Tree<K, T>::node& n) : v(n.v), parent(NULL) {
  if (n.left != NULL) {
    left = new node(*n.left);
    left->parent = this;
  } else
    left = NULL;
  if (n.right != NULL) {
    right = new node(*n.right);
    right->parent = this;
  } else
    right = NULL;
}

template <typename K, typename T>
typename s21Tree<K, T>::node& s21Tree<K, T>::node::operator=(
    s21Tree<K, T>::node&& src) {
  this->FREEnode();
  parent = NULL;
  v = src.v;
  if (src.left != NULL) {
    left = new node;
    node tmp(*src.left);
    *left = tmp;
    left->parent = this;
  } else
    left = NULL;
  if (src.right != NULL) {
    right = new node;
    node tmp(*src.right);
    *right = tmp;
    right->parent = this;
  } else
    right = NULL;
  return *this;
}

template <typename K, typename T>
void s21Tree<K, T>::node::FREEnode() {
  if (left != NULL) {
    left->FREEnode();
    delete left;
    left = NULL;
  }
  if (right != NULL) {
    right->FREEnode();
    delete right;
    right = NULL;
  }
}

template <typename K, typename T>
std::pair<typename s21Tree<K, T>::Titerator, bool> s21Tree<K, T>::node::Nput(
    std::pair<const K, T> p) {
  Titerator it(this);
  std::pair<typename s21Tree<K, T>::Titerator, bool> result(it, true);
  if (v.first > std::get<0>(p))
    if (left != NULL)
      result = left->Nput(p);
    else {
      left = new node(p.first, p.second);
      left->parent = this;
      result.first.val = left;
    }
  else if (v.first < std::get<0>(p))
    if (right != NULL)
      result = right->Nput(p);
    else {
      right = new node(p.first, p.second);
      right->parent = this;
      result.first.val = right;
    }
  else
    result.second = false;
  return result;
}

template <typename K, typename T>
T& s21Tree<K, T>::node::Nsearch(const K& k) {
  if (v.first > k)
    if (left != NULL)
      return left->Nsearch(k);
    else
      throw std::runtime_error("Not found");
  else if (v.first < k)
    if (right != NULL)
      return right->Nsearch(k);
    else
      throw std::runtime_error("Not found");
  else
    return v.second;
}

template <typename K, typename T>
typename s21Tree<K, T>::Titerator s21Tree<K, T>::node::searchIT(const K& k) {
  if (v.first > k)
    if (left != NULL)
      return left->searchIT(k);
    else
      return Titerator();
  else if (v.first < k)
    if (right != NULL)
      return right->searchIT(k);
    else
      return Titerator();
  else
    return Titerator(this);
}

template <typename K, typename T>
s21Tree<K, T>::s21Tree() : root(NULL) {}

template <typename K, typename T>
s21Tree<K, T>::s21Tree(K k, T v) {
  root = new node;
  root->parent = NULL;
  root->left = NULL;
  root->right = NULL;
  root->v = std::make_pair(k, v);
}

template <typename K, typename T>
s21Tree<K, T>::s21Tree(const s21Tree& tree) {
  if (tree.root != NULL) {
    root = new node(*tree.root);
  } else
    root = NULL;
}

template <typename K, typename T>
s21Tree<K, T>::s21Tree(s21Tree&& tree) {
  if (tree.root != NULL) {
    root = new node(std::move(*tree.root));
  } else
    root = NULL;
  tree.FREEtree();
}

template <typename K, typename T>
s21Tree<K, T>::s21Tree(
    std::initializer_list<std::pair<const K, T>> const& items) {
  root = new node;
  *root = node(std::get<0>(*items.begin()), std::get<1>(*items.begin()));
  for (auto it = items.begin() + 1; it != items.end(); it++) this->put(*it);
}

template <typename K, typename T>
void s21Tree<K, T>::FREEtree() {
  if (root != NULL) {
    root->FREEnode();
    delete root;
    root = NULL;
  }
}

template <typename K, typename T>
s21Tree<K, T>& s21Tree<K, T>::operator=(s21Tree<K, T>&& src) {
  this->~tree();
  *root = *(src.root);
  return *this;
}

template <typename K, typename T>
std::pair<typename s21Tree<K, T>::Titerator, bool> s21Tree<K, T>::put(
    std::pair<const K, T> p) {
  if (root == NULL) {
    root = new node(p.first, p.second);
    return std::pair<typename s21Tree<K, T>::Titerator, bool>(
        s21Tree<K, T>::Titerator(root), true);
  } else
    return root->Nput(p);
}

template <typename K, typename T>
typename s21Tree<K, T>::Titerator s21Tree<K, T>::force_put(
    std::pair<const K, T> p) {
  if (root == NULL) {
    root = new node(p.first, p.second);
    return s21Tree<K, T>::Titerator(root);
  } else {
    std::pair<s21Tree<K, T>::Titerator, bool> res = root->Nput(p);
    while (res.second == false) {
      if (res.first.val->right == NULL) {
        res.first.val->right = new node(p.first, p.second);
        res.first.val->right->parent = res.first.val;
        return s21Tree<K, T>::Titerator(res.first.val->right);
      } else
        res = res.first.val->right->Nput(p);
    }
    return res.first;
  }
}

template <typename K, typename T>
T& s21Tree<K, T>::search(const K& k) {
  if (root == NULL)
    throw std::runtime_error("Search in empty container");
  else {
    try {
      return root->Nsearch(k);
    } catch (const std::runtime_error& e) {
      throw e;
    }
  }
}

template <typename K, typename T>
typename s21Tree<K, T>::Titerator s21Tree<K, T>::searchIT(const K& k) {
  if (root == NULL)
    throw std::runtime_error("Search in empty container");
  else
    return root->searchIT(k);
}

template <typename K, typename T>
s21::s21Tree<K, T>::Titerator::Titerator() : val(NULL) {}

template <typename K, typename T>
s21::s21Tree<K, T>::Titerator::Titerator(s21Tree::node* n) : val(n) {}

template <typename K, typename T>
typename s21Tree<K, T>::Titerator& s21Tree<K, T>::Titerator::operator++() {
  if (end != NULL)
    throw std::runtime_error("End of tree");
  else {
    if (val->right == NULL)
      if (val->parent != NULL)
        if (val->parent->right == val) {
          s21Tree<K, T>::node* tmp = val->parent;
          while (tmp->parent != NULL && tmp->parent->right == tmp)
            tmp = tmp->parent;
          if (tmp->parent == NULL) {
            end = val;
            val = NULL;
          } else
            val = tmp->parent;
        } else
          val = val->parent;
      else {
        end = val;
        val = NULL;
      }
    else {
      val = val->right;
      while (val->left != NULL) val = val->left;
    }
  }
  return *this;
}

template <typename K, typename T>
typename s21Tree<K, T>::Titerator& s21Tree<K, T>::Titerator::operator--() {
  if (end != NULL) {
    val = end;
    end = NULL;
  } else {
    if (val->left == NULL)
      if (val->parent != NULL)
        if (val->parent->left == val) {
          s21Tree<K, T>::node* tmp = val->parent;
          while (tmp->parent != NULL && tmp->parent->left == tmp)
            tmp = tmp->parent;
          if (tmp->parent == NULL)
            throw std::runtime_error("Beginning of tree");
          else
            val = tmp->parent;
        } else
          val = val->parent;
      else
        throw std::runtime_error("Beginning of tree");
    else {
      val = val->left;
      while (val->right != NULL) val = val->right;
    }
  }
  return *this;
}

template <typename K, typename T>
bool s21Tree<K, T>::erase(Titerator pos) {
  if (pos.val == NULL) throw std::runtime_error("No node in iterator position");
  if (pos.val->parent == NULL) {
    if (pos.val->right == NULL) {
      if (pos.val->left != NULL)
        root = pos.val->left;
      else
        root = NULL;
    } else {
      root = pos.val->right;
      if (pos.val->left != NULL) {
        RebindLeft(pos);
      }
    }
    if (root != NULL) root->parent = NULL;
  } else {
    if (pos.val->parent->right == pos.val) {
      if (pos.val->right == NULL)
        pos.val->parent->right = pos.val->left;
      else {
        pos.val->parent->right = pos.val->right;
        if (pos.val->left != NULL) {
          RebindLeft(pos);
        }
      }
    } else {
      if (pos.val->right == NULL)
        pos.val->parent->left = pos.val->left;
      else {
        pos.val->parent->left = pos.val->right;
        if (pos.val->left != NULL) {
          RebindLeft(pos);
        }
      }
    }
  }
  delete pos.val;
  return true;
}

template <typename K, typename T>
void s21Tree<K, T>::RebindLeft(Titerator pos) {
  node* it = pos.val->right;
  while (it->left != NULL) it = it->left;
  pos.val->left->parent = it;
  it->left = pos.val->left;
}
}  // namespace s21