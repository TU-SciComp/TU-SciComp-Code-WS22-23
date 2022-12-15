/**
 \file      knapsack.cpp
 \brief     Compute solution to knapsack problem by dynamic programming
 \author    Thorsten Koch
 \version   1.0
 \date      17 Nov 2022
*/ 

#include <iostream>
#include <vector>
#include <algorithm>
#include <iterator>
#include <numeric>
#include <cassert>

#include "knapsack.hpp"

/** Check consistency of the class
 *  This is precondition for all operations on the Knapsack class other than building a problem.
 *  Design decision: Weight limit == 0. Valid or not?
 */
bool Knapsack::is_valid() const
{
   if (profit_.size() != num_items_ or profit_.size() != weight_.size())
      return false;

   if (num_items_ == 0)
      return true;

   if (  *std::min_element(weight_.begin(), weight_.end()) < 1
      or *std::min_element(profit_.begin(), profit_.end()) < 1)
      return false;

   return true;
}

/** Read Knapsack problem from stream.
 *  The stream must be a text stream with the following format:
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
 */
void Knapsack::read(std::istream& inp)
{
   inp >> num_items_; // Read number of items

   profit_.resize(num_items_);
   weight_.resize(num_items_);
   
   // Read items
   for(auto i = 0U; i < num_items_; ++i)
   {
      unsigned int id;

      inp >> id >> profit_[i] >> weight_[i];

      assert(id == i + 1);
   }   
   inp >> weight_limit_; // Read weight_limit

   assert(is_valid());
}

/** Build a list with the items of the optimal knapsack.
 *  Using a dynamic program a table with the best profit for each number of items and weight limit is computed.
 *  See, e.g., https://en.wikipedia.org/wiki/Knapsack_problem#0-1_knapsack_problem.
 *  The selected items are written to #selected_items.
 *  All weights and profits of the problems are expected to be >= 1.
 *  Note that in case num_items = 0, 0 is returned.
 * \return profit of the optimal knapsack.
 */
unsigned int Knapsack::solve(std::vector<unsigned int>& selected_items) const
{
   assert(is_valid()); // precondition

   std::vector<std::vector<unsigned int>> best(num_items_ + 1, std::vector<unsigned int>(weight_limit_ + 1, 0));

   // find value of best selection
   for(auto item = 1U; item <= num_items_; ++item)
   {
      for(auto max_weight = 1U; max_weight <= weight_limit_; ++max_weight)
      {
         // if item too heavy for current weight limit, our best result is unchanged.
         // otherwise check if we can do better by adding the item.
         if (weight_[item - 1] > max_weight)
            best[item][max_weight] =          best[item - 1][max_weight];
         else
            best[item][max_weight] = std::max(best[item - 1][max_weight], best[item - 1][max_weight - weight_[item - 1]] + profit_[item - 1]);
      }
   }

   // reconstruct items used for best selection
   for(auto item = num_items_, max_weight = weight_limit_; item > 0; --item)
   {
      assert(best[item][max_weight] >= best[item - 1][max_weight]); // "<" not possible

      // if the best profit changed without the current item, then the current item was selected.
      if (best[item][max_weight] > best[item - 1][max_weight])
      {
         selected_items.push_back(item); // remeber we used the item 
         max_weight -= weight_[item - 1];
      }
   }
   // postconditions: check reslt
   assert(selected_items.size()           <= weight_.size());
   assert(best[num_items_][weight_limit_] == std::accumulate(selected_items.begin(), selected_items.end(), 0U, [this](unsigned int sum, unsigned int const& i){ return sum + this->profit_[i - 1]; }));
   assert(weight_limit_                   >= std::accumulate(selected_items.begin(), selected_items.end(), 0U, [this](unsigned int sum, unsigned int const& i){ return sum + this->weight_[i - 1]; }));
   
   return best[num_items_][weight_limit_];
}

