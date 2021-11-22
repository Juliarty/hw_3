#include <cmath>
#include <iostream>
#include <stdexcept>
#include <string>
#include <tuple>

using std::tuple;

template <typename TKey> class AvlTree;
template <typename T> class AvlTreeConstIterator;

template <typename TKey> class TreeNode {
public:
  TreeNode(TKey key, int height = 1)
      : m_Key(key), m_Height(height), m_LeftChild(nullptr),
        m_RightChild(nullptr), m_Prev(nullptr), m_Next(nullptr),
        m_LeftmostNode(this), m_RightmostNode(this) {}
  friend AvlTree<TKey>;
  friend AvlTreeConstIterator<TKey>;

  const TreeNode *getPrev() { return m_Prev; }
  const TreeNode *getNext() { return m_Prev; }

protected:
  TKey m_Key;
  int m_Height;
  TreeNode<TKey> *m_LeftChild;
  TreeNode<TKey> *m_RightChild;
  TreeNode<TKey> *m_Prev;
  TreeNode<TKey> *m_Next;
  TreeNode<TKey> *m_LeftmostNode;
  TreeNode<TKey> *m_RightmostNode;
};

template <typename TKey> class AvlTree {
public:
  typedef AvlTreeConstIterator<TKey> const_iterator;

  AvlTree() : m_Root(nullptr) {}
  AvlTree(const AvlTree<TKey> &other);
  ~AvlTree() { removeAll(m_Root); }
  void add(TKey);
  const TreeNode<TKey> *next(TKey) const;
  const TreeNode<TKey> *prev(TKey) const;
  bool exists(TKey) const;
  void remove(TKey);
  void clear();
  const_iterator begin() const;
  const_iterator end() const;

  AvlTree<TKey> &operator=(const AvlTree<TKey> &other);

private:
  TreeNode<TKey> *m_Root;
  static void removeAll(TreeNode<TKey> *root);
  static TreeNode<TKey> *add(TKey, TreeNode<TKey> *);
  static const TreeNode<TKey> *find(TKey, const TreeNode<TKey> *);
  static TreeNode<TKey> *remove(TKey, TreeNode<TKey> *);
  static const TreeNode<TKey> *findMax(const TreeNode<TKey> *);
  static TreeNode<TKey> *balance(TreeNode<TKey> *);
  static int getBalance(const TreeNode<TKey> *);
  static TreeNode<TKey> *smallLeftRotate(TreeNode<TKey> *);
  static TreeNode<TKey> *smallRightRotate(TreeNode<TKey> *);
  static int getChildrenNum(const TreeNode<TKey> *);
  static int getHeight(const TreeNode<TKey> *);
  static void fixNode(TreeNode<TKey> *);
  static TreeNode<TKey> *copy(TreeNode<TKey> *);
};

template <typename TKey>
typename AvlTree<TKey>::const_iterator AvlTree<TKey>::begin() const {
  if (m_Root != nullptr) {
    return AvlTree<TKey>::const_iterator(m_Root->m_LeftmostNode, nullptr);
  }
  return AvlTree<TKey>::const_iterator(nullptr, nullptr);
}

template <typename TKey>
typename AvlTree<TKey>::const_iterator AvlTree<TKey>::end() const {
  if (m_Root != nullptr) {
    return AvlTree<TKey>::const_iterator(nullptr, m_Root->m_RightmostNode);
  }
  return AvlTree<TKey>::const_iterator(nullptr, nullptr);
}

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

template <typename TKey> void AvlTree<TKey>::removeAll(TreeNode<TKey> *root) {
  if (root == nullptr) {
    return;
  }
  auto leftChild = root->m_LeftChild;
  auto rightChild = root->m_RightChild;
  delete root;

  removeAll(leftChild);
  removeAll(rightChild);
}

template <typename TKey> void AvlTree<TKey>::clear() { removeAll(m_Root); }

