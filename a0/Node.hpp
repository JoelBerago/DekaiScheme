/**
 * \file Node.hpp
 *
 * Implements a node for use in a singly linked list. Each node has a
 * pointer to the next node, as well as a pointer to an element stored
 * using the Cell class so that the list can contain any mix of ints,
 * doubles, and symbols.
 */

#ifndef NODE_HPP
#define NODE_HPP

#include "Cell.hpp"

/**
 * \class Node
 * \brief A node within a singly linked list.
 */
struct Node {
  Node(Cell* elem, Node* next) : elem_m(elem), next_m(next) {}

  Cell* elem_m;
  Node* next_m;
};

#endif // NODE_HPP
