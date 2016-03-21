/**
 * \file helper.hpp
 * 
 * Helper functions to handle Assertions and Errors.
 */

#ifndef HELPER_HPP
#define HELPER_HPP

#include <string>

using namespace std;

class Cell;

/**
 * \brief Appends the where_arg to an exception's e_what string.
 * \param e_what Exception.what()
 * \param where_arg The current trace_prefix for that function call.
 */
string append_error(const string& e_what, const string& where_arg);

/**
 * \brief Throws a formatted runtime_error in: 
 * "(what_arg) 
 : at (where_arg);" format.
 * \param where_arg The current trace_prefix for that function call.
 * \param what_arg The specific error message from the causing error.
 */
void throw_error(const string& where_arg, const string& what_arg);

/**
 * \brief Asserts that the cell is not null. 
 * Throws a runtime_error exception if cell is null;
 * \param where_arg The current trace_prefix for that function call.
 * \param what_arg The specific error message from the causing error.
 * \param c The cell required for assertion.
 */
void assert_cellnotnull(const string& where_arg, const string& what_arg, Cell* const  c);

/**
 * \brief Asserts that the cell is a double or int cell.
 * Throws a runtime_error exception if cell is not a double or int cell.
 * \param where_arg The current trace_prefix for that function call.
 * \param what_arg The specific error message from the causing error.
 * \param c The cell required for assertion.
 */
void assert_isdoubleintcell(const string& where_arg, const string& what_arg, Cell* const  c);

/**
 * \brief Asserts that the list's size is within the lower limit and upper limit.
 * Throws a runtime_error exception if cell is not within size limits.
 * \param where_arg The current trace_prefix for that function call.
 * \param what_arg The specific error message from the causing error.
 * \param c The cell pointing to a list required for assertion.
 * \param lower_limit The lower limit range for the list.
 * \param upper_limit The upper limit range for the list.
 */
void assert_listsize(const string& where_arg, const string& what_arg, Cell* const  c, int lower_limit, int upper_limit);

/**
 * \brief Asserts that the list's size is equal to the specified size.
 * Throws a runtime_error exception if cell is of the specified size.
 * \param where_arg The current trace_prefix for that function call.
 * \param what_arg The specific error message from the causing error.
 * \param c The cell pointing to a list required for assertion.
 * \param size The required size for the given list.
 */
void assert_listsize(const string& where_arg, const string& what_arg, Cell* const  c, int size);

/**
 * \brief Asserts that the cell is of non-zero value.
 * Throws a runtime_error exception is cell holds a zero value.
 * \param where_arg The current trace_prefix for that function call.
 * \param what_arg The specific error message from the causing error.
 * \param c The Cell required for assertion.
 */
void assert_isnonzerovalue(const string& where_arg, const string& what_arg, Cell* const  c);


#endif // HELPER_HPP
