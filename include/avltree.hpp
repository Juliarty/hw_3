#include <cmath>
#include <iostream>
#include <stdexcept>
#include <string>

template <typename TKey> class AvlTree;

template <typename TKey> class TreeNode {
public:
  TreeNode(TKey key, int height = 1)
      : m_Key(key), m_Height(height), m_LeftChild(nullptr),
        m_RightChild(nullptr) {}
  friend AvlTree<TKey>;

protected:
  TKey m_Key;
  int m_Height;
  TreeNode<TKey> *m_LeftChild;
  TreeNode<TKey> *m_RightChild;
};

template <typename TKey> class AvlTree {
public:
  AvlTree() : m_Root(nullptr) {}
  AvlTree(const AvlTree<TKey> &other);
  ~AvlTree() { remove_all(m_Root); }
  void add(TKey);
  const TreeNode<TKey> *next(TKey) const;
  const TreeNode<TKey> *prev(TKey) const;
  bool exist(TKey) const;
  void remove(TKey);
  void clear();
  AvlTree<TKey> &operator=(const AvlTree<TKey> &other);

private:
  TreeNode<TKey> *m_Root;
  static void remove_all(TreeNode<TKey> *root);
  static TreeNode<TKey> *add(TKey, TreeNode<TKey> *);
  static const TreeNode<TKey> *find(TKey, const TreeNode<TKey> *);
  static TreeNode<TKey> *remove(TKey, TreeNode<TKey> *);
  static const TreeNode<TKey> *find_max(const TreeNode<TKey> *);
  static TreeNode<TKey> *balance(TreeNode<TKey> *);
  static int get_balance(const TreeNode<TKey> *);
  static TreeNode<TKey> *small_left_rotate(TreeNode<TKey> *);
  static TreeNode<TKey> *small_right_rotate(TreeNode<TKey> *);
  static int get_children_num(const TreeNode<TKey> *);
  static int get_height(const TreeNode<TKey> *);
  static void fix_node(TreeNode<TKey> *);
  static TreeNode<TKey> *copy(TreeNode<TKey> *);
};

template <typename TKey>
TreeNode<TKey> *AvlTree<TKey>::copy(TreeNode<TKey> *root) {
  if (root == nullptr) {
    return nullptr;
  }

  auto result = new TreeNode<TKey>(root->m_Key, root->m_Height);
  result->m_LeftChild = copy(root->m_LeftChild);
  result->m_RightChild = copy(root->m_RightChild);

  return result;
}

template <typename TKey> void AvlTree<TKey>::remove_all(TreeNode<TKey> *root) {
  if (root == nullptr) {
    return;
  }
  auto left_child = root->m_LeftChild;
  auto right_child = root->m_RightChild;
  delete root;

  remove_all(left_child);
  remove_all(right_child);
}

template <typename TKey> void AvlTree<TKey>::clear() { remove_all(m_Root); }

template <typename TKey> AvlTree<TKey>::AvlTree(const AvlTree<TKey> &other) {
  m_Root = copy(other.m_Root);
}

template <typename TKey>
AvlTree<TKey> &AvlTree<TKey>::operator=(const AvlTree<TKey> &other) {
  if (this == &other) {
    return *this;
  }
  remove_all(m_Root);
  m_Root = copy(other.m_Root);
  return *this;
}

template <typename TKey> void AvlTree<TKey>::add(TKey key) {
  this->m_Root = add(key, this->m_Root);
}

template <typename TKey> void AvlTree<TKey>::remove(TKey key) {
  this->m_Root = remove(key, this->m_Root);
}

// Returns the root pointer to the modified tree.
template <typename TKey>
TreeNode<TKey> *AvlTree<TKey>::add(TKey key, TreeNode<TKey> *node) {
  if (node == nullptr) {
    return new TreeNode<TKey>(key);
  }

  if (key < node->m_Key) {
    node->m_LeftChild = add(key, node->m_LeftChild);
  } else if (key > node->m_Key) {
    node->m_RightChild = add(key, node->m_RightChild);
  }

  if (node != nullptr) {
    fix_node(node);
  }

  return balance(node);
}

template <typename TKey>
const TreeNode<TKey> *AvlTree<TKey>::find(TKey key,
                                          const TreeNode<TKey> *root) {
  if (root == nullptr) {
    return nullptr;
  }
  if (key == root->m_Key) {
    return root;
  } else if (key < root->m_Key) {
    return AvlTree<TKey>::find(key, root->m_LeftChild);
  } else {
    return AvlTree<TKey>::find(key, root->m_RightChild);
  }
}

template <typename TKey> bool AvlTree<TKey>::exist(TKey key) const {
  return find(key, this->m_Root) != nullptr;
}

