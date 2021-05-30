#include <iostream>
#include <vector>

#include "tree.h"

int Node::id_counter = 0;

float Node::get_weight(){
  return self_weight+added_weight;
}

bool Node::add_weight(float wt)
{
  if(!children.empty()) return false;
  rollup_weight(wt);
  return true;
}

void Node::rollup_weight(float wt)
{
  added_weight+=wt;
  if(parent!=NULL) parent->rollup_weight(wt);
}

void Node::print(){
  printf("Node weight = %.2f \t", get_weight());
  printf("node id = %d\n", node_id);
}

void Node::add_random_children_to_node(int n){
  if(n<1) return;
  Node *new_node = new Node(rand()*1.0*MAX_WEIGHT/RAND_MAX, "India");
  new_node->parent = this;
  children.push_back(new_node);
  add_random_children_to_node(--n);
}

void Node::add_children_to_node(int n, float init_wt){
  while(n--){
    Node *new_node = new Node(init_wt, "India");
    new_node->parent = this;
    children.push_back(new_node);
  }
}

void create_simple_tree(Node* root, int n_children_per_node, int levels){
  if (levels>0){
  root->add_random_children_to_node(n_children_per_node);
  levels--;
    for(auto child: root->children){
      create_simple_tree(child, n_children_per_node, levels);
    }
  }
}

void print_tree(Node* node){
  if(!node)
    return;
    // std::runtime_error("[print_tree] Error accessing node while printing!!!!!!\n");
  node->print();
  for(auto child: node->children)
    print_tree(child);
}
/***main function
  creates a simple tree then adds random weights to leaf that roll up.
**/
int main(int argc, char const *argv[]) {
  float timer = 0;
  clock_t start, stop;
  printf("This is start of main. %d, argv[0]=%s\n****************\n", argc, argv[0]);
  // srand(SEED);
  Node *root = new Node(rand()*1.0*MAX_WEIGHT/RAND_MAX, "India");
  int n_children_per_node=2,levels=2;
  if(argc>1) n_children_per_node = std::stoi(argv[1]);
  if(argc>2) levels = std::stoi(argv[2]);
  // root->add_random_children_to_node(n_children);
  create_simple_tree(root,n_children_per_node,levels);

  Node* test_node = root;
  while(!test_node->children.empty()) test_node = test_node->children[rand()%n_children_per_node];
  start = clock();
  for(int i=0;i<20000;i++) test_node->add_weight(rand()*1.0/RAND_MAX);
  stop = clock();
  printf("Time taken for add_weight = %lf(micros)\n", (1.0e6*(double)(stop-start))/CLOCKS_PER_SEC);
  test_node->print();
  printf("**************\n");
  // print_tree(root);
  printf("**************\n");
  test_node->parent->print();
  test_node->parent->parent->print();
  test_node->parent->parent->parent->print();
  return 0;
}
