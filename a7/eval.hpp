/**
 * \file eval.hpp
 *
 * Encapsulates the interface for the expression evaluator function.
 */

#ifndef EVAL_HPP
#define EVAL_HPP

#include "cons.hpp"
#include <math.h>
#include <string>
#include <stdexcept>
#include <exception>
#include <vector>
#include "hashtablemap.hpp"

using namespace std;

typedef hashtablemap<string, Cell*> hashmap;

/**
 * \brief The global stack frame vector container holding defined maps.
 */
extern vector< hashmap > stack_frame;

/**
 * \brief Evaluate the expression tree whose root is pointed to by c
 * (error if c does not hold a well-formed expression).
 *
 * \return The value resulting from evaluating the expression.
 */
Cell* eval(Cell* const c);

/**
 * \brief Evaluate the sub-expression tree whose root is pointed to by c
 * (error if c does not hold a well-formed expression).
 *
 * \param funct is a function pointer passed for specific 
 * arithmetic operations, i.e. add, minus, multiply, divide.
 * \return The value resulting from evaluating the expression.
 */

Cell* arithmetic_eval(Cell* const c, Cell* (*funct)(Cell*,Cell*));

/**
 * \brief Evaluate the sub-expression tree whose root is pointed to by c
 * (error if c does not hold a well-formed expression).
 *
 * \param value The value of the first cell to operate on.
 * \param next_value The value of the next cell to operate on.
 * \return The value resulting from evaluating the sub-expression.
 */
Cell* add_cells(Cell* const value, Cell* const next_value);

/**
 * \brief Evaluate the sub-expression tree whose root is pointed to by c
 * (error if c does not hold a well-formed expression).
 *
 * \param value The value of the first cell to operate on.
 * \param next_value The value of the next cell to operate on.
 * \return The value resulting from evaluating the sub-expression.
 */
Cell* minus_cells(Cell* const value, Cell* const next_value);

/**
 * \brief Evaluate the sub-expression tree whose root is pointed to by c
 * (error if c does not hold a well-formed expression).
 *
 * \param value The value of the first cell to operate on.
 * \param next_value The value of the next cell to operate on.
 * \return The value resulting from evaluating the sub-expression.
 */
Cell* multiply_cells(Cell* const value, Cell* const next_value);

/**
 * \brief Evaluate the sub-expression tree whose root is pointed to by c
 * (error if c does not hold a well-formed expression).
 *
 * \param value The value of the first cell to operate on.
 * \param next_value The value of the next cell to operate on.
 * \return The value resulting from evaluating the sub-expression.
 */
Cell* divide_cells(Cell* const value, Cell* const next_value);

/**
 * \brief Evaluate the sub-expression tree whose root is pointed to by c
 * (error if c does not hold a well-formed expression).
 *
 * \return The value resulting from evaluating the sub-expression.
 */
Cell* if_eval(Cell* const c);

/**
 * \brief Evaluate the sub-expression tree whose root is pointed to by c
 * (error if c does not hold a well-formed expression).
 *
 * \return The value resulting from evaluating the sub-expression.
 */
Cell* ceil_eval(Cell* const c);

/**
 * \brief Evaluate the sub-expression tree whose root is pointed to by c
 * (error if c does not hold a well-formed expression).
 *
 * \return The value resulting from evaluating the sub-expression.
 */
Cell* floor_eval(Cell* const c);

/**
 * \brief Evaluate the sub-expression tree whose root is pointed to by c
 * (error if c does not hold a well-formed expression).
 *
 * \return The value resulting from evaluating the sub-expression.
 */
Cell* quote_eval(Cell* const c);

/**
 * \brief Evaluate the sub-expression tree whose root is pointed to by c
 * (error if c does not hold a well-formed expression).
 *
 * \return The value resulting from evaluating the sub-expression.
 */
Cell* cons_eval(Cell* const c);

/**
 * \brief Evaluate the sub-expression tree whose root is pointed to by c
 * (error if c does not hold a well-formed expression).
 *
 * \return The value resulting from evaluating the sub-expression.
 */
Cell* car_eval(Cell* const c);

/**
 * \brief Evaluate the sub-expression tree whose root is pointed to by c
 * (error if c does not hold a well-formed expression).
 *
 * \return The value resulting from evaluating the sub-expression.
 */
Cell* cdr_eval(Cell* const c);

/**
 * \brief Evaluate the sub-expression tree whose root is pointed to by c
 * (error if c does not hold a well-formed expression).
 *
 * \return The value resulting from evaluating the sub-expression.
 */
Cell* nullp_eval(Cell* const c);

/**
 * \brief Evaluate the sub-expression tree whose root is pointed to by c
 * (error if c does not hold a well-formed expression).
 *
 * \return The value resulting from evaluating the sub-expression.
 */
Cell* define_eval(Cell* const c);

/**
 * \brief Evaluate the sub-expression tree whose root is pointed to by c
 * (error if c does not hold a well-formed expression).
 *
 * \return The value resulting from evaluating the sub-expression.
 */
Cell* lessthan_eval(Cell* const c);

/**
 * \brief Evaluate the sub-expression tree whose root is pointed to by c
 * (error if c does not hold a well-formed expression).
 *
 * \return The value resulting from evaluating the sub-expression.
 */
Cell* not_eval(Cell* const c);

/**
 * \brief Evaluate the sub-expression tree whose root is pointed to by c
 * (error if c does not hold a well-formed expression).
 *
 * \return The value resulting from evaluating the sub-expression.
 */
Cell* print_eval(Cell* const c);

/**
 * \brief Evaluate the sub-expression tree whose root is pointed to by c
 * (error if c does not hold a well-formed expression).
 *
 * \return The value resulting from evaluating the sub-expression.
 */
Cell* eval_eval(Cell* const c);

/**
 * \brief Evaluate the sub-expression tree whose root is pointed to by c
 * (error if c does not hold a well-formed expression).
 *
 * \return The value resulting from evaluating the sub-expression.
 */
Cell* lambda_eval(Cell* const c);

/**
 * \brief Evaluate the sub-expression tree whose root is pointed to by c
 * (error if c does not hold a well-formed expression).
 *
 * \return The value resulting from evaluating the sub-expression.
 */
Cell* apply_eval(Cell* const c);

/**
 * \brief Evaluate the sub-expression tree whose root is pointed to by c
 * (error if c does not hold a well-formed expression).
 *
 * \return The value resulting from evaluating the sub-expression.
 */
Cell* let_eval(Cell* const c);

/**
 * \brief Evaluate the sub-expression tree whose root is pointed to by c
 * (error if c does not hold a well-formed expression).
 *
 * \return The value resulting from evaluating the sub-expression.
 */
Cell* intp_eval(Cell* const c);

/**
 * \brief Evaluate the sub-expression tree whose root is pointed to by c
 * (error if c does not hold a well-formed expression).
 *
 * \return The value resulting from evaluating the sub-expression.
 */
Cell* doublep_eval(Cell* const c);

/**
 * \brief Evaluate the sub-expression tree whose root is pointed to by c
 * (error if c does not hold a well-formed expression).
 *
 * \return The value resulting from evaluating the sub-expression.
 */
Cell* symbolp_eval(Cell* const c);

/**
 * \brief Evaluate the sub-expression tree whose root is pointed to by c
 * (error if c does not hold a well-formed expression).
 *
 * \return The value resulting from evaluating the sub-expression.
 */
Cell* listp_eval(Cell* const c);


#endif // EVAL_HPP
