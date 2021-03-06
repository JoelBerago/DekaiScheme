/**
 * \file eval.hpp
 *
 * Encapsulates the interface for the expression evaluator function.
 */

#ifndef EVAL_HPP
#define EVAL_HPP

#include "cons.hpp"

using namespace std;

/**
 * \brief Evaluate the expression tree whose root is pointed to by c
 * (error if c does not hold a well-formed expression).
 *
 * \return The value resulting from evaluating the expression.
 */
Cell* eval(Cell* const c);

/**
 * \brief Evaluate the inner sub-expression tree whose root is pointed to by c
 * (error if c does not hold a well-formed expression).
 *
 * \return The value resulting from evaluating the sub-expression.
 */
Cell* inner_eval(Cell* const c);

/**
 * \brief Evaluate the add sub-expression tree whose root is pointed to by c
 * (error if c does not hold a well-formed expression).
 *
 * \return The value resulting from evaluating the sub-expression.
 */
Cell* add_eval(Cell* const c);

/**
 * \brief Evaluate the if sub-expression tree whose root is pointed to by c
 * (error if c does not hold a well-formed expression).
 *
 * \return The value resulting from evaluating the sub-expression.
 */
Cell* if_eval(Cell* const c);

/**
 * \brief Evaluate the ceiling sub-expression tree whose root is pointed to by c
 * (error if c does not hold a well-formed expression).
 *
 * \return The value resulting from evaluating the sub-expression.
 */
Cell* ceil_eval(Cell* const c);

#endif // EVAL_HPP
