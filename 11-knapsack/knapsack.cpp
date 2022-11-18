/**
 \file      knapsack.cpp
 \brief     Compute solution to knapsack problem by dynamic programming
 \author    Thorsten Koch
 \version   1.0
 \date      17 Nov 2022

 g++ -std=c++17 -Wall -Wextra -Ofast -o knapsack knapsack.cpp
*/ 

#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <iterator>
#include <numeric>
#include <chrono>
#include <cassert>

using std::vector;

/** Build a list with the items of the optimal knapsack.
 *  Using a dynamic program a table with the best profit for each number of items and weight limit is computed.
 *  See, e.g., https://en.wikipedia.org/wiki/Knapsack_problem#0-1_knapsack_problem.
 *  The selected items are written to #selected_items.
 *  All weights and profits of the problems are expected to be >= 1.
 * \return profit of the optimal knapsack.
 */
int knapsack(
   vector<int> const& weight,          ///< vector holding the weights of the items.
   vector<int> const& profit,          ///< vector holding the profits of the items.
   int         const  weight_limit,    ///< weight limit of the knapsack.
   vector<int>&       selected_items)  ///< vector to which the selected items are added.
{
   assert(*std::min_element(weight.begin(), weight.end()) >= 1);
   assert(*std::min_element(profit.begin(), profit.end()) >= 1);
   assert(weight_limit  >= 1);
   assert(weight.size() == profit.size());

   int const           items = profit.size();
   vector<vector<int>> best(items + 1, vector<int>(weight_limit + 1, 0));

   for(auto item = 1; item <= items; ++item)
   {
      for(auto max_weight = 1; max_weight <= weight_limit; ++max_weight)
      {
         // if item too heavy for current weight limit, our best result is unchanged.
         // otherwise check if we can do better by adding the item.
         if (weight[item - 1] > max_weight)
            best[item][max_weight] =          best[item - 1][max_weight];
         else
            best[item][max_weight] = std::max(best[item - 1][max_weight], best[item - 1][max_weight - weight[item - 1]] + profit[item - 1]);
      }
   }

   for(auto item = items, max_weight = weight_limit; item > 0; --item)
   {
      assert(best[item][max_weight] >= best[item - 1][max_weight]); // "<" not possible

      // if the best profit changed without the current item, then the current item was selected.
      if (best[item][max_weight] > best[item - 1][max_weight])
      {
         selected_items.push_back(item); // remeber we used the item 
         max_weight -= weight[item - 1];
      }
   }
   assert(selected_items.size()     <= weight.size());
   assert(best[items][weight_limit] == std::accumulate(selected_items.begin(), selected_items.end(), 0, [&profit](int sum, int const& i){ return sum + profit[i - 1]; }));
   assert(weight_limit              >= std::accumulate(selected_items.begin(), selected_items.end(), 0, [&weight](int sum, int const& i){ return sum + weight[i - 1]; }));
   
   return best[items][weight_limit];
}

/** Knapsack problem solver.
 *  Read from standard input.
 *  Expects a file of the following format:
 *
 *  n
 *  1 profit1 weight1
 *  2 profit2 weight2
 *  ...
 *  n profit_n weight_n
 *  weight_limit
 *
 *  where n is the number of items to chose from.
 *  All numbers are integers.
 *
 * $ ./knapsack <instances/instances_n100_2.csv
 */
int main()
{
   using std::cin;
   using std::cout;
   using std::endl;
   using std::setprecision;
   using std::fixed;
   using std::chrono::high_resolution_clock;
   using std::chrono::duration_cast;
   using std::chrono::duration;
   using std::chrono::milliseconds;

   // Read number of items
   int n;
   cin >> n; 
   assert(n >= 1);
   
   vector<int> profit(n);
   vector<int> weight(n);

   // Read items
   for(auto i = 0; i < n; ++i)
   {
      int id;

      cin >> id >> profit[i] >> weight[i];

      assert(id == i + 1);
      assert(profit[i] > 0);
      assert(weight[i] > 0);
   }
   // Read weight_limit
   int weight_limit;
   cin >> weight_limit;
   assert(weight_limit > 0);

   vector<int> selected_items;

   // Compute the best selection of items
   auto                         const start_time_ms = high_resolution_clock::now();
   auto                         const optval        = knapsack(weight, profit, weight_limit, selected_items);
   duration<double, std::milli> const duration_ms   = high_resolution_clock::now() - start_time_ms;

   // Output the result
   cout << "Items:";
   for (auto i: selected_items)
      cout << ' ' << i; 
   cout << endl;   

   cout << "Total: " << optval << endl;
   cout << "Time : " << setprecision(0) << fixed << duration_ms.count() << " ms\n";
}
