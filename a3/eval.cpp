/**
 * \file eval.cpp
 *
 * Evaluates s-expressions from parse.cpp
 */

#include "eval.hpp"

using namespace std;

std::map<string, Cell*> defined_map;

Cell* eval(Cell* const c)
{
  string trace_prefix = "eval.cpp::eval(Cell*)";

  Cell *operation, *value;
  operation = value = nil;
  try {
    // if s-expression is just a string to a defined mapped cell.
    if (symbolp(c)) {
      return get_defined((string)get_symbol(c));
    } else if (listp(c)) {
      // Loop to ensure that first-most cell is an operator cell.
      operation = car(c);

      while (!nullp(operation) && !intp(operation) && !doublep(operation)) {
	if (listp(operation)) {
	  operation = car(operation);
	} else if (symbolp(operation)) {
	  if (get_operation(get_symbol(operation)) != undefined_opr) {
	    break;
	  } else {
	    throw_error("Not an OperatorCell", trace_prefix);
	  }
	}
      }

      if (nullp(operation) || intp(operation) || doublep(operation)) {
	throw_error("First most cell must be an operator cell." , 
		    trace_prefix);
      }
    }

    return cell_eval(c);
  } catch (runtime_error& e) {
    throw_error(e.what(), trace_prefix); 
  }
}


Cell* arithmetic_eval(Cell* const c, Cell* (*funct)(Cell*,Cell*))
{
  string trace_prefix = "eval.hpp::arithmetic_eval(Cell* const c, Cell* (*funct)(Cell*,Cell*))";
  // Add and Multiply with no arguments returns 1.
  if (nullp(c) && (funct == add_cells || funct == multiply_cells)) {
    return make_int(1);
  }

  Cell *value, *next, *next_value;
  value = next = next_value = nil;

  try { 
    value = car(c);
    next = cdr(c);
    next_value = nil;

    value = cell_eval(value);		

    // cannot divide by zero.
    if (funct == divide_cells) {
      assert_isnonzerovalue(trace_prefix, "Tried to divide by zero", value);
    }

    if (nullp(next)) {
      if (funct == minus_cells) {       
	// If one operand, returns negative of operand.
	if (doublep(value)) {
	  return make_double(get_double(value) * -1);
	} else {
	  return make_int(get_int(value) * -1);
	}
      } else if (funct == divide_cells) {
	// If only one operand, give inverse.
	return make_double(1.0 / get_value(value));        
      }
    }  

    // Forward loop through list and adds next_value to value until the end.
    //   Uses the given Function to add, minus, multiply or divide.
    while (!nullp(next)) {
      next_value = cell_eval(car(next));
      next = cdr(next);

      // cannot divide by zero.
      if (funct == divide_cells) {
	assert_isnonzerovalue(trace_prefix, "Tried to divide by zero", next_value);
      }

      value = funct(value, next_value);
    }

    // Ensure final value is an Int or DoubleCell then return.
    assert_isdoubleintcell(trace_prefix, "Expected IntCell or DoubleCell", value);
    return value;
  } catch (runtime_error& e) {    
    throw_error(e.what(), trace_prefix);
  }
}

Cell* add_cells(Cell* const value, Cell* const next_value)
{
  string trace_prefix = "eval.cpp::add_cells(Cell*, Cell*)";
  try {
    if (doublep(value) || doublep(next_value)) {
      return make_double(get_value(value) + get_value(next_value));
    } else {
      return make_int((int)(get_value(value) + get_value(next_value)));
    }
  } catch (runtime_error& e) {
    throw_error(e.what(), trace_prefix);
  }
}

Cell* minus_cells(Cell* const value, Cell* const next_value)
{
  string trace_prefix = "eval.cpp::minus_cells(Cell*, Cell*)";
  try {
    if (doublep(value) || doublep(next_value)) {
      return make_double(get_value(value) - get_value(next_value));
    } else {
      return make_int((int)(get_value(value) - get_value(next_value)));
    }
  } catch (runtime_error& e) {
    throw_error(e.what(), trace_prefix);
  }
}

Cell* multiply_cells(Cell* const value, Cell* const next_value)
{
  string trace_prefix = "eval.cpp::multiply_cells(Cell*, Cell*)";
  try {
    if (doublep(value) || doublep(next_value)) {
      return make_double(get_value(value) * get_value(next_value));
    } else {
      return make_int((int)(get_value(value) * get_value(next_value)));
    }
  } catch (runtime_error& e) {
    throw_error(e.what(), trace_prefix);
  }
}

