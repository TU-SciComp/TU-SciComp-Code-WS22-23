/** 
 \file      testit.c
 \brief     testdriver for graph routines
 \author    Thorsten Koch
 \version   1.2
 \date      03Dez2022

 \details
 This program is an example to use the graph routines.
*/
#include <iostream>
#include <iomanip>
#include <chrono>
#include <iterator>
#include <algorithm>
#include <exception>

#include "graph.hpp"

int main(int const argc, char const* const* const argv)
{
   using namespace std;
   
   using std::chrono::high_resolution_clock;
   using std::chrono::duration_cast;
   using std::chrono::duration;
   using std::chrono::milliseconds;

   try
   {
      cout << "Graph routines test driver, Version 1.2.0, 03Dez2022\n";
   
      if (argc < 3)
      {
         cerr << "usage: " << argv[0] << " filename.gph start_node end_node" << endl;
         return -1;
      }
      Graph g;

      g.read(argv[1]);
   
      auto const arg1       = stoll(argv[2]);
      auto const arg2       = stoll(argv[3]);
      long long const nodes = g.node_count();
   
      if (arg1 < 1 or arg1 > nodes or arg2 < 1 or arg2 > nodes)
      {
         cerr << "Start or end node outside allowed range from 1 to " << g.node_count() << endl;
         return -2;
      }
      auto const start_node = static_cast<Graph::node_no_t>(arg1) - 1;
      auto const end_node   = static_cast<Graph::node_no_t>(arg2) - 1;

      vector<Graph::node_no_size_t> depth(g.node_count(), Graph::invalid_value);
      vector<Graph::node_no_t>      pred(g.node_count(), Graph::invalid_node);

      // Part I - BFS and Components
      {
         auto const start_time_ms = high_resolution_clock::now();

         cout << "Depth: "      << g.bfs(start_node, depth, pred) << endl;
         cout << "Components: " << g.component_count() << endl;

         duration<double, milli> const duration_ms = high_resolution_clock::now() - start_time_ms;
         cout << "Time: " << setprecision(0) << fixed << duration_ms.count() << " ms\n";
      }
      // Part II - Shortest path
      {
         vector<double> dist(g.node_count(), Graph::invalid_value);

         auto const start_time_ms = high_resolution_clock::now();

         Graph::node_no_size_t num_components;
         cout << "MST= " << g.kruskal(num_components) << " [" << num_components << "] ";

         g.dijkstra(start_node, dist, pred);

         cout << "SP= " << dist[end_node] << " Path:";

         vector<Graph::node_no_t> path;
         for(Graph::node_no_t i = end_node; pred[i] != Graph::invalid_node; i = pred[i])
            path.push_back(i + 1);
         path.push_back(start_node + 1);
      
         reverse(path.begin(), path.end());
      
         for(auto i : path)
            cout << " " << i;
      
         cout << endl;

         duration<double, milli> const duration_ms = high_resolution_clock::now() - start_time_ms;
         cout << "Time: " << setprecision(0) << fixed << duration_ms.count() << " ms\n";
      }
   }
   catch(std::exception const& e)
   {
      cerr << "Exception: " << e.what() << endl;
   }
}

