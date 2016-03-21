#ifndef HASHTABLEMAP_HPP
#define HASHTABLEMAP_HPP

/*
 * Joel Berago and Long Hoang have implemented together the first steps
 * (because Long forgot to bring his charger to class). But most of
 * our code has been developed individually.
 */

/**
 * \file hashtablemap.hpp
 *
 * Creates a hashtablemap.
 */

#include <utility>
#include <stdexcept>
#include <iostream>
#include <iterator>
#include <cstddef>
#include <sstream>
#include <cmath>

#include "bstmap.hpp"

template <class Key, class T>
/**
 * \class hashtablemap
 * \brief Class hashtablemap
 */
class hashtablemap
{
  typedef hashtablemap<Key, T>     Self;

public:
  typedef Key                key_type;
  typedef T                  data_type;
  typedef pair<const Key, T> value_type;
  typedef unsigned int       size_type;
  typedef int                difference_type;

private:
  /**
   * \struct Node
   *
   * \brief data structure which holds the data
   * Node is used instead of directly value_type so as pointers can be used better.
   */
  struct Node {
  public:
    Node(value_type val) : value_m(val) {}
    value_type value_m;
  };

  typedef bstmap<Key, Node*> bucket_type;

  // Hash Table array of bstmaps.
  bucket_type* table_m;

  // holds the number of values/elements in map
  // note: size_m =/= TABLE_SIZE.
  size_type size_m;

  // strings hash well with constants 31, 33, 37, 39, 41 well with less than 7 collisions.
  //  41 arbitrarily chosen because it's the largest of the list, prime, and 42 - 1
  size_type static const TABLE_SIZE = 41;



public:
  ////////////////////////////////////////////////////////////////////////////////
  // template class iterator

  template<typename _T>
  class _iterator 
  {
    typedef input_iterator_tag iterator_category;
    typedef _T value_type;
    typedef int difference_type;
    typedef value_type* pointer;
    typedef value_type& reference;

    friend class hashtablemap;
  private:
    // pointer to a table instance.    
    const hashtablemap* table_m;
    
    // pointer to a value_type.
    Node* node_m;

  public:
    _iterator(const hashtablemap* my_table = NULL, Node* my_node = NULL)
      : table_m(my_table), node_m(my_node) {}

    _iterator(const _iterator& x) 
      : table_m(x.table_m), node_m(x.node_m){}

    _iterator& operator=(const _iterator& x) {
      node_m = x.node_m;
      table_m = x.table_m;
      return *this;
    }

    reference operator*() 
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

    // Sets the node to the next successor node.
    _iterator& operator++() 
    {
      node_m = table_m->_successor_node(node_m);
      return (*this);
    }

    _iterator& operator++(int) 
    {
      iterator temp = *this;
      ++(*this);
      return temp;
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
  };

  // template class _iterator
  ////////////////////////////////////////////////////////////////////////////////

  typedef _iterator<value_type> iterator;
  typedef _iterator<const value_type> const_iterator;

public:
  ///\brief Default constructor to create an empty map
  hashtablemap() 
    : table_m(new bucket_type[TABLE_SIZE]), size_m(0) {}

  ///\brief Copy constructor with deep copying.
  hashtablemap(const Self& x)
    : table_m(new bucket_type[TABLE_SIZE]), size_m(0)
  {
    // Manual reinsert into a new table.
    for (const_iterator i = x.begin(); i != x.end(); ++i) {
      insert(*i);
    }
  }

  /**
   * \brief Destructor
   */
  ~hashtablemap()
  {
    delete [] table_m;
  }

  ///\brief Assignment operator with deep copying.
  Self& operator=(const Self& x) {
    /// guard against self assignment
    if (this == &x) {
      return *this;
    }

    // Make anew before inserting.
    clear();
    
    for (const_iterator i = x.begin(); i != x.end(); ++i) {
      insert(*i);      /// same balancing problem as for copy constructor above
    }
  }

  /** 
   * \brief Returns an iterator to the first non-empty bucket.
   */
  iterator begin() {
    if (empty()) {
      return end();
    }

    // Finds the first node in the first non-empty bucket
    const bucket_type* first_filled_bucket = _find_next_nonempty_bucket();

    // First filled bucket cannot be NULL, else the whole table should be empty() == true.
    // Node* bstmap::begin()
    Node* first_node = first_filled_bucket->begin()->second;
    
    return iterator(this, first_node);
  }

  /** 
   * \brief Returns a const iterator to the first non-empty bucket.
   */  
  const_iterator begin() const {
    if (empty()) {
      return end();
    }

    // Finds the first node in the first non-empty bucket
    const bucket_type* first_filled_bucket = _find_next_nonempty_bucket();

    // First filled bucket cannot be NULL, else the whole table should be empty() == true.
    // bstmap->begin()
    Node* first_node = first_filled_bucket->begin()->second;
    
    return const_iterator(this, first_node);
  }

  /** 
   * \brief Returns an iterator to a wrapper on NULL
   */  
  iterator end()
  {
    // + 1 past the end.
    return iterator(this, NULL);
  }

  /** 
   * \brief Returns an iterator to a wrapper on NULL
   */  
  const_iterator end() const 
  {
    // + 1 past the end.
    return const_iterator(this, NULL);
  }

