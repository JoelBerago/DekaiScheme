/**
 * \file cons.hpp
 *
 * Encapsulates an abstract interface layer for a cons list ADT,
 * without using member functions. Makes no assumptions about what
 * kind of concrete type Cell will be defined to be.
 */

#ifndef CONS_HPP
#define CONS_HPP

#include "Cell.hpp"
#include <string>
#include <iostream>

/**
 * \brief The null pointer value.
 */
extern Cell* const nil;

/**
 * \brief Make an int cell.
 * \param i The initial int value to be stored in the new cell.
 */
inline Cell* make_int(const int i)
{
  return (Cell*) new IntCell(i);
}

/**
 * \brief Make a double cell.
 * \param d The initial double value to be stored in the new cell.
 */
inline Cell* make_double(const double d)
{
  return (Cell*) new DoubleCell(d);
}

/**
 * \brief Make a symbol cell.
 * \param s The initial symbol name to be stored in the new cell.
 */
inline Cell* make_symbol(const char* const s)
{
  return (Cell*) new SymbolCell(s);
}

/**
 * \brief Make a conspair cell.
 * \param my_car The initial car pointer to be stored in the new cell.
 * \param my_cdr The initial cdr pointer to be stored in the new cell.
 */
inline Cell* cons(Cell* const my_car, Cell* const my_cdr)
{
  return (Cell*) new ConsCell(my_car, my_cdr);
}

/**
 * \brief Make an operator cell.
 * \param s The initial operator name to be stored in the new cell.
 */
inline Cell* make_operator(const char* const s)
{
  return (Cell*) new OperatorCell(s);
}

/**
 * \brief Check if c points to an empty list, i.e., is a null pointer.
 * \return True iff c points to an empty list, i.e., is a null pointer.
 */
inline bool nullp(Cell* const c)
{
  return (c == nil);
}

/**
 * \brief Check if c points to a list (i.e., nil or a cons cell).
 * \return True iff c points to a list (i.e., nil or a cons cell).
 */
inline bool listp(Cell* const c)
{
  return nullp(c) || c->is_cons();
}

/**
 * \brief Check if c points to an int cell.
 * \return True iff c points to an int cell.
 */
inline bool intp(Cell* const c)
{
  return !nullp(c) && c->is_int();
}

/**
 * \brief Check if c points to a double cell.
 * \return True iff c points to a double cell.
 */
inline bool doublep(Cell* const c)
{
  return !nullp(c) && c->is_double();
}

/**
 * \brief Check if c points to a symbol cell.
 * \return True iff c points to a symbol cell.
 */
inline bool symbolp(Cell* const c)
{
  return !nullp(c) && c->is_symbol();
}

/**
 * \brief Check if c points to an operator cell.
 * \return True iff c points to an operator cell.
 */
inline bool operatorp(Cell* const c)
{
  return !nullp(c) && c->is_operator();
}

/**
 * \brief Check if c is a non-zero valued cell.
 * \return True iff c is a non-zero valued cell.
 */
inline bool nonzerop(Cell* const c)
{
  return c->is_nonzerovalue();
}

/**
 * \brief Accessor (error if c is not an int cell).
 * \return The value in the int cell pointed to by c.
 */
inline int get_int(Cell* const c)
{
  return c->get_int();
}

/**
 * \brief Accessor (error if c is not a double cell).
 * \return The value in the double cell pointed to by c.
 */
inline double get_double(Cell* const c)
{
  return c->get_double();
}

/**
 * \brief Retrieve the symbol name as a string (error if c is not a
 * symbol cell).
 * \return The symbol name in the symbol cell pointed to by c.
 */
inline std::string get_symbol(Cell* const c)
{
  return c->get_symbol();
}

/**
 * \brief Accessor (error if c is not a cons cell).
 * \return The car pointer in the cons cell pointed to by c.
 */
inline Cell* car(Cell* const c)
{
  return c->get_car();
}

/**
 * \brief Accessor (error if c is not a string cell).
 * \return The cdr pointer in the cons cell pointed to by c.
 */
inline Cell* cdr(Cell* const c)
{
  return c->get_cdr();
}

/**
 * \brief Accessor (error if c is not a string cell).
 * \return Double type value from either an Int, Double or Symbol cell.
 */
inline double get_value(Cell* const c)
{
  return c->get_value();
}

/**
 * \brief Gets the operation type from a string
 * \param opr string Operator name
 * \return Operation enum value. 
 */
inline Operation get_operation(std::string const opr)
{
  if (opr == "+") {
    return add_opr;
  } else if (opr == "if") {
    return if_opr;
  } else if (opr == "ceiling") {
    return ceil_opr;
  } else if (opr == "-") {
    return minus_opr;
  } else if (opr == "*") {
    return multiply_opr;
  } else if (opr == "/") {
    return divide_opr;
  } else if (opr == "floor") {
    return floor_opr;
  } else if (opr == "quote") {
    return quote_opr;
  } else if (opr == "cons") {
    return cons_opr;
  } else if (opr == "car") {
    return car_opr;
  } else if (opr == "cdr") {
    return cdr_opr;
  } else if (opr == "nullp") {
    return nullp_opr;
  } else {
    return undefined_opr;
  }
}


/**
 * \brief Print the subtree rooted at c, in s-expression notation.
 * \param os The output stream to print to.
 * \param c The root cell of the subtree to be printed.
 */
inline std::ostream& operator<<(std::ostream& os, const Cell& c)
{
  c.print(os);
  return os;
}

/**
 * \brief Gets the size of a given list.
 * \param c The head of the list.
 * \return The size of the list.
 */
inline int list_size(Cell* const c)
{
  int size = 0;
  Cell* curr = c;

  while(!nullp(curr)) {
    ++size;
    if (!listp(curr)) {
      return size;
    } else {
      curr = cdr(curr);
    }
  }

  return size;
}

#endif // CONS_HPP
