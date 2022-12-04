/** 
 \file      dijkstra_bh.c
 \brief     Dijkstra Algorithm 
 \author    Thorsten Koch
 \version   1.0
 \date      01Dec2022
 \copyright Copyright (C) 2022 by Thorsten Koch <koch@zib.de>,
            licensd under GPL version 3 or later
 \details

 Performs a label setting algorithm to compute the shortest paths in a graph.
  
 Beginning at starting point the reached nodes are stored in a priority queue.
 This allows to determine efficiently which node has currently the
 shortest label. Once the queue is empty we are finished.
*/
 
#include <queue>
#include <algorithm>
#include <iterator>

#include "graph.hpp"

using std::vector;

/** Check whether #dist and #pred constitute a shortests path tree.
 *  Implementation only works for undirected graphs.
 */
bool Graph::is_shortest_path_tree(
   node_no_t         const  root,
   vector<double>    const& dist,
   vector<node_no_t> const& pred) const
{
   if (dist[root] != 0 or pred[root] != invalid_node)
      return false;
   
   for(node_no_t head = 0; head < node_count(); head++)
   {
      /* The following has not to be true for
       * not reacheed nodes -> dist == infinite_dist and the starting node
       */
      if (dist[head] == infinite_dist or head == root) //lint !e777
         continue;
      
      /* If dist[n] has been set to a value < infinite_dist, there has to
       * be a predecessor p and an edge e with dist[n] = dist[p] + cost[e].         
       */
      bool found_pred = false;

      for(auto neighbor : get_node(head).adjacent_nodes())
      {
         node_no_t const tail = neighbor.node_no();
         double    const cost = neighbor.dist();
         
         // The tail must have been reached.
         if (dist[tail] == infinite_dist) //lint !e777
            return false;
         
         // There should be no shorter path.
         if (dist[tail] + cost < dist[head])
            return false;

         // We should encounter the predecessor excatly once.
         if (tail == pred[head])
         {
            if (found_pred)
               return false;
            
            found_pred = true;
         }
      }      
      if (not found_pred)
         return false;
   }
   return true;
}

/** Entry into the priority queue.
 */
struct Entry
{
   Graph::node_no_t node_no_;
   double           dist_;

   Entry(Graph::node_no_t const node_no, double const dist) : node_no_(node_no), dist_(dist) { };

   bool operator>(Entry const& b) const { return dist_ > b.dist_; };
};

/** Dijkstras Algorithm.
 */
void Graph::dijkstra(
   node_no_t const    start,
   vector<double>&    dist,
   vector<node_no_t>& pred,
   bool const         initialize) const
{
   using std::fill;
   using std::priority_queue;
   using std::greater;
   
   assert(start       <  node_count());
   assert(dist.size() == node_count());
   assert(pred.size() == node_count());

   // Shall we initialize distances and predecessors?
   if (initialize)  
   {
      fill(dist.begin(), dist.end(), infinite_dist);
      fill(pred.begin(), pred.end(), invalid_node);
   }
   dist[start] = 0;

   priority_queue<Entry, vector<Entry>, greater<Entry>> queue;

   // Put starting node into the queue.
   queue.emplace(Entry(start, 0.0));
   
   // Once the queue is empty, we are finished.
   while(not queue.empty())
   {
      // Which node are we processing next?
      Entry    const entry = queue.top();
      node_no_t const tail = entry.node_no_;

      queue.pop();

      // Already done node? Ignore!
      if (entry.dist_ > dist[tail])
         continue;

      /* Look through neighbors and correct all nodes where we can go to.
       */
      for(auto neighbor : get_node(tail).adjacent_nodes())
      {
         node_no_t const head   = neighbor.node_no();
         double    const weight = neighbor.dist() + dist[tail];

         assert(neighbor.dist() >= 0);
         
         /* Is it shorter to take this way?
          * Since we initialised with the distance with #infinite_dist
          * therefore this is true for all so far unvisited nodes.
          */
         if (dist[head] > weight)
         {
            // we are shorter, so record the predecessor and correct the distance.
            pred[head] = tail;
            dist[head] = weight;

            queue.emplace(Entry(head, weight));
         }
      }
   }
   // Postcondition
   assert(path_is_a_tree(start, pred, false));
   assert(is_shortest_path_tree(start, dist, pred));
}

/*
 * This code is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public License
 * as published by the Free Software Foundation; either version 3
 * of the License, or (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
