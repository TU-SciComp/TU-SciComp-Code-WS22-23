/** 
 \file      graph.hpp
 \brief     Implementation for Graph class
 \author    Thorsten Koch
 \version   1.0
 \date      01Dec2022

 This is a modification and extension of the code found in Hougardy, Vygen: Algorithmic Mathematics, Springer, 2016
 See http://www.or.uni-bonn.de/~hougardy/alma/alma_eng.html
*/

#include <iostream>
#include <stack>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <iterator>

#include "graph.hpp"

using std::vector;

/** Print the graph info.
 */
void Graph::info(bool const show_all) const
{
   using std::cout;
   
   cout << "Graph with " << node_count() << " vertices";

   if (show_all)
      cout << "\n";
   
   size_t edge_count = 0;
   
   for (node_no_t node_no = 0; node_no < node_count(); ++node_no) 
   {
      if (show_all)
         cout << "Incident edges to vertex " << node_no << ":\n";

      for(auto neighbor: get_node(node_no).adjacent_nodes())
      {
         if (show_all)
            cout << node_no << " - " << neighbor.node_no() << " dist= " << neighbor.dist() << "\n";

         edge_count++;
      }
   }
   assert(edge_count % 2 == 0);

   cout << " and " << edge_count / 2 << " edges.\n";
}

/** Read a Graph froma file.
 */
void Graph::read(std::string const& filename)
{
   using std::ifstream;
   using std::string;
   using std::getline;
   using std::to_string;
   using std::istringstream;
   using std::runtime_error;
   using std::cout;
   using std::endl;
   
   constexpr auto max_size = std::numeric_limits<std::streamsize>::max();
   
   ifstream file(filename); 
   
   if (not file) 
      throw runtime_error("Cannot open file: " + filename);

   cout << "Reading " << filename << endl;
   
   long long nodes;
   long long edges;
   long long count   = 0;
   size_t    line_no = 1;
   
   if ((file >> nodes >> edges).fail() or nodes < 1 or edges < 0)
      throw runtime_error("Line:" + to_string(line_no) + " node or edge count missing or illegal");

   if (nodes >= std::numeric_limits<node_no_size_t>::max())
      throw runtime_error("Line:" + to_string(line_no) + " node count too big for node_no_size_t");
      
   nodes_.resize(nodes); //lint !e732

   file.ignore(max_size, '\n'); // skip the rest of the line 
   
   for(string line; getline(file, line); ++line_no) //lint !e440 !e443
   {
      istringstream iss(line);
      long long head;
      long long tail;
      
      if ((iss >> tail >> head).fail())
         throw runtime_error("Line " + to_string(line_no) + " syntax error: " + line);

      if (tail < 1 || tail > nodes || head < 1 || head > nodes) 
         throw runtime_error("Line " + to_string(line_no) + " node number outside 1.." + to_string(nodes));

      if (tail == head) 
         throw runtime_error("Line " + to_string(line_no) + " loops not allowed");

      if (count >= edges)
         throw runtime_error("Line " + to_string(line_no) + " too many edges");

      double dist;
      if ((iss >> dist).fail())
         throw runtime_error("Line " + to_string(line_no) + " syntax error: " + line);

      // Check what happens if -7

      // Node numbers in the file start with 1, internally with 0
      tail--;
      head--;
      
      nodes_[tail].add_neighbor(static_cast<node_no_t>(head), dist); //lint !e732
      nodes_[head].add_neighbor(static_cast<node_no_t>(tail), dist); //lint !e732

      count++;
   }
   if (edges != count)
      throw runtime_error("Line " + to_string(line_no) + " unexpected EOF: "
         + to_string(edges) + " edges expected, got " + to_string(count));

   if (has_parallel_arcs())
      throw runtime_error("Error: Graph has parallel edges");      

   info();
}

/** Check whether the graph has parallel edges.
 */
bool Graph::has_parallel_arcs() const
{
   for(auto node : nodes_)
   {
      vector<Neighbor> neighbors(node.adjacent_nodes());

      std::sort(neighbors.begin(), neighbors.end(), 
         [](Neighbor const& a, Neighbor const& b) -> bool { return a.node_no() > b.node_no(); });

      if (std::adjacent_find(neighbors.begin(), neighbors.end()) != neighbors.end())
         return true;
   }
   return false;
}

/** Check #pred defines a tree.
 *  The algorithm does a DFS using a stack to check.
 */
bool Graph::path_is_a_tree(
   node_no_t         const  root,
   vector<node_no_t> const& pred,
   bool              const  check_is_spanning) const
{
   //   assert(not has_parallel_arcs());

   node_no_size_t  const nodes = node_count();
   std::stack<node_no_t> stack;
   vector<bool>          visited(nodes, false);

   visited[root] = true;
   stack.push(root);

   while(!stack.empty())
   {
      node_no_t tail = stack.top();

      stack.pop();

      /* Check all outgoing edges, find the predecessor, and put on the stack.
       */
      for(auto neighbor : get_node(tail).adjacent_nodes())
      {
         node_no_t const head = neighbor.node_no();

         if (pred[head] == tail)
         {
            if (visited[head])
               return false;

            visited[head] = true;
            stack.push(head);
         }
      }
   }
   if (check_is_spanning)
      return all_of(visited.begin(), visited.end(), [](bool v) { return v; });
      
   return true;
}
  
/** Count the connected components of the graph.
 *  We run BFS until we have covered the whole graph.
 */
Graph::node_no_size_t Graph::component_count() const
{
   node_no_size_t const nodes = node_count();
   
   vector<node_no_size_t> depth(nodes, invalid_value);
   vector<node_no_t>      pred(nodes, invalid_node);

   node_no_size_t count = 0;
   
   for(node_no_t n = 0; n < nodes; n++)
   {
      if (depth[n] == invalid_value)
      {
         bfs(n, depth, pred); //lint !e534
         count++;
      }
   }
   assert(all_of(depth.begin(), depth.end(), [](node_no_size_t d) { return d != invalid_value; }));

   return count;
};


