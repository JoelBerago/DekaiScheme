#include "linkedlist.hpp"

int list_size(const Node* n) {
  int size = 0;
  while(n != NULL){
    ++size;
    n = get_next(n);
  }
  return size;
}

Cell* list_ith(Node* n, unsigned int i) {
  for(; i > 0; --i){
    if(n == NULL){
      std::cerr << "Error at list_ith(Node* n, unsigned int i)\n";
      std::cerr << "Error: Given node n became NULL, out of bounds.\n";
      exit(1);
    }

    n = get_next(n);
  }
  return get_elem(n);
}

Node* list_erase(Node* n, Node* pos) {
  if(n == NULL || pos == NULL){
    std::cerr << "Error at list_erase(Node* n, Node* pos)\n";
    std::cerr << "Error: Head node n and/or node pos were NULL pointers.\n";
    exit(1);
  }
  
  // To delete head node. Deletes n's cell, moves next cell's contents to n's cell pointer.
  if(n == pos){
    free(get_elem(n));
    // Save next node as n->next_m will be overwriten.
    Node* next = get_next(n);
    if(get_next(n) != NULL){
      n->elem_m = get_elem(next);
      n->next_m = get_next(next);
      free(next);
    } else{
      n->elem_m = NULL;
      n->next_m = NULL;
    }
  } else{
    while(get_next(n) != pos)
      n = get_next(n);
  
    n->next_m = get_next(pos);
    free(get_elem(pos));
    free(pos);
  }
  return get_next(n);
}

Node* list_insert(Node* n, Node* pos, Cell* c) {
  if(n == NULL){
    std::cerr << "Error at list_insert(Node* n, Node* pos, Cell* c)\n";
    std::cerr << "Error: Given head node n is NULL.\n";
    exit(1);
  }
  // if insert at head node, copy over n's cell contents to a new node after n
  // then insert c back into n.
  if(n == pos){
    Cell* n_cell = get_elem(n);
    n->elem_m = c;
    n->next_m = make_node(n_cell, get_next(n));
    return n;
  }
  else{
    while(get_next(n) != pos)
      n = get_next(n);

    n->next_m = make_node(c, pos);
    return get_next(n);
  }
}

Node* list_insert_int(Node* n, Node* pos, const int value) {
  return list_insert(n, pos, make_int(value));
}

Node* list_insert_double(Node* n, Node* pos, const double value) {
  return list_insert(n, pos, make_double(value));
}

Node* list_insert_symbol(Node* n, Node* pos, const char* value) {
  return list_insert(n, pos, make_symbol(value));
}
