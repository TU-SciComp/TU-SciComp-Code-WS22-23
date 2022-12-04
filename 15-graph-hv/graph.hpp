/** 
 \file      graph.hpp
 \brief     Header for Graph class
 \author    Thorsten Koch
 \version   1.0
 \date      01Dec2022

 This is a modification and extension of the code found in Hougardy, Vygen: Algorithmic Mathematics, Springer, 2016
 See http://www.or.uni-bonn.de/~hougardy/alma/alma_eng.html
*/
#ifndef GRAPH_H_
#define GRAPH_H_

#include <vector>
#include <string>
#include <limits>
#include <cassert>

class Graph {
public:
   using node_no_t      = unsigned int;  // vertices are numbered 0, ... ,node_count() - 1
   using node_no_size_t = node_no_t;
   
   class Neighbor
   {
   private:
      node_no_t node_no_;          
      double    dist_;  

   public:
      Neighbor(node_no_t node_no, double dist) : node_no_(node_no), dist_(dist) {}

      double    dist()                        const { return dist_; };
      node_no_t node_no()                     const { return node_no_; };
      bool      operator==(Neighbor const& b) const { return node_no_ == b.node_no_; };
   };

   class Node
   {
   private:
      std::vector<Neighbor> neighbors_;

   public:
      void add_neighbor(node_no_t node_no, double dist)    { neighbors_.emplace_back(Neighbor(node_no, dist)); }; //lint !e534
      std::vector<Neighbor> const& adjacent_nodes() const  { return neighbors_; };
   };
   
private:
   std::vector<Node> nodes_;

   bool has_parallel_arcs() const;
   bool path_is_a_tree(node_no_t root, std::vector<node_no_t> const& pred, bool check_is_spanning = true) const;
   bool is_shortest_path_tree(node_no_t root, std::vector<double> const& dist, std::vector<node_no_t> const& pred) const;

   
public:
   Graph()                        = default;
   Graph(const Graph&)            = default; 
   Graph(Graph&&)                 = default; 
   // Graph& operator=(const Graph&) = default;
   // Graph& operator=(Graph&&)      = default;
   ~Graph()                       = default;   

   void           read(std::string const& filename);

   node_no_size_t node_count()             const { return static_cast<node_no_size_t>(nodes_.size()); }; 
   Node const&    get_node(node_no_t node) const { return nodes_[node]; };
   void           info(bool show_all = false) const;
   node_no_size_t bfs(node_no_t start, std::vector<node_no_size_t>& depth, std::vector<node_no_t>& pred) const;
   void           dijkstra(node_no_t start, std::vector<double>& dist, std::vector<node_no_t>& pred, bool initialize = true) const;
   double         kruskal(node_no_size_t& num_components) const;
   node_no_size_t component_count() const;

   static constexpr node_no_t      invalid_node  = std::numeric_limits<node_no_t>::max();
   static constexpr node_no_size_t invalid_value = std::numeric_limits<node_no_size_t>::max();
   static constexpr double         infinite_dist = std::numeric_limits<double>::max(); 
};

#endif // GRAPH_H_

