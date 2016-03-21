/*
 * \file eval.cpp
 *
 * Evaluates s-expressions from parse.cpp
 */

#include "eval.hpp"
#include <math.h>


Cell* eval(Cell* const c)
{
  // Ensure for empty expressions.
  if (nullp(c)) {
    cerr << "ERROR: Null reference: at eval()" << endl
	 << "c was null" << endl;
    exit(1);
  }

  Cell* operation = car(c);

  // Loop to ensure that first-most cell is an operator cell.
  while (!nullp(operation) && !intp(operation) && 
         !doublep(operation)) {
    if (listp(operation)) {
      operation = car(operation);
    } else if (symbolp(operation)) {
      if (get_operation(get_symbol(operation)) != undefined_opr) {
        break;
      } else {
        cerr << "ERROR: Invalid Input: at eval()" << endl
	     << "Not an operator." << endl;
	exit(1);
      }
    }
  }

  if (nullp(operation) || intp(operation) || doublep(operation)) {
    cerr << "ERROR: Invalid Input: at eval()" << endl
	 << "First most cell must be an operator cell." << endl;
    exit(1);
  }

  return c->eval();
}

Cell* add_eval(Cell* const c)
{
  // (+) returns 0
  if (nullp(c)) {
    return make_int(0);
  }
  
  Cell* value = car(c);
  Cell* next = cdr(c);
  Cell* next_value = nil;
  bool is_double = false;
  Cell* temp = nil;

  if (nullp(value)) {
    cerr << "ERROR: Null reference: at add_eval(Cell*)" << endl
	 << "car was null." << endl;
  } else {
    value = value->eval();
  }
  
  // Forward loop through list and adds next_value to value until the end.
  while (!nullp(next)) {
    next_value = car(next)->eval();
    if (is_double || doublep(value) || doublep(next_value)) {
      is_double = true;
      temp = value;
      value = make_double(get_value(value) + get_value(next_value));
      delete temp;
    } else {
      temp = value;
      value = make_int((int)(get_value(value) + get_value(next_value)));
      delete temp;
    }

    next = cdr(next);
  }

  return value;
}

Cell* if_eval(Cell* const c)
{
  if (nullp(c)) {
    cerr << "ERROR: Invalid Input: at if_eval(Cell*)" << endl
	 << "Empty Operands" << endl;
    exit(1);
  }

  Cell* test = car(c);
  Cell* result = cdr(c);

  int size = list_size(c);

  if (size < 2 || size > 3) {
    cerr << "ERROR: Invalid Input: at if_eval(Cell*)" << endl
	 << "Expected 2 or 3 operands." << endl;
    exit(1);
  }

  if (nullp(test)) {
    cerr << "ERROR: Invalid Input: at if_eval(Cell*)" << endl
	 << "Empty test case" << endl;
    exit(1);
  } else {
    test = test->eval();
  }

  // Result handling.
  Cell* r_true = car(result);
  Cell* r_false = cdr(result);
  
  if (nonzerop(test)) {
    // if test is true, i.e. nonzero
    return r_true->eval();
  } else {
    // if test is false, i.e. zero
    // Check if False case is given.
    if (size != 3) {
      cerr << "ERROR: Invalid Input: at if_eval(Cell*)" << endl
           << "False case is missing" << endl;
      exit(1);
    } else {
      return r_false->eval();
    }
  }
}

Cell* ceil_eval(Cell* const c)
{
  if (nullp(c)) {
    cerr << "ERROR: Invalid Input: at ceil_eval(Cell*)" << endl
	 << "Empty Operands" << endl;
    exit(1);
  }

  Cell* value = car(c);

  if (list_size(c) != 1){
    cerr << "ERROR: Invalid Input: at ceil_eval(Cell*)" << endl 
	 << "Expected only one operand." << endl;
    exit(1);
  }

  if (nullp(value)) {
    cerr << "ERROR: Null reference: at ceil_eval(Cell*)" << endl
	 << "Empty Operands" << endl;
    exit(1);
  } else {
    value = value->eval();
  }

  return make_int((int)ceil(get_value(value)));
}


