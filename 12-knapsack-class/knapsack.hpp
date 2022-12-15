/**
 \file      knapsack.h
 \brief     Compute solution to knapsack problem by dynamic programming
 \author    Thorsten Koch
 \version   1.0
 \date      17 Nov 2022
*/ 
#ifndef KNAPSACK_H_
#define KNAPSACK_H_

class Knapsack
{
 private:
   unsigned int              num_items_;       ///< number of items to chose from.
   std::vector<unsigned int> weight_;          ///< vector holding the weights of the items.
   std::vector<unsigned int> profit_;          ///< vector holding the profits of the items.
   unsigned int              weight_limit_;    ///< weight limit of the knapsack.

 public:
   bool         is_valid() const;
   void         read(std::istream& inp);
   unsigned int solve(std::vector<unsigned int>& selected_items) const;

   Knapsack() : num_items_(0), weight_(0), profit_(0), weight_limit_(0) { assert(is_valid()); };

   Knapsack(Knapsack const&)            = default; 
   Knapsack(Knapsack&&)                 = default; 
   Knapsack& operator=(Knapsack const&) = default;
   Knapsack& operator=(Knapsack&&)      = default;
   ~Knapsack()                          = default;   
};

#endif // KNAPSACK_H_
