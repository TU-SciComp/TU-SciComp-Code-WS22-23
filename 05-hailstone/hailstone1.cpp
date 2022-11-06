/**
 \file      hailstone1.cpp
 \brief     Computing minimum maximum hailstone number from range of collatz sequences
 \author    Thorsten Koch
 \version   1.0
 \date      22 Oct 2022
*/ 

#include <iostream>
#include <limits>

using namespace std;

int main(int const argc [[maybe_unused]], char const* const* const argv)
{
   auto const   first    = atoi(argv[1]);
   auto const   last     = atoi(argv[2]);
   auto         hail_min = numeric_limits<int>::max();
   
   for(auto i = first; i <= last; ++i)
   {
      auto seq_max = i;

      for(auto n = i; n > 1; n = (n & 1) ? (3 * n + 1) : (n / 2))
         seq_max = max(seq_max, n);

      hail_min = min(hail_min, seq_max);
   }
   cout << hail_min << endl;
}