Cell* minus_eval(Cell* const c)
{
  if (nullp(c)) {
    cerr << "ERROR: Invalid Input: at minus_eval(Cell*)" << endl
	 << "Empty Operands" << endl;
    exit(1);
  }

  Cell* value = car(c);
  Cell* next = cdr(c);
  Cell* next_value = nil;
  bool is_double = false;
  Cell* temp = nil;

  if (nullp(value)) {
    cerr << "ERROR: Invalid Input: at minus_eval(Cell*)" << endl
	 << "Empty Operands" << endl;
    exit(1);
  } else {
    value = value->eval();
  }

  // If one operand, returns negative of operand.
  if (nullp(next)) {
    if (doublep(value)) {
      return make_double(get_double(value) * -1);
    } else {
      return make_int(get_int(value) * -1);
    }
  }  

  // Forward loop, subtracting next_value from value.
  while (!nullp(next)) {
    next_value = next->eval();
    if (is_double || doublep(value) || doublep(next_value)) {
      is_double = true;
      temp = value;
      value = make_double(get_value(value) - get_value(next_value));
      delete temp;
    } else {
      temp = value;
      value = make_int((int)(get_value(value) - get_value(next_value)));
      delete temp;
    }

    next = cdr(next);
  }

  return value;
}

Cell* multiply_eval(Cell* const c)
{
  // (*) returns 1
  if (nullp(c)) {
    return make_int(1);
  }

  Cell* value = car(c);
  Cell* next = cdr(c);
  Cell* next_value = nil;
  bool is_double = false;
  Cell* temp = nil;

  if (nullp(value)) {
    cerr << "ERROR: Invalid Input: at multiply_eval(Cell*)" << endl
	 << "Empty Operands" << endl;
    exit(1);
  } else {
    value = car(c)->eval();
    next = cdr(c);
  }
  
  // Forward loop, mutiplying next_value to value.
  while (!nullp(next)) {
    next_value = next->eval();
    if (is_double || doublep(value) || doublep(next_value)) {
      is_double = true;
      temp = value;
      value = make_double(get_value(value) * get_value(next_value));
      delete temp;
    } else {
      temp = value;
      value = make_int((int)(get_value(value) * get_value(next_value)));
      delete temp;
    }

    next = cdr(next);
  }

  return value;
}

Cell* divide_eval(Cell* const c)
{
  if (nullp(c)) {
    cerr << "ERROR: Invalid Input: at divide_eval(Cell*)" << endl
	 << "Empty Operands" << endl;
    exit(1);
  }

  Cell* value = car(c);
  Cell* next = cdr(c);
  Cell* next_value = nil;
  bool is_double = false;
  Cell* temp = nil;

  if (nullp(value)) {
    cerr << "ERROR: Invalid Input: at divide_eval(Cell*)" << endl
	 << "Empty Operands" << endl;
    exit(1);
  } else {
    value = value->eval();
  }

  if (get_value(value) == 0) {
    cerr << "ERROR: Invalid Input: at divide_eval(Cell*)" << endl
         << "Tried to divide by zero" << endl;
    exit(1);
  }

  
  // If only one operand, give inverse.
  if (nullp(next)) {
    return make_double(1.0 / get_value(value)); 
  }

  // Forward loop, dividing next_value from value.
  while (!nullp(next)) {
    next_value = next->eval();

    if (get_value(next_value) == 0) {
      cerr << "ERROR: Invalid Input: at divide_eval(Cell*)" << endl
	   << "Tried to divide by zero" << endl;
      exit(1);
    }

    if (is_double || doublep(value) || doublep(next_value)) {
      is_double = true;
      temp = value;
      value = make_double(get_value(value) / get_value(next_value));
      delete temp;
    } else {
      temp = value;
      value = make_int((int)(get_value(value) / get_value(next_value)));
      delete temp;
    }

    next = cdr(next);
  }

  return value;
}

Cell* floor_eval(Cell* const c)
{
  if (nullp(c)) {
    cerr << "ERROR: Invalid Input: at floor_eval(Cell*)" << endl
	 << "Empty Operands" << endl;
    exit(1);
  }

  if (list_size(c) != 1){
    cerr << "ERROR: Invalid Input: at floor_eval(Cell*)" << endl 
	 << "Expected only one operand." << endl;
    exit(1);
  }

  Cell* value = car(c);

  if (nullp(value)) {
    cerr << "ERROR: Null reference: at floor_eval(Cell*)" << endl
	 << "Empty Operands" << endl;
    exit(1);
  }

  return make_int((int)floor(get_value(value)));
}