template <typename TKey> AvlTree<TKey>::AvlTree(const AvlTree<TKey> &other) {
  m_Root = copy(other.m_Root);
}

template <typename TKey>
AvlTree<TKey> &AvlTree<TKey>::operator=(const AvlTree<TKey> &other) {
  if (this == &other) {
    return *this;
  }
  removeAll(m_Root);
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

  fixNode(node);

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

template <typename TKey> bool AvlTree<TKey>::exists(TKey key) const {
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
    TreeNode<TKey> tmp = *root;
    if (root->m_LeftChild == nullptr && root->m_RightChild == nullptr) {
      delete root;
      root = nullptr;
    } else if (root->m_LeftChild != nullptr && root->m_RightChild == nullptr) {
      delete root;
      root = tmp.m_LeftChild;
    } else if (root->m_LeftChild == nullptr && root->m_RightChild != nullptr) {
      delete root;
      root = tmp.m_RightChild;
    } else {
      const TreeNode<TKey> *leftMax = findMax(root->m_LeftChild);
      if (leftMax != nullptr) {
        root->m_Key = leftMax->m_Key;
        root->m_LeftChild = remove(leftMax->m_Key, root->m_LeftChild);
      }
    }
  }

  if (root != nullptr) {
    fixNode(root);
  }

  return balance(root);
}
template <typename TKey>
const TreeNode<TKey> *AvlTree<TKey>::findMax(const TreeNode<TKey> *root) {
  if (root == nullptr) {
    return nullptr;
  }

  if (root->m_RightChild == nullptr) {
    return root;
  }

  return AvlTree<TKey>::findMax(root->m_RightChild);
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
  TreeNode<TKey> *currentNode = this->m_Root;
  TreeNode<TKey> *res = nullptr;
  while (currentNode != nullptr) {
    if (key > currentNode->m_Key) {
      res = currentNode;
      currentNode = currentNode->m_RightChild;
    } else {
      currentNode = currentNode->m_LeftChild;
    }
  }
  return res;
}

template <typename TKey>
TreeNode<TKey> *AvlTree<TKey>::balance(TreeNode<TKey> *root) {
  if (root == nullptr) {
    return nullptr;
  }

  if (std::abs(getBalance(root)) < 2) {
    return root;
  }

  if (getBalance(root) == 2) {
    if (getBalance(root->m_LeftChild) == -1) {
      root->m_LeftChild = smallLeftRotate(root->m_LeftChild);
    }
    return smallRightRotate(root);
  } else {
    if (getBalance(root->m_RightChild) == 1) {
      root->m_RightChild = smallRightRotate(root->m_RightChild);
    }
    return smallLeftRotate(root);
  }
}
template <typename TKey>
int AvlTree<TKey>::getBalance(const TreeNode<TKey> *root) {
  if (root == nullptr) {
    return 0;
  }

  return getHeight(root->m_LeftChild) - getHeight(root->m_RightChild);
}
template <typename TKey>
TreeNode<TKey> *AvlTree<TKey>::smallLeftRotate(TreeNode<TKey> *root) {
  if (root == nullptr) {
    return nullptr;
  }
  auto newRoot = root->m_RightChild;

  if (newRoot == nullptr) {
    throw std::exception();
  }

  root->m_RightChild = newRoot->m_LeftChild;
  newRoot->m_LeftChild = root;

  fixNode(root);
  fixNode(newRoot);

  return newRoot;
}
template <typename TKey>
TreeNode<TKey> *AvlTree<TKey>::smallRightRotate(TreeNode<TKey> *root) {
  if (root == nullptr) {
    return nullptr;
  }
  auto newRoot = root->m_LeftChild;

  if (newRoot == nullptr) {
    throw std::exception();
  }

  root->m_LeftChild = newRoot->m_RightChild;
  newRoot->m_RightChild = root;

  fixNode(root);
  fixNode(newRoot);

  return newRoot;
}

