/**
 * \file eval.cpp
 *
 * Evaluates s-expressions from parse.cpp
 */

#include "eval.hpp"

using namespace std;

// Initialize stack_frame with one map frame.
vector< map<string, Cell*> > stack_frame(1, map<string, Cell*>());

Cell* eval(Cell* const c)
{
  string trace_prefix = "eval.cpp::eval(Cell*)";

  Cell *operation, *value;
  operation = value = nil;
  try {
    // if s-expression is just a string to a defined mapped cell.
    if (listp(c)) {
      operation = car(c);
    } else {
      operation = c;
    } 

    // Ensure first cell is an operation cell.
    //   If first cell is a symbol, let SymbolCell eval handle for defined.
    if (nullp(operation) || intp(operation) || doublep(operation)) {
      throw_error("First most cell must be an operator cell.");
    }

    return cell_eval(c);
  } catch (runtime_error& e) {
    throw_error(e.what(), trace_prefix); 
  }
}


Cell* arithmetic_eval(Cell* const c, Cell* (*funct)(Cell*,Cell*))
{
  string trace_prefix = "eval.hpp::arithmetic_eval(Cell* const c, Cell* (*funct)(Cell*,Cell*))";

  if (nullp(c)) {
    if (funct == add_cells) {
      // Args-less add returns 0
      return make_int(0);
    } else if (funct == multiply_cells) {
      // Args-less multiply returns 1
      return make_int(1);
    }
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
      assert_isnonzerovalue("Tried to divide by zero", value);
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

      value = funct(value, next_value);
    }

    // Ensure final value is an Int or DoubleCell then return.
    assert_isdoubleintcell("Expected IntCell or DoubleCell", value);
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
    // cannot divide by zero.
    assert_isnonzerovalue("Tried to divide by zero", next_value);

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
    assert_listsize("Expected 2 or 3 operands", c, 2, 3);

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
      assert_listsize("False case is missing", c, 3);
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
    assert_listsize("Expected only one operand", c, 1);

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
    assert_listsize("Expected only one operand", c, 1);

    value = car(c);  

    value = cell_eval(value);

    return make_int((int)floor(get_double(value)));
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
    assert_listsize("Expected only one operand", c, 1);

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
    assert_cellnotnull("Empty Operands : c", c);
    assert_listsize("Expected 2 operands", c, 2);

    Cell* my_car = car(c);
    Cell* my_cdr = car(cdr(c));

    my_car = cell_eval(my_car);
    my_cdr = cell_eval(my_cdr);

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
    assert_listsize("Expected only one operand", c, 1);

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
    assert_listsize("Expected only one operand", c, 1);

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
    assert_listsize("Expected only one operand", c, 1);

    result = car(c);

    result = cell_eval(result);

    // If Empty Operand, returns 1 then else return 0
    if (nullp(result)) {
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
    assert_listsize("Expected only two operands", c, 2);
    key_c = car(c);
    value = car(cdr(c));

    value = cell_eval(value);

    string key = get_symbol(key_c);
    if (get_operation(key) != undefined_opr) {
      throw_error("Cannot define with a reserved keyword \"" + key + "\"");
    }

    // Uses map::insert(pair<Key, T>) which returns a pair<iterator, bool>
    //   pair.second is bool and stores if insert was successful.
    //   if pair.second is false, it means there has been an element defined on that key. 

    pair<map<string, Cell*>::iterator, bool> itbool_pair;
    map<string, Cell*> defined_frame = stack_frame.back();
    stack_frame.pop_back();

    itbool_pair = defined_frame.insert(pair<string, Cell*>(key, value));
    stack_frame.push_back(defined_frame);

    if (itbool_pair.second == false) {
      throw_error("Cannot redefine a mapped definition \"" + key + "\"");
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
      assert_isdoubleintcell("Expected an Int or Double cell", next_value);

      // Short Circuit evaluation, returns immediately when false.
      //   Case was unspecified, but suggested to assert that all arguments are int or double type.
      //     (< 1 4.5 3 UNDEFINED)
      //     (< 1 4.5 3 (if 1 UNDEFINED)) <<-- Unsure how to handle without fully evaluating given list.
      if (get_value(value) >= get_value(next_value)) {
        return make_int(0);
      }
      value = next_value;
    }

    assert_isdoubleintcell("Expected an Int or Double cell", value);
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
    assert_listsize("Expected only one operand", c, 1);

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
    assert_listsize("Expected only one operand", c, 1);

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
    assert_listsize("Expected only one operand", c, 1);

    value = car(c);

    // Evaluates twice, one for inner expression evaluation,
    //   second for actual (eval (...)) operation.
    value = cell_eval(cell_eval(value));

    return value;
  } catch (runtime_error& e) {
    throw_error(e.what(), trace_prefix);
  }
}

Cell* lambda_eval(Cell* const c)
{
  string trace_prefix = "eval.cpp::lambda_eval(Cell*)";

  Cell *formals, *body;
  formals = body = nil;

  try {
    formals = car(c);
    body = cdr(c);

    return lambda(formals, body);
  } catch (runtime_error& e) {
    throw_error(e.what(), trace_prefix);
  }
}

Cell* apply_eval(Cell* const c)
{
  string trace_prefix = "eval.cpp::apply_eval(Cell*)";

  Cell *procedure, *args;
  procedure = args = nil;

  try {
    assert_listsize("Expected only two operands", c, 2);

    // Evaluate procedure twice for
    //  variable --> symbol definition -- lambda function.
    procedure = cell_eval(car(c));
    args = cell_eval(car(cdr(c)));

    return cell_apply(procedure, args);
  } catch (runtime_error& e) {
    throw_error(e.what(), trace_prefix);
  }
}

Cell* let_eval(Cell* const c)
{
  string trace_prefix = "eval.cpp::let_eval(Cell*)";

  Cell *formals, *funct_body, *args, *procedure, *var_pair, *var_list;
  formals = funct_body = procedure = args = var_pair = var_list= nil;
  try {
    var_list = car(c);
    funct_body = cdr(c);

    while (!nullp(var_list)) {
      var_pair = car(var_list);
      formals = cons(car(var_pair),formals);
      args = cons(car(cdr(var_pair)), args);
      var_list = cdr(var_list);
    }

    procedure = lambda(formals, funct_body);
    return procedure->apply(args);
  } catch (runtime_error& e) {
    throw_error(e.what(), trace_prefix);
  }
}
