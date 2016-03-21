/**
 * \file linkedlist.hpp
 *
 * Encapsulates an abstract interface layer for a sequence ADT,
 * without using member functions. Makes no assumptions about what
 * kind of concrete type Cell will be defined to be.
 */

#ifndef LINKEDLIST_HPP
#define LINKEDLIST_HPP

#include "linkedlist_internals.hpp"

/**
 * \brief Size of the list from the node n
 * \return List size
 */
int list_size(const Node* n);

/**
 * \brief Value at the position i (starting from 0)
 * \return Pointer to the value at position i in the list
 */
Cell* list_ith(Node* n, unsigned int i);

/**
 * \brief Erase the value at position at node 'pos' of a list with head at 'n'
 * \return Pointer to the value after the position pos
 */
Node* list_erase(Node* n, Node* pos);

/**
 * \brief Insert the value before the position 'pos' of a list with head at 'n'
 * \return Pointer to the inserted node
 */
Node* list_insert(Node* n, Node* pos, Cell* c);

/**
 * \brief Insert an int  before the position 'pos' of a list with head at 'n'
 * \return Pointer to the inserted node 
 */
Node* list_insert_int(Node* n, Node* pos, const int value);

/**
 * \brief Insert a double before the position 'pos' of a list with head at 'n'
 * \return Pointer to the inserted node 
 */
Node* list_insert_double(Node* n, Node* pos, const double value);

/**
 * \brief Insert a symbol before the position 'pos' of a list with head at 'n'
 * \return Pointer to the inserted node 
 */
Node* list_insert_symbol(Node* n, Node* pos, const char* value);

#endif // LINKEDLIST_HPP