  /** 
   * \brief Checks if map has no inserted elements.
   */  
  bool empty() const 
  {
    return (size_m == 0);
  }

  /** 
   * \brief Returns the number of elements inserted.
   */  
  size_type size() const 
  {
    return size_m;
  }

  /** 
   * \brief Inserts an element by a given pair
   */  
  pair<iterator,bool> insert(const value_type& x) 
  {
    iterator it = find(x.first);

    // element exists already
    //  find() returns end() if element does not exist.
    if (it != end()) {
      return pair<iterator, bool>(iterator(this, it.node_m), false);
    } else {
      // insert new node
      int hashvalue = _hash(x.first);
      Node* new_node = new Node(x);
      
      const pair<Key, Node*> keynodepair(x.first, new_node);
      table_m[hashvalue].insert(keynodepair);

      ++size_m;

      return pair<iterator, bool>(iterator(this, it.node_m), true);
    }
  }

  /**
   * \brief Erases from an iterator
   */
  void erase(iterator pos) {
    if (pos == NULL) {
      throw runtime_error("Cannot erase Null iterator");
    }

    erase(pos->first);
  }

  /**
   * \brief Erases by a given key value.
   * \return size_type number of elements erased 
   */
  size_type erase(const Key& x) {
    iterator it = find(x);
    
    if (it == end()) {
      return 0;
    }
    
    int hashvalue = _hash(x);

    table_m[hashvalue].erase(x);
    --size_m;
    
    return 1; // since Key in maps are unique, can only be 1
  }
  
  /**
   * \brief Empty all buckets
   */
  void clear() {
    if (empty()) {
      return;
    }

    delete [] table_m;
    table_m = new bucket_type[TABLE_SIZE];
    size_m = 0;
  }

  /** 
   * \brief Finds the element with given key.
   * \return An iterator pointing to the element found.
   *   returns end() if not found.
   */  
  iterator find(const Key& x) {
    int hashvalue = _hash(x);
    
    bucket_type* ptr = table_m + hashvalue;
    typename bucket_type::iterator it = ptr->find(x);

    if (it == ptr->end()) {
      return end();
    }

    return iterator(this, (*it).second);
  }

  /** 
   * \brief Finds the element with given key.
   * \return A const iterator pointing to the element found.
   *    returns end() if not found.
   */  
  const_iterator find(const Key& x) const {
    int hashvalue = _hash(x);
    
    bucket_type* ptr = table_m + hashvalue;
    typename bucket_type::iterator it = ptr->find(x);

    if (it == ptr->end()) {
      return end();
    }

    return const_iterator(this, (*it).second);
  }

  /** 
   * \brief Counts the number of times an element is found in the map.
   */  
  size_type count(const Key& x) const 
  {
    // if found, then there must only be one instance as hashtable/bstmap only have unique entries.
    if (find(x) != end()) {
      return 1;
    } else {
      return 0;
    }
  }
  
  /** 
   * \brief Subscript operator accessor.
   */  
  T& operator[](const Key& k) 
  {
    iterator it = find(k);

    if (it != end()) {
      // found key
      return (*it).second;
    } else {
      // not found -> create empty
      insert(value_type(k, T()));
      return (*this)[k];
    }
  }

  /** 
   * \brief Alternative to the subscript operator, 
   * but throws out_of_range exception when element is not found.
   */  
  T& at(const Key& k) 
  {
    iterator it = find(k);

    if (it != end()) {
      // found key
      return (*it).second;
    } else {
      // not found -> throw out_of_range
      throw out_of_range("K is not found");
    }
  }

private:
  
  /** 
   * \brief Hashs a given key by turning into a string then hashs with
   * hash = SUM((s[n]*128^n) mod TABLE_SIZE
   */  

  int _hash(const Key& k) const
  {
    stringstream ss;
    ss << k;
    const char* c_str = ss.str().c_str();
    int hashvalue = 5381;
    double temp;
    for(double i = 1; *c_str != '\0'; ++c_str, ++i) {
      temp  = (*c_str)*pow(128.0, (double) i);
      hashvalue += (int) floor(temp);
    }
    
    hashvalue %= TABLE_SIZE;

    // returns a value between 0 and TABLE_SIZE (41)
    return hashvalue;
  }

  /**
   * \brief Finds the next nonempty bucket after a given index
   * \return A const pointer to the first after a given index nonempty bucket.
   */
  const bucket_type* _find_next_nonempty_bucket(int const index = -1) const {
    for (int i = index + 1; i < TABLE_SIZE; ++i) {
      if (!table_m[i].empty()) {
	return &table_m[i];
      }
    }

    return NULL;
  }

  /**
   * \brief Used for iteration.
   * \return Node* returns NULL if there are no following Nodes
   */
  Node* _successor_node(Node* n) const {    
    Key k = n->value_m.first;
    int hashvalue = _hash(k);

    bucket_type* curr_bucket = table_m + hashvalue;
    typename bucket_type::iterator next = ++(curr_bucket->find(k));

    // is there an element in current bucket?
    if (next != curr_bucket->end()) {
      return next->second;
    }

    const bucket_type* next_bucket = _find_next_nonempty_bucket(hashvalue);
      
    if (!next_bucket) {
      return NULL;
    }

    // return first node of next non-empty bucket
    return next_bucket->begin()->second;
  }
};

#endif 
