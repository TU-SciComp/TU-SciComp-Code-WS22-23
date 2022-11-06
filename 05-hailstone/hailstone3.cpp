/**
 \file      hailstone3.cpp
 \brief     Computing minimum maximum hailstone number from range of collatz sequences
 \author    Thorsten Koch
 \version   1.0
 \date      22 Oct 2022
*/ 

#include <iostream>
#include <limits>

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

   if (first < 1 || last < first)
   {
      cerr << "Error: violation of 0 < first=" << first << "< last=" << last << endl;
      return EXIT_FAILURE;
   }
   
   auto hail_min = numeric_limits<int>::max();
   
   for(auto i = first; i <= last; ++i)
   {
      auto seq_max = i;
      auto n       = i;
      
      while(n > 1)
      {
         if (n % 2)
         {
            n = 3 * n + 1;

            if (n > seq_max)
               seq_max = n;
         }
         n /= 2;
      }
      // Is our maximum a new minimum?
      if (seq_max < hail_min)
         hail_min = seq_max;

      /* We cannot improve after our starting number is power of 2,
       * because this will go directly down to 1. No number after can have a lower max.
       * https://stackoverflow.com/questions/600293/how-to-check-if-a-number-is-a-power-of-2
       * Here read about two-complement storage of numbers. note: 0 is wrongly giving as true,
       * however, this doesn't matter here.
       */
      if ((i & (i - 1)) == 0)
         break;
   }
   cout << hail_min << endl;
}
