#include "Cell.hpp"
#include "eval.hpp"

Cell* const nil = 0;
extern map<std::string, Cell*> defined_map;

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

bool Cell::is_nonzerovalue() const
{
  throw_error("Cell::is_nonzerovalue()", 
	      "Expected an Int,Double or Symbol Cell.");
}

int Cell::get_int() const
{
  throw_error("Cell::get_int()",
	      "Not an IntCell");
}

double Cell::get_double() const
{
  throw_error("Cell::get_double()", 
	      "Not a DoubleCell");
}

char* Cell::get_symbol() const
{
  throw_error("Cell::get_symbol()", 
	      "Not a SymbolCell");
}

Cell* Cell::get_car() const
{
  throw_error("Cell::get_car()", 
	      "Not a ConsCell");
}

Cell* Cell::get_cdr() const
{
  throw_error("Cell::get_cdr()", 
	      "Not a ConsCell");
}

double Cell::get_value() const
{
  throw_error("Cell::get_value()", 
	      "Not an IntCell or DoubleCell");
}

Cell* Cell::eval(Cell* const c) const
{
  throw_error("Cell::eval(Cell*)", 
	      "Not an OperatorCell");
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

void IntCell::print(std::ostream& os) const
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

void DoubleCell::print(std::ostream& os) const
{
  os << std::fixed << std::setprecision(5);
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
  std::strcpy(str, s);
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
  return ((string)get_symbol()).empty() ? false : true;
}

char* SymbolCell::get_symbol() const
{
  return symbol_m;
}

void SymbolCell::print(std::ostream& os) const
{
  os << get_symbol();
}

Cell* SymbolCell::clone() const
{
  return new SymbolCell(get_symbol());
}

Cell* SymbolCell::eval() const
{
  Operation operation = get_operation((string) get_symbol());
  // return a copy for quote.
  if (operation != undefined_opr) {
    // if it's a defined operation.
    return new OperatorCell(get_symbol());    
  } 
  
  // Check if symbol was a defined symbol.
  //  Uses mapped_type& map::at(const key_type& k);
  //    Returns a reference to the mapped value of the element identified with key k;
  //    Function throws out_of_range exception if k is not found;
  try {
    Cell* value = defined_map.at((string) get_symbol());
    return clone_cell(value);
  } catch (std::out_of_range&) {
    //    return clone();
    throw_error("Attempted to reference an undefined symbol \"" + string(get_symbol()) + "\"" ,
		"SymbolCell::eval()");
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

Cell* OperatorCell::clone() const
{
  return new OperatorCell(get_symbol());
}

Cell* OperatorCell::eval() const
{
  return clone();
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
    default: {
      throw_error("Cannot evaluate unknown Operator Type",
		  "OperatorCell::eval(Cell*)");
    }
  }
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
  if (!nullp(car_m))
    delete car_m;
  if (!nullp(cdr_m))
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

void ConsCell::print(std::ostream& os) const
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
	  cout << "()";
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

  // when upper expression returns an operator symbol.
  if (symbolp(car)) {
    Operation operation = get_operation((string) car->get_symbol());
    if (operation != undefined_opr) {
      // if it's a defined operation.
      car = make_operator(car->get_symbol());
    }
  }

  // Handle
  //  pass into deeper levels with car as operator, and cdr as arguments.
  //  else simply return car as result
  if (operatorp(car)) {
    return car->eval(cdr);
  } else {
    return car;
  }
}

bool ConsCell::is_emptylist() const
{
  // empty lists are Cons(NULL,NULL);
  if (nullp(get_car()) && nullp(get_cdr())) {
    return true;
  } else {
    return false;
  }
}

// ENDREGION class ConsCell
////////////////////////////////////////////////////////////////////////////////
