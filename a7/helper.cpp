/**
 * \file helper.hpp
 * 
 * Helper functions to handle Assertions and Errors.
 */

#include "helper.hpp"
#include "cons.hpp"

string append_error(const string& e_what, const string& where_arg)
{
  string what = e_what + "\n" + "\t : at " + where_arg;
  return what;
}

void throw_error(const string& what_arg, const string& where_arg)
{
  if (where_arg == "") {
    throw runtime_error(what_arg);
  } else {
    throw runtime_error(append_error(what_arg, where_arg));
  }
}

void assert_cellnotnull(const string& what_arg, Cell* const c)
{
  if (nullp(c)) {
    throw_error(what_arg);
  }
}

void assert_isdoubleintcell(const string& what_arg, Cell* const c)
{
  try {
    get_value(c);
  } catch (runtime_error) {
    throw_error(what_arg);
  }
}

void assert_listsize(const string& what_arg, Cell* const c, int lower_limit, int upper_limit)
{
  int listsize = list_size(c);
  if (listsize < lower_limit || listsize > upper_limit) {
    throw_error(what_arg);
  }
}

void assert_listsize(const string& what_arg, Cell* const c, int size)
{
  int listsize = list_size(c);
  if (listsize != size) {
    throw_error(what_arg);
  }
}

void assert_isnonzerovalue(const string& what_arg, Cell* const c)
{
  if (get_value(c) == 0) {
    throw_error(what_arg);
  }
}
