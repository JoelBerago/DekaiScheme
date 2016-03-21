#ifndef BSTMAP_HPP
#define BSTMAP_HPP

/*
 * Joel Berago and Long Hoang have implemented together the first step
 * (because Long forgot to bring his charger to class). But most of
 * our code has been developed individually.
 */

/**
 * \file bstmap.hpp
 *
 * Creates a Binary Search Tree Map.
 */


#include <utility>
#include <cstddef>
#include <iterator>
#include <iostream>
#include <stdexcept>

using namespace std;

/**
 * \class bstmap
 * \brief Class bstmap
 */
template <class Key, class T>
class bstmap
{
  typedef bstmap<Key, T>     Self;

public:
  typedef Key                key_type;
  typedef T                  data_type;
  typedef pair<const Key, T> value_type;
  typedef unsigned int       size_type;
  typedef int                difference_type;

private:
  /**
   * \class Node
   * \brief Class Node
   */  
  class Node  {
  public:
    Node (value_type val, Node* parent = NULL,  Node* left = NULL, Node* right = NULL) :
      value_m(val), parent_m(parent), left_m(left), right_m(right) {}

    value_type value_m;
    Node* parent_m;
    Node* left_m;
    Node* right_m;

    ~Node() {
      // Don't delete because this Node does not "own" the others
      parent_m = NULL;
      left_m = NULL;
      right_m = NULL;
    }
  };

  Node* root_m;

public:
  template<typename _T>
  class _iterator 
  {
  public:
    typedef input_iterator_tag iterator_category;
    typedef _T value_type;
    typedef int difference_type;
    typedef value_type* pointer;
    typedef value_type& reference;

    friend class bstmap;
    friend class Node;
    
    _iterator() : map_m(NULL), node_m(NULL) {}
    _iterator(const bstmap* map, Node* n = NULL) : map_m(map), node_m(n) {}
    _iterator(const _iterator& x) : map_m(x.map_m), node_m(x.node_m) {}
    
    _iterator& operator=(const _iterator& x)
    {
      map_m = x.map_m;
      node_m = x.node_m;
      return *this;
    }

    reference operator*() const
    {
      try {
	if (node_m == NULL) {
	  // throw runtime_error("Cannot access Node; Node is null");
	  cerr << "ERROR: Cannot access Node; Node is NULL" << endl;
	}
	
	return node_m->value_m;
      } catch (runtime_error& e) {
	throw;
      }
    }

    pointer operator->() const
    {
      try {
	if (node_m == NULL) {
	  // throw runtime_error("Cannot access Node; Node is null");
	  cerr << "ERROR: Cannot access Node; Node is NULL" << endl;
	}

	return &(node_m->value_m);
      } catch (runtime_error& e) {
	throw;
      }
    }

    // Static operator== for const to const iterator and variants.
    //  friend equality comparision operators to allow for "static"
    friend bool operator==(const _iterator& x, const _iterator& y) 
    {
      return (x.node_m == y.node_m);
    }

    friend bool operator!=(const _iterator& x, const _iterator& y)
    {
      return (x.node_m != y.node_m);
    }

    _iterator operator++()
    {
      try {
	if (map_m == NULL) {
	  // throw runtime_error("Cannot access map; map is null");
	  cerr << "Cannot access map; map is NULL" << endl;
	}

	node_m = map_m->_successor(node_m);
	return *this;
      } catch (runtime_error& e) {
	throw;
      }
    }

    _iterator operator++(int) 
    {
      _iterator temp = *this;
      ++(*this);
      return temp;
    }

  private:
    const bstmap* map_m;
    Node* node_m;
  };
  
  typedef _iterator<value_type> iterator;
  typedef _iterator<const value_type> const_iterator;

public:
  // default constructor to create an empty map
  bstmap() : root_m(NULL) {}

  ~bstmap() 
  {
    clear();
    root_m = NULL;
  }

  // overload copy constructor to do a deep copy
  bstmap(const Self& x) 
    : root_m(NULL)
  {
    for (const_iterator i = x.begin(); i != x.end(); ++i) {
      insert(*i); // Insert value at x's i
    }
  }

  // overload assignment to do a deep copy
  Self& operator=(const Self& x) 
  {
    // guard against self assignment
    if (this == &x) {
      return (*this);
    }

    clear();

    for (const_iterator i = x.begin(); i != x.end(); ++i) {
      insert(*i); // Insert value at x's i
    }    
  }

  // accessors:
  iterator begin() 
  {
    return iterator(this, _leftmost_node());
  }
  
