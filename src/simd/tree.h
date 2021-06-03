#include <iostream>
#include <vector>
#include <time.h>
#include <unordered_map>

#define MAX_WEIGHT 2
#define SEED MAX_WEIGHT

class Node;

std::unordered_map<Node*, int> leafnodes;
std::vector<Node*> leaves_vector;
std::vector<std::vector<Node*>> ancestor_of_leaves;
std::vector<std::vector<float*>> ancestor_weights_of_leaves;

struct Leaf{
  std::vector<std::vector<Node*>> ancestors_deposited_wt;
  std::vector<Node*> ancestors;
};

class Node
{
  Node():parent(NULL), added_weight(0){node_id = id_counter++;};
public:
  Node(float wt, std::string name_):
          parent(NULL),
          added_weight(0)
          {
            node_id = id_counter++;
            self_weight=wt;
            name = name_;
            // printf("Added node with id = %d\t, weight = %.2f\n", node_id, wt);
          };
  ~Node(){printf("Destroying tree with %i elements!!\n", id_counter);};
  std::vector<Node*> children;
  std::vector<Node*> list_of_leaves_mapped;
  std::vector<float> deposited_weight;
  Node* parent;
  float added_weight;
  float get_weight();
  int get_id();
  bool add_weight(float wt);
  std::string name;
  void add_random_children_to_node(int n = 1);
  Node* add_random_child_to_node();
  void add_children_to_node(int n, float init_wt);
  void add_children_to_node(int n, float init_wt[]) {}; //Unimplemented
  void rollup_weight(float wt);
  void print();
private:
  static int id_counter;
  int node_id;
  float self_weight;
};