Cell* quote_eval(Cell* const c)
{
  if (nullp(c)) {
    cerr << "ERROR: Invalid Input: at quote_eval(Cell*)" << endl
	 << "Empty Operands" << endl;
    exit(1);
  }

  if (list_size(c) != 1) {
    cerr << "ERROR: Invalid Input: at quote_eval(Cell*)" << endl
	 << "Expected only one operand." << endl;
    exit(1);
  }

  Cell* my_car = car(c);
  // (quote ())
  if (nullp(my_car)) {
    return nil;
  }

  return my_car->clone();
}

Cell* cons_eval(Cell* const c)
{
  if (nullp(c)) {
    cerr << "ERROR: Invalid Input: at cons_eval(Cell*)" << endl
	 << "Empty Operands" << endl;
    exit(1);
  }

  int size = list_size(c);

  if (size != 2) {
    cerr << "ERROR: Invalid Input: at cons_eval(Cell*)" << endl 
	 << "Expected 2 operands" << endl;
    exit(1);
  }

  Cell* my_car = car(c);
  Cell* my_cdr = cdr(c);

  if (nullp(my_car)) {
    cerr << "ERROR: Null reference: at cons_eval(Cell*)" << endl
	 << "Empty Operands" << endl;
    exit(1);
  } else {
    my_car = my_car->eval();
  }

  if (nullp(my_cdr)) {
    cerr << "ERROR: Null Reference: at cons_eval(Cell*)" << endl
	 << "Second Operand Empty" << endl;
    exit(1);
  } else {
    my_cdr = my_cdr->eval();
  }

  return cons(my_car, my_cdr);
}

Cell* car_eval(Cell* const c)
{
  if (nullp(c)) {
    cerr << "ERROR: Invalid Input: at car_eval(Cell*)" << endl
	 << "Empty Operands" << endl;
    exit(1);
  }

  if (list_size(c) != 1) {
    cerr << "ERROR: Invalid Input: at car_eval(Cell*)" << endl
	 << "Expected only one operand" << endl;
    exit(1);
  }

  Cell* my_car = car(c);
  Cell* result = nil;

  if (nullp(my_car)) {
    cerr << "ERROR: Null reference: at car_eval(Cell*)" << endl
	 << "Empty Operands" << endl;
    exit(1);
  } else {
    my_car = my_car->eval();
  }

  // result in car.
  if (nullp(my_car)) {
    cerr << "ERROR: Null reference: at car_eval(Cell*)" << endl
	 << "Cannot car nil." << endl;
    exit(1);
  } else {
    result = car(my_car);
  }

  return result;
}

Cell* cdr_eval(Cell* const c)
{
  if (nullp(c)) {
    cerr << "ERROR: Invalid Input: at cdr_eval(Cell*)" << endl
	 << "Empty Operands" << endl;
    exit(1);
  }

  if (list_size(c) != 1) {
    cerr << "ERROR: Invalid Input: at cdr_eval(Cell*)" << endl
	 << "Expected only one operand" << endl;
    exit(1);
  }

  Cell* my_car = car(c);
  Cell* result = nil;

  // First operand is still car();
  if (nullp(my_car)) {
    cerr << "ERROR: Null reference: at cdr_eval(Cell*)" << endl
	 << "Empty Operands" << endl;
    exit(1);
  } else {
    my_car = my_car->eval();
  }

  // result is in cdr.
  if (nullp(my_car)) {
    cerr << "ERROR: Null reference: at cdr_eval(Cell*)" << endl
	 << "Cannot cdr nil." << endl;
    exit(1);
  } else {
    result = cdr(my_car);
  }
  return result;
}

Cell* nullp_eval(Cell* const c)
{
  if (nullp(c)) {
    cerr << "ERROR: Invalid Input: at nullp_eval(Cell*)" << endl
	 << "Empty Operands" << endl;
    exit(1);
  }

  if (list_size(c) != 1) {
    cerr << "ERROR: Invalid Input: at nullp_eval(Cell*)" << endl
	 << "Expected only one operand" << endl;
    exit(1);
  }

  Cell* result = car(c);

  if (!nullp(result)) {
    result = result->eval();
  }

  // If Empty Operand, returns 1; else return 0; 
  if (nullp(result)) {
    return make_int(1);
  } else {
    return make_int(0);
  }
}
