#include "tree.h"

int Node::id_counter = 0;

/** Returns total weight for a node
**/
inline float Node::get_weight(){
  return self_weight+added_weight;
}

/** Returns integet unique id for a node
**/
int Node::get_id(){
  return node_id;
}

/** Adds weight wt to a node and rolls up to the ancestors.
    wt: weight to be added
**/
bool Node::add_weight(float wt)
{
  if(!children.empty()) return false;
  rollup_weight(wt);
  return true;
}

/** Prints information of node and sub-tree under it.
    node: pointer to Node to be printed
**/
void Node::rollup_weight(float wt)
{
  added_weight+=wt;
  if(parent!=NULL) parent->rollup_weight(wt);
}

/** Prints information of a single node
**/
void Node::print(){
  printf("Node weight = %.2f \t", get_weight());
  printf("node id = %d\n", node_id);
}

/** Adds n children to a node initialized with random value of weights.
**/
void Node::add_random_children_to_node(int n){
  if(n<1) return;
  Node *new_node = new Node(rand()*1.0*MAX_WEIGHT/RAND_MAX, "India");
  new_node->parent = this;
  children.push_back(new_node);
  add_random_children_to_node(--n);
}

/** Adds a single child to the node and initializes with a random
**/
Node* Node::add_random_child_to_node(){
  Node *new_node = new Node(rand()*1.0*MAX_WEIGHT/RAND_MAX, "India");
  new_node->parent = this;
  children.push_back(new_node);
  return new_node;
}

/** Adds n children to a node initialized with weight if init_wt.
    n: number of children to add.
    init_wt: initial weight for the children.
**/
void Node::add_children_to_node(int n, float init_wt){
  while(n--){
    Node *new_node = new Node(init_wt, "India");
    new_node->parent = this;
    children.push_back(new_node);
  }
}

/** Create a simple saturated tree structure from a single root node.
    root: pointer to root Node of the tree to be created.
    n_children_per_node: number of children of each node in the print_tree.
    levels: number of levels to be added in the tree.
**/
void create_simple_tree(Node* root, int n_children_per_node, int levels){
  if (levels>0){
    for(int i=0;i<n_children_per_node;i++){
      Node* new_child = root->add_random_child_to_node();
      if(levels==1){
        leafnodes.insert({new_child, new_child->get_id()});
        leaves_vector.push_back(new_child);
        Node* ancestor = new_child->parent;
        float* ancestor_wt;
        ancestor_wt = &(ancestor->added_weight);
        std::vector<Node*> node_vector;
        std::vector<float*> wt_vector;
        while(ancestor!=NULL){
          node_vector.push_back(ancestor);
          wt_vector.push_back(&(ancestor->added_weight));
          ancestor = ancestor->parent;
        }
        ancestor_of_leaves.push_back(node_vector);
        ancestor_weights_of_leaves.push_back(wt_vector);
        // Node::leafnodes[new_child->get_id()] = new_child;
        continue;
      }
      create_simple_tree(new_child, n_children_per_node, levels-1);
    }
  }
}

/** Prints information of node and sub-tree under it.
    node: pointer to Node to be printed
**/
void print_tree(Node* node){
  if(!node)
    return;
    // std::runtime_error("[print_tree] Error accessing node while printing!!!!!!\n");
  node->print();
  for(auto child: node->children)
    print_tree(child);
}

/** Prints the weight of a leaf node and all its ancestors.
    leaf_idx: index of leaf node in the leafnodes vector to be printed.
**/
void print_node_and_ancestors(int leaf_idx){  //Leaf function
  std::string name_of_node="";
  for(auto l : ancestor_of_leaves[leaf_idx]){
    name_of_node += std::to_string(l->get_id());
    std::cout<<name_of_node <<" = "<<l->get_weight()<<"\n";
    name_of_node += ".";
  }
}

/** Adds a weight to leaf node all its ancestor.
    leaf_idx: index of leaf node in the leafnodes vector to which weight is added.
    wt: weight that needs to be added
**/
inline void deposit_weights_to_ancestors(int lead_node_idx, float wt){ //Leaf function
  //this type of loop is vetorization friendly. For MIMD *p could be 'gathered' over all threads before updating values.
  for(float* p : ancestor_weights_of_leaves[lead_node_idx]){
    *p = *p + wt;
  }
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
  printf("**************, Leaves = %ld*******\n", leafnodes.size());

  //Select a node to test weight addition.
  root->print();//print_tree(root);
  start = clock();
  int size = leafnodes.size();
  for(int i=0;i<1e2*size;i++){
    int idx = rand()%size;
      leaves_vector[idx]->add_weight(rand()*1.1e-4/RAND_MAX);
    if(!(i%size)) root->print();
  }
  stop = clock();
  printf("Time taken for add_weight = %lf(micros)\n", (1.0e6*(double)(stop-start))/CLOCKS_PER_SEC);
  print_node_and_ancestors(rand()%size);
  printf("*********************\n");
  root->print();
  // for(auto leaf : leafnodes)
    // print_tree(leaf.first);
  printf("**************\n");
  // root->parent->print();
  delete(root);
  return 0;
}
