#include "Cell.hpp"
#include "eval.hpp"

Cell* const nil = 0;

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
  std::cerr << "ERROR: Type Mismatch: at Cell::is_nonzerovalue()" << std::endl
	    << "Expected an Int, Double or Symbol Cell." << std::endl;
  exit(1);
}

int Cell::get_int() const
{
  std::cerr << "ERROR: Type Mismatch: at Cell::get_int()" << std::endl
	    << "Not an IntCell" << std::endl;
  exit(1);
}

double Cell::get_double() const
{
  std::cerr << "ERROR: Type Mismatch: at Cell::get_double()" << std::endl
	    << "Not a DoubleCell" << std::endl;
  exit(1);
}

char* Cell::get_symbol() const
{
  std::cerr << "ERROR: Type Mismatch: at Cell::get_symbol()" << std::endl
	    << "Not a SymbolCell" << std::endl;
  exit(1);
}

Cell* Cell::get_car() const
{
  std::cerr << "ERROR: Type Mismatch: at Cell::get_car()" << std::endl
            << "Not a ConsCell" << std::endl;
  exit(1);
}

Cell* Cell::get_cdr() const
{
  std::cerr << "ERROR: Type Mismatch: at Cell::get_cdr()" << std::endl
	    << "Not a ConsCell" << std::endl;
  exit(1);
}

double Cell::get_value() const
{
  std::cerr << "ERROR: Type Mismatch: at Cell::get_value()" << std::endl
	    << "Not an IntCell or DoubleCell" << std::endl;
  exit(1);
}

Cell* Cell::eval(Cell* const c) const
{
  std::cerr << "ERROR: Type Mismatch: at Cell::eval(Cell*)" << std::endl
	    << "Expected an OperatorCell" << std::endl;
  exit(1);
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
  return new IntCell(int_m);
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
  if (operation == undefined_opr) {
    return clone();
  } else {
    // else turn it into an OperatorCell.
    return new OperatorCell(get_symbol());
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
      return add_eval(args);
    }
    case if_opr: {
      return if_eval(args);
    }
    case ceil_opr: {
      return ceil_eval(args);
    }
    case minus_opr: {
      return minus_eval(args);
    }
    case multiply_opr: {
      return multiply_eval(args);
    }
    case divide_opr: {
      return divide_eval(args);
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
    default: {
      std::cerr << "ERROR: Type Error: at OperatorCell::eval(Cell*)" << std::endl
		<< "cannot evaluate unknown operator type." << std::endl;
      exit(1);
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
  delete car_m;
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
	  // cout << "()";
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

  // "Initialize", check if inner s-exprs
  if (!nullp(car)) {
    car = car->eval();
  }

  // "Handle"
  // pass into deeper levels with car as operator, and cdr as arguments.
  // else simply return car as "result"
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
