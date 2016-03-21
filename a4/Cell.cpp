#include "Cell.hpp"
#include "eval.hpp"

using namespace std;

Cell* const nil = 0;
extern vector< map<string, Cell*> > stack_frame;

////////////////////////////////////////////////////////////////////////////////
// REGION class Cell

Cell::~Cell()
{
  // Purposely Empty.
}

bool Cell::is_int() const
{
  return false;
}

bool Cell::is_double() const
{
  return false;
}

bool Cell::is_symbol() const
{
  return false;
}

bool Cell::is_cons() const
{
  return false;
}

bool Cell::is_operator() const
{
  return false;
}

bool Cell::is_procedure() const
{
  return false;
}

bool Cell::is_nonzerovalue() const
{
  throw_error("Expected an Int,Double or Symbol Cell.",
	      "Cell::is_nonzerovalue()");
}

int Cell::get_int() const
{
  throw_error("Not an IntCell",
	      "Cell::get_int()");
}

double Cell::get_double() const
{
  throw_error("Not a DoubleCell",
	      "Cell::get_double()");
}

char* Cell::get_symbol() const
{
  throw_error("Not a SymbolCell",
	      "Cell::get_symbol()");
}

Cell* Cell::get_car() const
{
  throw_error("Not a ConsCell",
	      "Cell::get_car()");
}

Cell* Cell::get_cdr() const
{
  throw_error("Not a ConsCell",
	      "Cell::get_cdr()");
}

double Cell::get_value() const
{
  throw_error("Not an IntCell or DoubleCell",
	      "Cell::get_value()");
}

Cell* Cell::get_formals() const
{
  throw_error("Not a Procedure Cell",
	      "Cell::get_formals()");
}

Cell* Cell::get_body() const
{
  throw_error("Not a Procedure Cell",
	      "Cell::get_body()");
}

Cell* Cell::eval(Cell* const c) const
{
  throw_error("Not an OperatorCell",
	      "Cell::eval(Cell*)");
}

Cell* Cell::apply(Cell* const args) const
{
  throw_error("Not a ProcedureCell",
	      "Cell::apply(Cell*)");
}

// ENDREGION class Cell
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
// REGION class IntCell

IntCell::IntCell(const int i)
{
  int_m = i;
}

bool IntCell::is_int() const
{
  return true;
}

bool IntCell::is_nonzerovalue() const
{
  return get_value() != 0 ? true : false;
}

int IntCell::get_int() const
{
  return int_m;
}

double IntCell::get_value() const
{
  return (double) get_int();
}

void IntCell::print(ostream& os) const
{
  os << get_int();
}

Cell* IntCell::clone() const
{
  return new IntCell(get_int());
}

Cell* IntCell::eval() const
{
  return clone();
}

// ENDREGION class IntCell
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
// REGION class DoubleCell

DoubleCell::DoubleCell(const double d)
{
  double_m = d;
}

bool DoubleCell::is_double() const
{
  return true;
}

bool DoubleCell::is_nonzerovalue() const
{
  return get_value() != 0 ? true : false;
}

double DoubleCell::get_double() const
{
  return double_m;
}

double DoubleCell::get_value() const
{
  return get_double();
}

void DoubleCell::print(ostream& os) const
{
  os << fixed << setprecision(5);
  os << get_double();
}

Cell* DoubleCell::clone() const
{
  return new DoubleCell(get_double());
}

Cell* DoubleCell::eval() const
{
  return clone();
}

// ENDREGION class DoubleCell
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
// REGION class SymbolCell

SymbolCell::SymbolCell(const char* const s)
{
  char* str = new char();
  strcpy(str, s);
  symbol_m = str;
}

SymbolCell::~SymbolCell()
{
  delete [] symbol_m;
}

bool SymbolCell::is_symbol() const
{
  return true;
}

bool SymbolCell::is_nonzerovalue() const
{
  // Empty String.
  return ((string)get_symbol()).empty() ? false : true;
}

char* SymbolCell::get_symbol() const
{
  return symbol_m;
}

void SymbolCell::print(ostream& os) const
{
  os << get_symbol();
}

Cell* SymbolCell::clone() const
{
  return new SymbolCell(get_symbol());
}

