/**
 \file      hailstone2.cpp
 \brief     Computing minimum maximum hailstone number from range of collatz sequences
 \author    Thorsten Koch
 \version   1.0
 \date      22 Oct 2022
*/ 

#include <iostream>
#include <limits>
#include <cassert>

using namespace std;

int main(int const argc, char const* const* const argv)
{
   if (argc < 3)
   {
      cerr << "usage: " << argv[0] << " first last\n" 
         "       compute min max hailstone number for collatz sequences from first to last\n";

      return EXIT_FAILURE;
   }

   auto const first = atoi(argv[1]);
   auto const last  = atoi(argv[2]);

   assert(first >= 1 && last >= first);
   
   auto hail_min = numeric_limits<int>::max();
   
   for(auto i = first; i <= last; ++i)
   {
      auto seq_max = i;
      auto n       = i;
      
      while(n > 1)
      {
         if (n % 2 == 0) 
            n /= 2;
         else
            n = 3 * n + 1;

         if (n > seq_max)
            seq_max = n;
      }
      // Is our maximum a new minimum?
      if (seq_max < hail_min)
         hail_min = seq_max;
   }
   cout << hail_min << endl;
}