template <typename TKey> void AvlTree<TKey>::fixNode(TreeNode<TKey> *node) {
  if (node == nullptr) {
    return;
  }

  node->m_Height =
      std::max(getHeight(node->m_LeftChild), getHeight(node->m_RightChild)) + 1;

  node->m_Prev = nullptr;
  node->m_Next = nullptr;
  node->m_LeftmostNode = node;
  node->m_RightmostNode = node;

  if (node->m_LeftChild != nullptr) {
    node->m_Prev = node->m_LeftChild->m_RightmostNode;
    node->m_LeftChild->m_RightmostNode->m_Next = node;
    node->m_LeftmostNode = node->m_LeftChild->m_LeftmostNode;
  }

  if (node->m_RightChild != nullptr) {
    node->m_Next = node->m_RightChild->m_LeftmostNode;
    node->m_RightChild->m_LeftmostNode->m_Prev = node;
    node->m_RightmostNode = node->m_RightChild->m_RightmostNode;
  }
}

template <typename TKey>
int AvlTree<TKey>::getChildrenNum(const TreeNode<TKey> *node) {
  if (node == nullptr) {
    return 0;
  }

  return node->m_LeftChildren_num + node->m_RightChildren_num;
}
template <typename TKey>

int AvlTree<TKey>::getHeight(const TreeNode<TKey> *node) {
  if (node == nullptr) {
    return 0;
  }
  return node->m_Height;
}

template <typename T> class AvlTreeConstIterator {
public:
  typedef typename std::allocator<T>::difference_type difference_type;
  typedef typename std::allocator<T>::value_type value_type;
  typedef T &reference;
  typedef const T &const_reference;
  typedef T *pointer;
  typedef const T *const_pointer;
  typedef std::bidirectional_iterator_tag iterator_category;

  const T &operator*() const;
  AvlTreeConstIterator &operator++();
  AvlTreeConstIterator operator++(int);
  AvlTreeConstIterator &operator--();
  AvlTreeConstIterator operator--(int);

  bool operator==(const AvlTreeConstIterator &) const;
  bool operator!=(const AvlTreeConstIterator &) const;
  friend AvlTree<T>;

private:
  AvlTreeConstIterator(TreeNode<T> *node, TreeNode<T> *prevNode)
      : m_Node(node), m_PrevNode(prevNode) {}
  TreeNode<T> *m_Node;
  TreeNode<T> *m_PrevNode;
};

template <typename T> const T &AvlTreeConstIterator<T>::operator*() const {
  return m_Node->m_Key;
}

template <typename T>
AvlTreeConstIterator<T> &AvlTreeConstIterator<T>::operator++() {
  m_PrevNode = m_Node;
  if (m_Node != nullptr) {
    m_Node = m_Node->m_Next;
  }
  return *this;
}

template <typename T>
AvlTreeConstIterator<T> AvlTreeConstIterator<T>::operator++(int) {
  auto res = *this;
  m_PrevNode = m_Node;
  if (m_Node != nullptr) {
    m_Node = m_Node->m_Next;
  }
  return res;
}

template <typename T>
AvlTreeConstIterator<T> &AvlTreeConstIterator<T>::operator--() {
  m_Node = m_PrevNode;
  if (m_Node != nullptr) {
    m_PrevNode = m_Node->m_Prev;
  }
  return *this;
}

template <typename T>
AvlTreeConstIterator<T> AvlTreeConstIterator<T>::operator--(int) {
  auto res = *this;
  m_Node = m_PrevNode;
  if (m_Node != nullptr) {
    m_PrevNode = m_Node->m_Prev;
  }
  return res;
}

template <typename T>
bool AvlTreeConstIterator<T>::operator==(
    const AvlTreeConstIterator &other) const {
  return m_Node == other.m_Node;
}

template <typename T>
bool AvlTreeConstIterator<T>::operator!=(
    const AvlTreeConstIterator &other) const {
  return !(*this == other);
}