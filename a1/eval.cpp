/**
 * \file eval.cpp
 *
 * Evaluates s-expressions from parse.cpp
 */

#include "eval.hpp"
#include <math.h>

Cell* eval(Cell* const c)
{
  Cell* c_car = NULL;
  Cell* c_cdr = NULL;

  // Checks if Empty Expression
  if(nullp(c)) {
    cerr << "ERROR: Invalid Input: at eval():\nEmpty expression.\n";
    exit(1);
  } else if(listp(c)) {
    c_car = car(c);
    c_cdr = cdr(c);
  }

  // Checks for inner expressions.
  if(listp(c) && nullp(c_car)) {
    cerr << "ERROR: Null reference: at eval():\nNull Operands.\n";
    exit(1);
  } else if(listp(c_car)) {
    c_car = inner_eval(c_car);
  } 

  // Main eval handling.
  if (symbolp(c_car)){
    string prefix = get_symbol(c_car);
    if(prefix == "+") {
      return add_eval(c_cdr);    
    } else if(prefix == "if") {
      return if_eval(c_cdr);
    } else if(prefix == "ceiling") {
      return ceil_eval(c_cdr);
    }
  } else {
    // For cases like (2).
    // Personal Note: I'd much rather return (2) as is,
    //  so to provide generalization.
    cerr << "ERROR: Invalid structure: at eval().\n";
    exit(1);
  }
}

Cell* inner_eval(Cell* const c)
{
  if(nullp(c)) {
    cerr << "ERROR: NULL Reference.\n Empty Operands.\n";
    exit(1);
  } else if(listp(c)){
    return eval(c);
  } else {
    return c;
  }
}

Cell* add_eval(Cell* const c)
{

  Cell* value = NULL;
  Cell* next = NULL;
  if(nullp(c)){
    // Returns default value 0 if no operands.
    return make_int(0);
  } else if(listp(c)) {
    // Checks for inner expressions.
    value = inner_eval(car(c));
    next = cdr(c);
  }
  
  // Checks if trying to add symbols.  
  if(symbolp(value) || symbolp(next)) {
    cerr << "ERROR: Type Mismatch: at add_eval\nExpected int or double type operands.\n";
    exit(1);
  }

  // Main recursive case.
  //  Checks until end of tree list, where next is null. Else recurse.
  if(nullp(next)) {
    return value;
  } else if(listp(next)) {
    next = add_eval(next);
  }

  // Returns double if at least one is double.
  if(intp(value) && intp(next)){
    return make_int(get_int(value) + get_int(next));
  } else {
    return make_double(
      (doublep(value) ? get_double(value) : get_int(value))
      + (doublep(next) ? get_double(next) : get_int(next))
    );
  }
}

Cell* if_eval(Cell* const c)
{
  Cell* test = NULL;
  Cell* result = NULL;
  if(nullp(c)) {
    cerr << "ERROR: NULL reference: at if_eval()\nEmpty operands.\n";
    exit(1);
  } else if(listp(c)){
    test = inner_eval(car(c));
    result = cdr(c);
  } else {
    cerr << "ERROR: Type mismatch: at if_eval()\nExpected conspairs.\n";
    exit(1);
  }

  // If there's a 4th operand.
  //  !nullp() for less than 3 operand compatibility
  if(!nullp(result) && listp(result) 
      && !nullp(cdr(result)) && listp(cdr(result)) 
      && !nullp(cdr(cdr(result)))
    ) 
  {
    cerr << "ERROR: Invalid Input: at if_eval()\nExpected at most 3 operands.\n";
    exit(1);
  }

  // Checks if insufficient results.
  if(nullp(result) || !listp(result)){
    cerr << "ERROR: Invalid Input: at if_eval()\nExpected at least 2 operand.\n";
    exit(1);
  }

  // Result handling.
  Cell* r_true = car(result);
  Cell* r_false = cdr(result);
  
  // True cases, checks if test is not zero.
  if(intp(test)) {
    if(get_int(test) != 0) {
      return inner_eval(r_true);
    }
  } else if (doublep(test)) {
    if(get_double(test) != 0) {
      return inner_eval(r_true);
    }
  } else if(symbolp(test)) {
    if(!get_symbol(test).empty()) {
      return inner_eval(r_true);
    }
  }
  
  // False cases.
  if(nullp(r_false) || !listp(r_false) || nullp(car(r_false))){
    cerr << "ERROR: NULL reference: at if_eval()\nFalse case is missing.\n";
    exit(1);
  } else {
    return inner_eval(car(r_false));
  }
}

Cell* ceil_eval(Cell* const c)
{
  Cell* value = NULL;
  if(nullp(c)) {
    cerr << "ERROR: NULL reference: at ceil_eval()\nEmpty operands\n";
    exit(1);
  } else if(listp(c)) {
    if(!nullp(cdr(c))){
      cerr << "ERROR: Invalid Input: at ceil_eval()\nExpected only one operand.\n";
      exit(1);
    } else {
      value = car(c);
    }
  }

  if(listp(value) || doublep(value)){
    return make_int((int)ceil(get_double(inner_eval(value))));
  } else {
    cerr << "ERROR: Type mismatch: at ceil_eval()\nExpected operands to be a double or a conspair.\n";
    exit(1);
  }
}
