/**
 * \mainpage COMP2012H Programming Assignment 4, Fall 2014
 *
 * \author Berago Joel Arvin
 * \author 20211631
 * \author jamberago@connect.ust.hk
 * \author LA1
 *
 * \date 2014.11.10
 *
 * Instructor: <a href="http://www.cs.ust.hk/~dekai/">Dekai Wu</a>
 * Due: 2014.11.13 at 23:00 by CASS
 */

/**
 * \file Cell.hpp
 *
 * Encapsulates the abstract interface for a concrete class-based
 * implementation of cells for a cons list data structure.
 */

#ifndef CELL_HPP
#define CELL_HPP

#include <cstddef>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <stack>

#include <iomanip>
#include <stdexcept>
#include <vector>
#include <map>

using namespace std;

/**
 * \class Cell
 * \brief Class Cell
 */
class Cell
{
public:

  /**
   * \brief Virtual Destructor for specific memory management.
   */
  virtual ~Cell();

  /**
   * \brief Check if this is an int cell.
   * \return True iff this is an int cell.
   */
  virtual bool is_int() const;

  /**
   * \brief Check if this is a double cell.
   * \return True iff this is a double cell.
   */
  virtual bool is_double() const;

  /**
   * \brief Check if this is a symbol cell.
   * \return True iff this is a symbol cell.
   */
  virtual bool is_symbol() const;

  /**
   * \brief Check if this is a cons cell.
   * \return True iff this is a cons cell.
   */
  virtual bool is_cons() const;

  /**
   * \brief Check if this is an operation cell.
   * \return True iff this is an operation cell.
   */
  virtual bool is_operator() const;

  /**
   * \brief Check if this is an procedure cell.
   * \return True iff this is an procedure cell.
   */
  virtual bool is_procedure() const;

  /**
   * \brief Check if this cell holds a non-zero value.
   * \return True iff this cell holds a non-zero value.
   */
  virtual bool is_nonzerovalue() const;

  /**
   * \brief Accessor (error if this is not an int cell).
   * \return The value in this int cell.
   */
  virtual int get_int() const;

  /**
   * \brief Accessor (error if this is not a double cell).
   * \return The value in this double cell.
   */
  virtual double get_double() const;

  /**
   * \brief Accessor (error if this is not a symbol cell).
   * \return The symbol name in this symbol cell.
   */
  virtual char* get_symbol() const;

  /**
   * \brief Accessor (error if this is not a cons cell).
   * \return First child cell.
   */
  virtual Cell* get_car() const; 

  /**
   * \brief Accessor (error if this is not a cons cell).
   * \return Rest child cell.
   */
  virtual Cell* get_cdr() const;

  /**
   * \brief Accessor (error if this is not an Int, Double or Symbol cell.
   * \return Double type value of the cell.
   */
  virtual double get_value() const;

  /**
   * \brief Accessor (error if c is not a procedure cell).
   * \return Pointer to the cons list of formal parameters for the function
   * pointed to by c.
   */
  virtual Cell* get_formals() const;

  /**
   * \brief Accessor (error if c is not a procedure cell).
   * \return Pointer to the cons list containing the expression defining the
   * body for the function pointed to by c.
   */
  virtual Cell* get_body() const;

  /**
   * \brief Print the subtree rooted at this cell, in s-expression notation.
   * \param os The output stream to print to.
   */
  virtual void print(ostream& os = cout) const = 0;
  /**
   * \brief Makes a new copy of the cell.
   * \return A new copy of the cell.
   */
  virtual Cell* clone() const = 0;
  /**
   * \brief Evaluates the Cell.
   * \return The result from evaluation.
   */
  virtual Cell* eval() const = 0;
  /**
   * \brief Evaluates the Cell with given arguments
   * \param args Holds the arguments to pass for evaluation.
   * \return The result from evaluation.
   */
  virtual Cell* eval(Cell* const args) const;
  /**
   * \brief Applies the function in a ProcedureCell.
   * \param args A list of values to supply the function to apply.
   * \return The result from applying the function.
   */
  virtual Cell* apply(Cell* const args) const;
};

/**
 * \class IntCell
 * \brief Class IntCell
 */