Cell* SymbolCell::eval() const
{
  string trace_prefix = "SymbolCell::eval()";

  Operation operation = get_operation((string) get_symbol());
  if (operation != undefined_opr) {
    // if it's a defined operation.
    return new OperatorCell(get_symbol());    
  }
  
  // Check if symbol was a defined symbol.
  //  Uses mapped_type& map::at(const key_type& k);
  //    Returns a reference to the mapped value of the element identified with key k;
  //    Function throws out_of_range exception if k is not found;
  try {
    Cell* value = nil;
    vector< map<string, Cell*> >::reverse_iterator rit = stack_frame.rbegin();

    while (rit != stack_frame.rend()) { 
      try {
        // Uses *rit to dereference to the map in the vector.
	value = (*rit).at((string) get_symbol());
        return clone_cell(value);
      } catch (out_of_range&) {
        // Moves reversed_iterator to a lower stack_frame.
	++rit;
      }
    }

    // If value can't be found in whole stack_frame.
    throw_error("Attempted to reference an undefined symbol \"" + string(get_symbol()) + "\"");
  } catch (runtime_error& e) {
    throw_error(e.what(), trace_prefix);
  }
}

// ENDREGION class SymbolCell
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
// REGION class OperatorCell

OperatorCell::OperatorCell(const char* const s)
  : SymbolCell(s)
{
  // Purposely Empty.
}

bool OperatorCell::is_operator() const
{
  return true;
}

bool OperatorCell::is_symbol() const
{
  return false;
}

Cell* OperatorCell::eval() const
{
  return SymbolCell::clone();
}

Cell* OperatorCell::eval(Cell* const args) const
{
  switch (get_operation((string) get_symbol())) {
    case add_opr: {
      return arithmetic_eval(args, add_cells);
    }
    case if_opr: {
      return if_eval(args);
    }
    case ceil_opr: {
      return ceil_eval(args);
    }
    case minus_opr: {
      return arithmetic_eval(args, minus_cells);
    }
    case multiply_opr: {
      return arithmetic_eval(args, multiply_cells);
    }
    case divide_opr: {
      return arithmetic_eval(args, divide_cells);
    }
    case floor_opr: {
      return floor_eval(args);
    }
    case quote_opr: {
      return quote_eval(args);
    }
    case cons_opr: {
      return cons_eval(args);
    }
    case car_opr: {
      return car_eval(args);
    }
    case cdr_opr: {
      return cdr_eval(args);
    }
    case nullp_opr: {
      return nullp_eval(args);
    }
    case define_opr: {
      return define_eval(args);
    }
    case lessthan_opr: {
      return lessthan_eval(args);
    }
    case not_opr: {
      return not_eval(args);
    }
    case print_opr: {
      return print_eval(args);
    }
    case eval_opr: {
      return eval_eval(args);
    }
    case lambda_opr: {
      return lambda_eval(args);
    }
    case apply_opr: {
      return apply_eval(args);
    }
    case let_opr: {
      return let_eval(args);
    }
    default: {
      throw_error("Cannot evaluate unknown Operator Type",
		  "OperatorCell::eval(Cell*)");
    }
  }
}

Cell* OperatorCell::apply(Cell* const args) const
{
  string trace_prefix = "OperatorCell::apply(Cell*)";

  try {
    return eval(args);
  } catch (runtime_error& e) {
    throw_error(e.what(), trace_prefix);
  }
}


void OperatorCell::print(ostream& os) const
{
  throw_error("Cannot print Operators Cells.",
	      "OperatorCell::print(ostream&)");
}

// ENDREGION class OperatorCell
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
// REGION class ConsCell
ConsCell::ConsCell(Cell* const my_car, Cell* const my_cdr)
{
  car_m = my_car;
  cdr_m = my_cdr;
}

ConsCell::~ConsCell()
{
  if (!nullp(get_car()))
    delete car_m;
  if (!nullp(get_cdr()))
    delete cdr_m;
}

bool ConsCell::is_cons() const
{
  return true;
}

Cell* ConsCell::get_car() const
{
  return car_m;
}

Cell* ConsCell::get_cdr() const
{
  return cdr_m;
}

void ConsCell::print(ostream& os) const
{
  // Initial brackets
  os << "(";
  if (!nullp(get_car())) {
    get_car()->print(os);
  }
  if (!nullp(get_cdr())) {
    Cell* curr = get_cdr();
    // cdr requires looping inside.
    while (!nullp(curr)) {
      if (!listp(curr)) {
	curr->print();
	break;
      } else {
	os << " ";
	if (nullp(car(curr))) {
	  // for (quote ())
	  os << "()";
	  break;
	}
	car(curr)->print();
	curr = cdr(curr);
      }
    }
  }
  os << ")";
}

Cell* ConsCell::clone() const
{
  return new ConsCell(get_car(), get_cdr());
}