  const_iterator begin() const 
  {
    return const_iterator(this, _leftmost_node());
  }
  
  iterator end() 
  {
    if (empty()) { 
      // Return same as begin if is empty;
      return begin();
    } else {
      // Null wrapper. "Point past-the-end"
      return iterator(this, NULL);
    }
  }

  const_iterator end() const 
  {
    if (empty()) { 
      // Return same as begin if is empty;
      return begin();
    } else {
      // Null wrapper. "Point past-the-end"
      return const_iterator(this, NULL);
    }
  }

  bool empty() const 
  {
    return root_m == NULL;
  }

  size_type size() const 
  {
    return _size(root_m);
  }

  pair<iterator, bool> insert(const value_type& x) 
  {
    Node* ret_n;
    bool ret_b;
    pair<Node*, bool> my_pair = _find(x.first, root_m);

    if (my_pair.second == true) {
      // means value is already in tree
      ret_b = false;
      ret_n = my_pair.first;
    } else if (my_pair.first == NULL) {
      // First Node insertaion
      ret_n = root_m = new Node(x);
      ret_b = true;
    } else {
      // create and insert new node
      Node* new_node = new Node(x, my_pair.first);
      if (x.first < (my_pair.first->value_m).first) {
	ret_n = my_pair.first->left_m = new_node;      
      } else {
	ret_n = my_pair.first->right_m = new_node;
      }
      ret_b = true;
    }
    
    return pair<iterator, bool>(iterator(this, ret_n), ret_b);
  }

  void erase(iterator pos) 
  {
    if (pos == NULL) {
      // throw runtime_error("Cannot erase NULL");
      cerr << "ERROR: Cannot erase NULL" << endl;
    }

    Node* del = pos.node_m;
    Node* del_parent = del->parent_m;

    // Case 3: Has two children:
    //   goal: to turn into one or zero children left.
    if (del->left_m && del->right_m) {
      // "Move" predecessor's value to del's by creating 
      //    a new Node with the new value and del's old pointers;
      Node* predecessor = _predecessor(del);
      Node* replacement = new Node(predecessor->value_m, del_parent, del->left_m, del->right_m);

      
      if (del_parent) {
	// Has parents
	if (del_parent->left_m == del) {
	  del_parent->left_m = replacement;
	} else {
	  del_parent->right_m = replacement;
	}
      } else {
	// No parents (is root_m)
	root_m = replacement;
      }
      
      if (del->left_m) {
	del->left_m->parent_m = replacement;
      }
      if (del->right_m) {
	del->right_m->parent_m = replacement;
      }

      delete del;
      // Let erase handle unknown case for predecessor.
      erase(iterator(this, predecessor));
    } else if (del->left_m == NULL && del->right_m == NULL) {
      // Case 1: No Child, simply delete.
      if (del == root_m) {
	// If is root
	delete del;
	root_m = NULL;
      } else {
	// if not root, must have parent.
	if (del == del_parent->left_m) {
	  del_parent->left_m = NULL;
	} else {
	  del_parent->right_m = NULL;
	}

	delete del;
      }
    } else {
      // case 2: has only one child;

      if (del == root_m) {
	// has one child, is root.
	Node* old_child;

	if (del->left_m) {
	  // if has left child;
	  old_child = del->left_m;
	} else {
	  // Must have right child;
	  old_child = del->right_m;
	} 

	delete root_m;
	root_m = old_child;
	root_m->parent_m = NULL;

      } else if (del != root_m) {
	// has one child, is not root, i.e. has parent.
	Node* old_child;

	if (del->left_m) {
	  // if has left child;
	  old_child = del->left_m;
	} else {
	  // Must have right child;
	  old_child = del->right_m;
	} 

	if (del == del_parent->left_m) {
	  // del is parent's left child;
	  del_parent->left_m = old_child;
	} else {
	  // del is parent's right child;
	  del_parent->right_m = old_child;
	}
       
	old_child->parent_m = del_parent;
	delete del;
      } else {
	// Exists for ultra-safety.
	// throw runtime_error("ERROR: Impossible erase case...");
	cerr << "ERROR: impossible erase case..." << endl;
      }
    }
  }

  // return number of given keys deleted
  //   should either be 0 or 1 in bstmap.
  size_type erase(const Key& x)
  {
    iterator it(find(x));
    
    if (it == end()) { 
      // Key not found
      return 0;
    } else {
      erase(it);
      // since Key in bstmaps are unique, can only be 1
      return 1; 
    }
  }
  
  void clear() 
  {
    _recursive_delete(root_m);
    root_m = NULL;
  }