Cell* divide_cells(Cell* const value, Cell* const next_value)
{
  string trace_prefix = "eval.cpp::divide_cells(Cell*, Cell*)";
  try {
    if (doublep(value) || doublep(next_value)) {
      return make_double(get_value(value) / get_value(next_value));
    } else {
      return make_int((int)(get_value(value) / get_value(next_value)));
    }
  } catch (runtime_error& e) {
    throw_error(e.what(), trace_prefix);
  }
}

Cell* if_eval(Cell* const c)
{
  string trace_prefix = "eval.cpp::if_eval(Cell*)";

  Cell *test, *results;
  test = results = nil;

  try {
    assert_listsize(trace_prefix, "Expected 2 or 3 operands", c, 2, 3);

    test = car(c);
    results = cdr(c);
    int size = list_size(c);

    test = cell_eval(test);

    // Result handling.
    if (nonzerop(test)) {
      // if test is true, i.e. nonzero
      results = cell_eval(car(results));
    } else {
      // if test is false, i.e. zero
      assert_listsize(trace_prefix, "False case is missing", c, 3);
      results = cell_eval(car(cdr(results)));
    }

    return results;
  } catch (runtime_error& e) {
    throw_error(e.what(), trace_prefix);
  }
}

Cell* ceil_eval(Cell* const c)
{
  string trace_prefix = "eval.cpp::ceil_eval(Cell*)";

  Cell *value;
  value = nil;
  try {
    assert_listsize(trace_prefix, "Expected only one operand", c, 1);

    value = car(c);

    value = cell_eval(value);

    return make_int((int)ceil(get_double(value)));
  } catch (runtime_error& e) {
    throw_error(e.what(), trace_prefix);
  }
}

Cell* floor_eval(Cell* const c)
{
  string trace_prefix = "eval.cpp::floor_eval(Cell*)";
  
  Cell *value;
  value = nil;
  try {
    assert_listsize(trace_prefix, "Expected only one operand", c, 1);

    value = car(c);  

    value = cell_eval(value);

    return make_int((int)floor(get_value(value)));
  } catch (runtime_error& e) {
    throw_error(e.what(), trace_prefix);
  }
}

Cell* quote_eval(Cell* const c)
{
  string trace_prefix = "eval.cpp::quote_eval(Cell*)";

  Cell *my_car;
  my_car = nil;
  try {
    assert_listsize(trace_prefix, "Expected only one operand", c, 1);

    my_car = car(c);

    // (quote ())
    if (nullp(my_car)) {
      return nil;
    }

    return clone_cell(my_car);
  } catch (runtime_error& e) {
    throw_error(e.what(), trace_prefix);
  }
}

Cell* cons_eval(Cell* const c)
{
  string trace_prefix = "eval.cpp::cons_eval(Cell*)";

  Cell *my_car, *my_cdr;
  my_car = my_cdr = nil;
  try {
    assert_cellnotnull(trace_prefix, "Empty Operands : c", c);

    Cell* my_car = car(c);
    Cell* my_cdr = cdr(c);

    my_car = cell_eval(my_car);
    my_cdr = cell_eval(my_cdr);

    if (emptylistp((ConsCell*)my_cdr)) {
      my_cdr = NULL;
    }

    return cons(my_car, my_cdr);
  } catch (runtime_error& e) {
    throw_error(e.what(), trace_prefix);
  }
}

Cell* car_eval(Cell* const c)
{
  string trace_prefix = "eval.cpp::car_eval(Cell*)";

  Cell *my_car, *result;
  my_car = result = nil;
  try {
    assert_listsize(trace_prefix, "Expected only one operand", c, 1);

    Cell* my_car = car(c);
    Cell* result = nil;

    my_car = cell_eval(my_car);

    // result in car.
    result = car(my_car);

    return result;
  } catch (runtime_error& e) {
    throw_error(e.what(), trace_prefix);
  }
}

Cell* cdr_eval(Cell* const c)
{
  string trace_prefix = "eval.cpp::cdr_eval(Cell*)";
  
  Cell *my_car, *result;
  my_car = result = nil;
  try {
    assert_listsize(std::string(trace_prefix), std::string("Expected only one operand"), c, 1);

    Cell* my_car = car(c);
    Cell* result = nil;

    // First operand is still car();
    my_car = cell_eval(my_car);

    // result is in cdr.
    result = cdr(my_car);

    return result;
  } catch (runtime_error& e) {
    throw_error(e.what(), trace_prefix);
  }
}

