/** 
 \file      kruskal.cpp
 \brief     Kruskals Algorithm 
 \author    Thorsten Koch
 \version   1.0
 \date      04Dec2022
 \details

 Performs Kruskals algorithm for egnerating a minimal spanning forrest.
 Sorts all the edges. Starts with the shortest edge and adds it to the tree
 whenever this does not lead to a cycle.
 Since there is no starting point, this works alos for a non connected graph.
*/
 
#include <queue>
#include <unordered_set>
#include <algorithm>
#include <iterator>

#include "graph.hpp"

using std::vector;
using std::unordered_set;

/** Entry into the priority queue.
 */
struct Edge
{
   Graph::node_no_t tail_;
   Graph::node_no_t head_;
   double           dist_;

   Edge(Graph::node_no_t const tail, Graph::node_no_t const head, double const dist)
      : tail_(tail), head_(head), dist_(dist) { };
};

/** Kruskals Algorithm.
 */
double Graph::kruskal(node_no_size_t& num_components) const
{
   using std::unordered_set;
   
   vector<Edge>                      edges;
   vector<unordered_set<node_no_t>>  component(node_count());
   vector<unordered_set<node_no_t>*> component_of_node(node_count());

   for(node_no_t node_no = 0; node_no < node_count(); ++node_no) 
   {
      component        [node_no].insert(node_no);
      component_of_node[node_no] = &component[node_no];

      for(auto neighbor: get_node(node_no).adjacent_nodes())
      {
         node_no_t const head = neighbor.node_no();

         if (head > node_no)
            edges.emplace_back(Edge(node_no, head, neighbor.dist()));
      }
   }
   // Sort edges in by ascending distance
   sort(edges.begin(), edges.end(), [](Edge const& a, Edge const& b) -> bool { return a.dist_ < b.dist_; });

   double tree_length = 0.0;

   // Takes edges, starting from the smalles
   for(auto edge : edges)
   {
      unordered_set<node_no_t>* const component_tail = component_of_node[edge.tail_];
      unordered_set<node_no_t>* const component_head = component_of_node[edge.head_];

      // Both end of the edge in different components? 
      if (component_head != component_tail)
      {
         // Update thepointers and merge the two components
         for(auto node_no : *component_head)
            component_of_node[node_no] = component_tail;
            
         component_tail->merge(*component_head);
         
         tree_length += edge.dist_;
      }
   }
   // How many components are left?
   num_components = 0;
   
   for(auto set : component)
      if (set.size() > 0)
         num_components++;

   assert(num_components == component_count());
   
   return tree_length;
}

   
