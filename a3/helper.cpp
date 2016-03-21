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

void throw_error( const string& what_arg, const string& where_arg)
{
  throw runtime_error(append_error(what_arg, where_arg));
}

void assert_cellnotnull(const string& where_arg, const string& what_arg, Cell* const  c)
{
  if (nullp(c)) {
    throw_error(where_arg, what_arg);
  }
}

void assert_isdoubleintcell(const string& where_arg, const string& what_arg, Cell* const  c)
{
  try {
    get_value(c);
  } catch (runtime_error) {
    throw_error(where_arg, what_arg);
  }
}

void assert_listsize(const string& where_arg, const string& what_arg, Cell* const  c, int lower_limit, int upper_limit)
{
  int listsize = list_size(c);
  if (listsize < lower_limit || listsize > upper_limit) {
    throw_error(where_arg, what_arg);
  }
}

void assert_listsize(const string& where_arg, const string& what_arg, Cell* const  c, int size)
{
  int listsize = list_size(c);
  if (listsize != size) {
    throw_error(where_arg, what_arg);
  }
}

void assert_isnonzerovalue(const string& where_arg, const string& what_arg, Cell* const  c)
{
  if (get_value(c) == 0) {
    throw_error(where_arg, what_arg);
  }
}
