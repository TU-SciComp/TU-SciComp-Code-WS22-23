/**
 \file      knapsack.cpp
 \brief     Compute solution to knapsack problem by dynamic programming
 \author    Thorsten Koch
 \version   1.0
 \date      17 Nov 2022

 g++ -std=c++17 -Wall -Wextra -Ofast -o knapsack2 knapsack2.cpp
*/ 

#include <iostream>
#include <fstream>
#include <iomanip>
#include <vector>
#include <chrono>
#include <cassert>

#include "knapsack.hpp"

/** Knapsack problem solver.
 *  Read from standard input or a file.
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
int main(int const argc, char const* const* const argv)
{
   using std::vector;
   using std::ifstream, std::cin, std::cout, std::endl, std::setprecision, std::fixed;
   using std::chrono::high_resolution_clock, std::chrono::duration_cast, std::chrono::duration, std::chrono::milliseconds;

   Knapsack knapsack;

   if (argc < 2)
      knapsack.read(cin);
   else
   {
      ifstream input_file(argv[1]);
      knapsack.read(input_file);
   }
   vector<unsigned int> selected_items;

   // Compute the best selection of items
   auto                         const start_time_ms = high_resolution_clock::now();
   auto                         const optval        = knapsack.solve(selected_items);
   duration<double, std::milli> const duration_ms   = high_resolution_clock::now() - start_time_ms;

   // Output the result
   cout << "Items:";
   for (auto i: selected_items)
      cout << ' ' << i; 
   cout << endl;   

   cout << "Total: " << optval << endl;
   cout << "Time : " << setprecision(0) << fixed << duration_ms.count() << " ms\n";
}