class IntCell : public Cell
{
public:
  /**
   * \brief Constructor to make IntCell
   * \param i holds the int value.
   */
  IntCell(const int i);

  virtual bool is_int() const;
  virtual bool is_nonzerovalue() const;
  virtual int get_int() const;
  virtual double get_value() const;
  virtual void print(ostream& os = cout) const;
  virtual Cell* clone() const;
  virtual Cell* eval() const;
private:
  int int_m;
};

/**
 * \class DoubleCell
 * \brief Class DoubleCell
 */
class DoubleCell : public Cell
{
public:
  /**
   * \brief Constructor to make DoubleCell
   * \param d is double type to store at double_m
   */
  DoubleCell(const double d);

  virtual bool is_double() const;
  virtual bool is_nonzerovalue() const;
  virtual double get_double() const;
  virtual double get_value() const;
  virtual void print(ostream& os = cout) const;
  virtual Cell* clone() const;
  virtual Cell* eval() const;
private:
  double double_m;
};

/**
 * \enum Operation
 * \brief enum Operation lists all available operations
 */
enum Operation {
  undefined_opr = 0,
  add_opr,
  if_opr,
  ceil_opr,
  minus_opr,
  multiply_opr,
  divide_opr,
  floor_opr,
  quote_opr,
  cons_opr,
  car_opr,
  cdr_opr,
  nullp_opr,
  define_opr,
  lessthan_opr,
  not_opr,
  print_opr,
  eval_opr,
  lambda_opr,
  apply_opr,
  let_opr
};

/**
 * \class SymbolCell
 * \brief Class SymbolCell
 */
class SymbolCell : public Cell
{
public:

  /**
   * \brief Constructor to make a SymbolCell
   * \param s holds a char pointer to a string.
   */
  SymbolCell(const char* const s);
  virtual ~SymbolCell();
  virtual bool is_symbol() const;
  virtual bool is_nonzerovalue() const;
  virtual char* get_symbol() const;
  virtual void print(ostream& os = cout) const;
  virtual Cell* clone() const;
  virtual Cell* eval() const;
private:
  char* symbol_m;
};

/**
 * \class OperatorCell
 * \brief Class OperatorCell
 */
class OperatorCell : public SymbolCell
{
public:
  /**
   * \brief Constructor to make an OperatorCell
   * \param s char* passed to derived SymbolCell(s)
   */
  OperatorCell(const char* const s);

  virtual bool is_operator() const;
  virtual bool is_symbol() const;
  virtual Cell* eval() const;
  virtual Cell* eval(Cell* const args) const;
  virtual Cell* apply(Cell* const args) const;
  virtual void print(ostream& os = cout) const;
};

/**
 * \class ConsCell
 * \brief Class ConsCell
 */
class ConsCell : public Cell
{
public:
  /**
   * \brief Constructor to make ConsCell
   * \param my_car Cell pointer to store car
   * \param my_cdr Cell pointer to store cdr
   */
  ConsCell(Cell* const my_car, Cell* const my_cdr);
  virtual ~ConsCell();

  virtual bool is_cons() const;
  virtual Cell* get_car() const;
  virtual Cell* get_cdr() const;
  virtual void print(ostream& os = cout) const;
  virtual Cell* clone() const;
  virtual Cell* eval() const;
private:
  Cell* car_m;
  Cell* cdr_m;
};

/**
 * \class ProcedureCell
 * \brief Class ProcedureCell
 */
class ProcedureCell : public Cell
{
public:
  /**
   * \brief Constructor to make ProcedureCell
   * \param my_formals Cell pointer to store formals
   * \param my_body Cell pointer to store body
   */
  ProcedureCell(Cell* const my_formals, Cell* const my_body);
  virtual ~ProcedureCell();

  virtual bool is_procedure() const;
  virtual Cell* get_formals() const;
  virtual Cell* get_body() const;
  virtual void print(ostream& os = cout) const;
  virtual Cell* clone() const;
  virtual Cell* eval() const;
  virtual Cell* eval(Cell* const args) const;
  virtual Cell* apply(Cell* const args) const;
private:
  Cell* formals_m;
  Cell* body_m;
};

extern Cell* const nil;
extern vector< map<string, Cell*> > stack_frame;

#endif