  // map operations:
  iterator find(const Key& x) 
  {
    pair<Node*, bool> my_pair = _find(x, root_m);

    if (my_pair.second) { // Key was found.
      return iterator(this, my_pair.first);
    } else {
      return end();
    }
  }

  const_iterator find(const Key& x) const 
  {
    pair<Node*, bool> my_pair = _find(x, root_m);

    if (my_pair.second) { // Key was found.
      return const_iterator(this, my_pair.first);
    } else {
      return end();
    }
  }


  /**
   * \brief STL Quote: "Because all elements in a map container are unique, the
   * function can only return 1 (if the element is found) or zero
   * (otherwise)."
   */
  size_type count(const Key& x) const 
  {
    // find(x) returns end() if not found.
    if (find(x) != end()) {
      return 1;
    } else {
      return 0;
    }
  }

  T& operator[](const Key& k) 
  {
    iterator it = find(k);
    
    if (it != end()) {   
      // found key
      return (*it).second;
    } else {               
      // not found --> create new empty
      insert( value_type(k, T()));
      return (*this)[k];
    }
  }

  iterator min()
  {
    return iterator(this, _leftmost_node());
  }

  const_iterator min() const
  {
    return const_iterator(this, _leftmost_node());
  }

  iterator max()
  {
    return iterator(this, _rightmost_node());
  }

  const_iterator max() const
  {
    return const_iterator(this, _rightmost_node());
  }

  // Private functions for internal helping.
private:
  // Finds a Node from given key and subtree.
  //   returns a Node pointer and bool pair
  //   bool holds if given key was found in map.
  pair<Node*, bool> _find(const key_type& key, Node* const subtree, Node* const parent = NULL) const 
  {
    if (subtree == NULL) {
      return pair<Node*, bool>(parent, false);
    } 

    if (subtree->value_m.first == key) {
      return pair<Node*, bool>(subtree, true);
    } else if (subtree->value_m.first > key) {
      return _find(key, subtree->left_m, subtree);
    } else {
      return _find(key, subtree->right_m, subtree);
    }
  }

  // Returns a pointer to the leftmost node.
  //   if head is unspecified, assume start at root.
  Node* _leftmost_node(Node* const head = NULL) const
  {
    Node* curr = head;
    
    if (curr == NULL) {
      curr = root_m;
    }

    if (curr != NULL) {
      while (curr->left_m != NULL) {
	curr = curr->left_m;
      }
    }

    return curr;    
  }

  // Returns a pointer to the rightmost node.
  //   if head is unspecificed, assume start at root.
  Node* _rightmost_node(Node* const head = NULL) const
  {
    Node* curr = head;

    if (curr == NULL) {
      curr = root_m;
    }

    if (curr != NULL) {
      while (curr->right_m != NULL) {
	curr = curr->right_m;
      }
    }

    return curr;
  }

  
  // returns the successor of a Node.
  Node* _successor(Node* const n) const
  {
    if (n == NULL) {
      // throw runtime_error("Cannot success NULL");
      cerr << "Cannot success NULL" << endl;
    }

    // If node has a right child,
    //   successor is the min(node.right)
    if (n->right_m) {
      return _leftmost_node(n->right_m);
    } else {
      Node* curr = n;
      Node* parent = n->parent_m;

      // successor is the parent where node is right child of parent.
      while (parent != NULL && curr == parent->right_m) {
	curr = parent;
	parent = parent->parent_m;
      }

      return parent;
    }
  }

  // Predecessor is the largest element in the left subtree from n.
  Node* _predecessor(Node* const n) const
  {
    if (n == NULL) {
      // throw runtime_error("Cannot predecess NULL");
      cerr << "Cannot predecess NULL" << endl;
    } else if (n == _leftmost_node()) { 
      // throw runtime_error("Cannot predecess lowest node.");
      cerr << "Cannot predecess lowest node" << endl;
    }
    
    if (n->left_m) {
      // Predecessor is the largest element in left subtree.
      return _rightmost_node(n->left_m);
    } else {
      return n->parent_m;
    }
  }

  // recursively delete left, then right, then myself.
  void _recursive_delete(Node* n) 
  {
    if (n != NULL) {
      _recursive_delete(n->left_m);
      _recursive_delete(n->right_m);

      delete n;
      n = NULL;
    }
  }

  // Gets size of left tree + right tree + 1 (root)
  size_type _size(Node* const n) const 
  {
    if (n == NULL) {
      return 0;
    } else {
      // root + size(left), + size(right)
      return 1 + _size(n->left_m) + _size(n->right_m);
    }
  }
};

#endif