// Returns the root pointer to the modified tree.
template <typename TKey>
TreeNode<TKey> *AvlTree<TKey>::remove(TKey key, TreeNode<TKey> *root) {
  if (root == nullptr) {
    return nullptr;
  }

  if (key < root->m_Key) {
    root->m_LeftChild = remove(key, root->m_LeftChild);
  } else if (key > root->m_Key) {
    root->m_RightChild = remove(key, root->m_RightChild);
  } else {
    if (root->m_LeftChild == nullptr && root->m_RightChild == nullptr) {
      delete root;
      root = nullptr;
    } else if (root->m_LeftChild != nullptr && root->m_RightChild == nullptr) {
      auto tmp = root->m_LeftChild;
      delete root;
      root = tmp;
    } else if (root->m_LeftChild == nullptr && root->m_RightChild != nullptr) {
      auto tmp = root->m_RightChild;
      delete root;
      root = tmp;
    } else {
      auto left_max_value = find_max(root->m_LeftChild)->m_Key;
      root->m_Key = left_max_value;
      root->m_LeftChild = remove(left_max_value, root->m_LeftChild);
    }
  }

  if (root != nullptr) {
    fix_node(root);
  }

  return balance(root);
}
template <typename TKey>
const TreeNode<TKey> *AvlTree<TKey>::find_max(const TreeNode<TKey> *root) {
  if (root->m_RightChild == nullptr) {
    return root;
  }

  return AvlTree<TKey>::find_max(root->m_RightChild);
}
template <typename TKey>
const TreeNode<TKey> *AvlTree<TKey>::next(TKey key) const {
  TreeNode<TKey> *current_node = this->m_Root;
  TreeNode<TKey> *res = nullptr;
  while (current_node != nullptr) {
    if (key < current_node->m_Key) {
      res = current_node;
      current_node = current_node->m_LeftChild;
    } else {
      current_node = current_node->m_RightChild;
    }
  }
  return res;
}
template <typename TKey>
const TreeNode<TKey> *AvlTree<TKey>::prev(TKey key) const {
  TreeNode<TKey> *current_node = this->m_Root;
  TreeNode<TKey> *res = nullptr;
  while (current_node != nullptr) {
    if (key > current_node->m_Key) {
      res = current_node;
      current_node = current_node->m_RightChild;
    } else {
      current_node = current_node->m_LeftChild;
    }
  }
  return res;
}

template <typename TKey>
TreeNode<TKey> *AvlTree<TKey>::balance(TreeNode<TKey> *root) {
  if (root == nullptr) {
    return nullptr;
  }

  if (std::abs(get_balance(root)) < 2) {
    return root;
  }

  if (get_balance(root) == 2) {
    if (get_balance(root->m_LeftChild) == -1) {
      root->m_LeftChild = small_left_rotate(root->m_LeftChild);
    }
    return small_right_rotate(root);
  } else {
    if (get_balance(root->m_RightChild) == 1) {
      root->m_RightChild = small_right_rotate(root->m_RightChild);
    }
    return small_left_rotate(root);
  }
}
template <typename TKey>
int AvlTree<TKey>::get_balance(const TreeNode<TKey> *root) {
  if (root == nullptr) {
    return 0;
  }

  return get_height(root->m_LeftChild) - get_height(root->m_RightChild);
}
template <typename TKey>
TreeNode<TKey> *AvlTree<TKey>::small_left_rotate(TreeNode<TKey> *root) {
  auto new_root = root->m_RightChild;

  if (new_root == nullptr) {
    throw std::exception();
  }

  root->m_RightChild = new_root->m_LeftChild;
  new_root->m_LeftChild = root;

  fix_node(root);
  fix_node(new_root);

  return new_root;
}
template <typename TKey>
TreeNode<TKey> *AvlTree<TKey>::small_right_rotate(TreeNode<TKey> *root) {
  auto new_root = root->m_LeftChild;

  if (new_root == nullptr) {
    throw std::exception();
  }

  root->m_LeftChild = new_root->m_RightChild;
  new_root->m_RightChild = root;

  fix_node(root);
  fix_node(new_root);

  return new_root;
}

template <typename TKey> void AvlTree<TKey>::fix_node(TreeNode<TKey> *node) {
  node->m_Height =
      std::max(get_height(node->m_LeftChild), get_height(node->m_RightChild)) +
      1;
}

template <typename TKey>
int AvlTree<TKey>::get_children_num(const TreeNode<TKey> *node) {
  if (node == nullptr) {
    return 0;
  }

  return node->m_LeftChildren_num + node->m_RightChildren_num;
}
template <typename TKey>

int AvlTree<TKey>::get_height(const TreeNode<TKey> *node) {
  if (node == nullptr) {
    return 0;
  }
  return node->m_Height;
}