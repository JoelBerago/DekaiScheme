#include <iostream>
#include "linkedlist.hpp"

int main(int argc, char** argv) {
  using namespace std;

  Node* mixed_list = make_node(make_int(1),NULL);
  Node* mixed_list_rev = make_node(make_int(1),NULL);
  Node* pos = mixed_list_rev;

  for (int i = 2; i <= 5; ++i) {
    list_insert_int(mixed_list, NULL, i);
    list_insert_int(mixed_list_rev, pos, i);
  }

  for (int i = 1; i <=5; ++i) {
    double d = i * 0.1;
    list_insert_double(mixed_list, NULL, d);
    list_insert_double(mixed_list_rev, pos, d);
  }

  for (char c = 'a'; c < 'f'; ++c) {
    char* str = new char[1];
    str[0] = c;
    str[1] = '\0';
    list_insert_symbol(mixed_list, NULL, str);
    list_insert_symbol(mixed_list_rev, pos, str);
  }
  
  cout << *mixed_list << "\n";
  cout << *mixed_list_rev << "\n";
  
  // Get all the even positioned elements (head at 1) and delete them
  Node* curr = mixed_list;
  int size = list_size(mixed_list);
  cout << "Printing mixed_list with even positioned elements deleted.\n";
  for(int i = 1; i < size; ++i){
    if(i % 2 == 0)
      list_erase(mixed_list, curr);
    curr = get_next(curr);
    cout << *mixed_list << "\n";
  }
  
  // Same for reversed
  curr = mixed_list_rev;
  size = list_size(mixed_list_rev);
  cout << "Printing mixed_list_rev with even positioned elements delete.\n";
  for(int i = 1; i < size; ++i){
    if(i % 2 == 0)
      list_erase(mixed_list_rev, curr);
    curr = get_next(curr);
    cout << *mixed_list_rev << "\n";
  }
  
  // Prints the size of the list.
  cout << "Size of the list is " << list_size(mixed_list) << "\n";
  cout << *mixed_list << "\n";

  cout << "Size of the reverse list is " << list_size(mixed_list_rev) << "\n";
  cout << *mixed_list_rev << "\n";

  // Write test cases for erase and ith
  // Print mixed_list_rev via for loop.
  size = list_size(mixed_list_rev);
  Cell* cell;
  for(int i = 0; i < size; ++i){
    cell = list_ith(mixed_list_rev, i);
    if(intp(cell))
      cout << get_int(cell);
    else if(doublep(cell))
      cout << get_double(cell);
    else if(symbolp(cell))
      cout << get_symbol(cell);

    cout << " ";
  }
  cout << "\n";
  return EXIT_SUCCESS;
}