Cell* nullp_eval(Cell* const c)
{
  string trace_prefix = "eval.cpp::nullp_eval(Cell*)";

  Cell* result;
  result = nil;
  try {
    assert_listsize(trace_prefix, "Expected only one operand", c, 1);

    result = car(c);

    result = cell_eval(result);

    // If Empty Operand, returns 1 then else return 0
    if (listp(result) && emptylistp((ConsCell*)result)) {
      return make_int(1);
    } else if (nullp(result)) {
      return make_int(1);
    } else {
      return make_int(0);
    }
  } catch (runtime_error& e) {
    throw_error(e.what(), trace_prefix);
  }
}

Cell* define_eval(Cell* const c)
{
  string trace_prefix = "eval.cpp::define_eval(Cell*)";

  Cell *key_c, *value;
  key_c = value = nil;
  try {
    assert_listsize(trace_prefix, "Expected only two operands", c, 2);
    key_c = car(c);
    value = cdr(c);

    value = cell_eval(car(value));

    string key = get_symbol(key_c);
    if (get_operation(key) != undefined_opr) {
      throw_error("Cannot define with a reserved keyword \"" + key + "\"",
		  trace_prefix);
    }

    // Uses map::insert(std::pair<Key, T>) which returns a pair<iterator, bool>
    //   pair.second is bool and stores if insert was successful.
    //   if pair.second is false, it means there has been an element defined on that key. 
    pair<map<string, Cell*>::iterator, bool> itbool_pair;
    itbool_pair = defined_map.insert(pair<string, Cell*>(key, value));
    if (itbool_pair.second == false) {
      throw_error("Cannot redefine a mapped definition \"" + key + "\"",
		  trace_prefix);
    }

    return nil;
  } catch (runtime_error& e) {
    throw_error(e.what(), trace_prefix);
  }
}

Cell* lessthan_eval(Cell* const c)
{
  string trace_prefix = "eval.cpp::lessthan_eval(Cell*)";

  if (nullp(c)) {
    return make_int(1);
  }

  Cell *value, *next, *next_value;
  value = next = next_value = nil;

  try {
    value = car(c);
    next = cdr(c);
    next_value = nil;

    value = cell_eval(value);
    while (!nullp(next)) {
      next_value = cell_eval(car(next));
      next = cdr(next);
      assert_isdoubleintcell(trace_prefix, "Expected an Int or Double cell", next_value);

      // Short Circuit evaluation, returns immediately when false.
      if (get_value(value) >= get_value(next_value)) {
        return make_int(0);
      }
      value = next_value;
    }

    assert_isdoubleintcell(trace_prefix, "Expected an Int or Double cell", value);
    return make_int(1);
  } catch (runtime_error& e) {
    throw_error(e.what(), trace_prefix);
  }
}

Cell* not_eval(Cell* const c)
{
  string trace_prefix = "eval.cpp::not_eval(Cell*)";

  Cell *value;
  value = nil;
  try {
    assert_listsize(trace_prefix, "Expected only one operand", c, 1);

    value = car(c);
 
    value = cell_eval(value);

    // try/catch block to catch for non-int/double types.
    //   returns assumed 0 for non-value types.
    try {
      if (get_value(value) == 0) {
	return make_int(1);
      } else {
	return make_int(0);
      }
    } catch (runtime_error&) {
      // error'd at get_value.
      return make_int(0);
    }
  } catch (runtime_error& e) {
    throw_error(e.what(), trace_prefix);
  }
}

Cell* print_eval(Cell* const c)
{
  string trace_prefix = "eval.cpp::print_eval(Cell*)";
  
  Cell *value;
  value = nil;
  try {
    assert_listsize(trace_prefix, "Expected only one operand", c, 1);

    value = car(c);
    value = cell_eval(value);

    // checks for (print ())
    if (!nullp(value)) {
      cout << *value << endl;
    } else {
      cout << "()" << endl;
    }
    
    return nil;
  } catch (runtime_error& e) {
    throw_error(e.what(), trace_prefix);
  }
}

Cell* eval_eval(Cell* const c)
{
  string trace_prefix = "eval.cpp::eval_eval(Cell*)";

  Cell *value;
  value = nil;
  try {
    assert_listsize(trace_prefix, "Expected only one operand", c, 1);

    value = car(c);

    // Evaluates twice, one for inner expression evaluation,
    //   second for actual (eval (...)) operation.
    value = eval(eval(value));

    return value;
  } catch (runtime_error& e) {
    throw_error(e.what(), trace_prefix);
  }
}