Cell* ConsCell::eval() const
{
  Cell* car = get_car();
  Cell* cdr = get_cdr();

  // Initialize
  //  check if inner s-exprs
  car = cell_eval(car);

  // Handle
  //  pass into deeper levels with car as operator, and cdr as arguments.
  //  else simply return car as result
  if (operatorp(car) || procedurep(car)) {
    return car->eval(cdr);
  } else {
    throw_error("Cannot evaluate non-operator and non-function cells.", 
		"ConsCell::eval()");
  }
}

// ENDREGION class ConsCell
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// REGION class ProcedureCell

ProcedureCell::ProcedureCell(Cell* const my_formals, Cell* const my_body)
{
  formals_m = my_formals;
  body_m = my_body;
}

ProcedureCell::~ProcedureCell()
{
  if (!nullp(get_formals()))
    delete formals_m;
  if (!nullp(get_body()))
    delete body_m;
}

bool ProcedureCell::is_procedure() const
{
  return true;
}

Cell* ProcedureCell::get_formals() const
{
  return formals_m;
}

Cell* ProcedureCell::get_body() const
{
  return body_m;
}

void ProcedureCell::print(ostream& os) const
{
  os << "#<function>";
}

Cell* ProcedureCell::clone() const
{
  return lambda(get_formals(), get_body());
}

Cell* ProcedureCell::eval() const
{
  return clone();
}

/**
 * \brief Maps formals with arguments for definition.
 * \return A map of the formals to the arguments.
 */
map<string, Cell*> pair_formals_args(Cell* const formals, Cell* const args)
{
  string trace_prefix = "Cell.cpp::pair_formals_args(Cell*, Cell*)";

  map<string, Cell*> defined_map;
  string key = "";
  pair<map<string, Cell*>::iterator, bool> itbool_pair;

  Cell *value_f, *value_a, *next_f, *next_a;
  value_f = value_a = next_f = next_a = nil;
  try {
    if (nullp(formals) && nullp(args)) {
      // if no formals and no arguments given in function.
      //  ((lambda () (...)) )
    } else if (symbolp(formals)) {
      // Multiple Arguments.
      //  Directly map the entire argument list to the key in formals.
      key = get_symbol(formals);      

      next_a = args;

      // Appends to front of list.
      Cell* temp;
      while (!nullp(next_a)) {
	value_a = cons(car(next_a), value_a);
	next_a = cdr(next_a);
      }

      // Reverses list back into order in value_f.
      next_f = value_a;
      while (!nullp(next_f)) {
	value_f = cons(cell_eval(car(next_f)), value_f);
	next_f = cdr(next_f);
      }

      itbool_pair = defined_map.insert(pair<string, Cell*>(key, value_f));
      if (itbool_pair.second == false) {
	throw_error("Cannot redefine a mapped definition \"" + key + "\"");
      }
    } else if (list_size(formals) != list_size(args)) {
      throw_error("Size of formals does not match size of arguments given.");
    } else {
      // Normally pair formals with arguments.
      next_f = formals;
      next_a = args;
    
      while (!nullp(next_f)) {
	value_f = car(next_f);
	value_a = cell_eval(car(next_a));
     
	next_f = cdr(next_f);
	next_a = cdr(next_a);

	key = get_symbol(value_f);      
	itbool_pair = defined_map.insert(pair<string, Cell*>(key, value_a));
	if (itbool_pair.second == false) {
	  throw_error("Cannot redefine a mapped definition \"" + key + "\"");
	}
      }
    }
    return defined_map;
  } catch (runtime_error& e) {
    throw_error(e.what(), trace_prefix);
  }
}

Cell* ProcedureCell::eval(Cell* const args) const
{
  string trace_prefix = "ProcedureCell::eval(Cell*)";
  Cell* formals = get_formals();
  Cell* body = get_body();

  map<string, Cell*> local_frame;

  Cell *expression, *next, *result;
  expression = next = result = nil;
  try {
    if (!nullp(formals)) {
      local_frame = pair_formals_args(formals, args);
      stack_frame.push_back(local_frame);
    }

    next = body;

    while (!nullp(next)) {
      expression = car(next);
      result = cell_eval(expression);
      next = cdr(next);
    }

    if (!nullp(formals)) {
      stack_frame.pop_back();
      local_frame.clear();
    }

    return result;
  } catch (runtime_error& e) {
    if (!local_frame.empty()) {
      stack_frame.pop_back();
    }
    throw_error(e.what(), trace_prefix);
  }
}

Cell* ProcedureCell::apply(Cell* const args) const
{
  string trace_prefix = "ProcedureCell::apply(Cell*)";

  try {
    return eval(args);
  } catch (runtime_error& e) {
    throw_error(e.what(), trace_prefix);
  }
}

// ENDREGION class ProcedureCell
////////////////////////////////////////////////////////////////////////////////
