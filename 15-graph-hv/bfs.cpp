/**
 \file   bfs.cpp
 \brief  Breath-First-Search of a graph
 \author Thorsten Koch
 \version   1.0
 \date      01Dec2022
 */
#include <queue>

#include "graph.hpp"

using std::vector;
using std::queue;

/** Breath-First-Search.
 *
 *  \return Maximal reached depth, i.e., max distance a node can have from the starting node.
 */
Graph::node_no_size_t Graph::bfs(
   node_no_t const         start,
   vector<node_no_size_t>& depth,
   vector<node_no_t>&      pred) const
{
   assert(start        <  node_count());
   assert(depth.size() == node_count());
   assert(pred.size()  == node_count());

   depth[start] = 0;

   queue<node_no_t> queue;
   
   queue.push(start);

   node_no_size_t dmax = 0;

   while(!queue.empty())
   {
      node_no_t const tail = queue.front();

      queue.pop();

      for(auto neighbor: get_node(tail).adjacent_nodes())
      {
         node_no_t const head = neighbor.node_no();

         assert(depth[head] == invalid_value or depth[head] <= depth[tail] + 1);

         // node not yet visited?
         if (depth[head] == invalid_value)
         {
            queue.push(head);
            dmax        = depth[tail] + 1;
            depth[head] = dmax;
            pred[head]  = tail;
            
            assert(dmax <= node_count());
         }
      }
   }
   assert(queue.empty());
   
   // Postcondition
   assert(path_is_a_tree(start, pred, false));
   
   return dmax;
}




