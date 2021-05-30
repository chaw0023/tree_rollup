#include <iostream>
#include <vector>
#include <time.h>

#define MAX_WEIGHT 1000
#define SEED MAX_WEIGHT

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
  ~Node(){};
  std::vector<Node*> children;
  Node* parent;
  float added_weight;
  float get_weight();
  bool add_weight(float wt);
  std::string name;
  void add_random_children_to_node(int n);
  void add_children_to_node(int n, float init_wt);
  void add_children_to_node(int n, float init_wt[]) {}; //Unimplemented
  void rollup_weight(float wt);
  void print();
private:
  static int id_counter;
  int node_id;
  float self_weight;
};
