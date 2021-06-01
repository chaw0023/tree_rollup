#include <iostream>
#include <vector>

#include "tree.h"

int Node::id_counter = 0;

float Node::get_weight(){
  return self_weight+added_weight;
}

int Node::get_id(){
  return node_id;
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

Node* Node::add_random_child_to_node(){
  Node *new_node = new Node(rand()*1.0*MAX_WEIGHT/RAND_MAX, "India");
  new_node->parent = this;
  children.push_back(new_node);
  return new_node;
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
    for(int i=0;i<n_children_per_node;i++){
      Node* new_child = root->add_random_child_to_node();
      if(levels==1){
        leafnodes.insert({new_child, new_child->get_id()});
        leaves_vector.push_back(new_child);
        // Node::leafnodes[new_child->get_id()] = new_child;
        continue;
      }
      create_simple_tree(new_child, n_children_per_node, levels-1);
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
  //create a tree with random weights within 0 to MAX_WEIGHT
  Node *root = new Node(rand()*1.0*MAX_WEIGHT/RAND_MAX, "India");
  int n_children_per_node=2,levels=2;
  if(argc>1) n_children_per_node = std::stoi(argv[1]);
  if(argc>2) levels = std::stoi(argv[2]);
  create_simple_tree(root,n_children_per_node,levels);

  //Select a node to test weight addition.
  root->print();//print_tree(root);
  start = clock();
  int size = leafnodes.size();
  for(int i=0;i<1e4*size;i++){
    int idx = rand()%size;
      leaves_vector[idx]->add_weight(rand()*1.1e-4/RAND_MAX);
    // if(!(i%size)) root->print();
  }
  stop = clock();
  printf("Time taken for add_weight = %lf(micros)\n", (1.0e6*(double)(stop-start))/CLOCKS_PER_SEC);
  printf("**************, Leaves = %ld*******\n", leafnodes.size());
  root->print();
  // for(auto leaf : leafnodes)
    // print_tree(leaf.first);
  printf("**************\n");
  // root->parent->print();
  delete(root);
  return 0;
}
