// TK25Oct2022

#include <cstdio>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <array>
#include <random>
#include <limits>
#include <vector>

using namespace std;

int const default_samples = 5;
int const min_start       = 1;
int const max_start       = 10'000;
int const cache_size      = 4096;

string exec(string const& cmd)
{
   array<char, 1024>                   buf;
   string                              result;
   unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd.c_str(), "r"), pclose);
    
   if (!pipe)
      throw runtime_error("popen() failed!");

   while (fgets(buf.data(), buf.size(), pipe.get()) != nullptr)
      result += buf.data();

   return result;
}

int hailstone(int first, int last)
{
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
      if (seq_max < hail_min)
         hail_min = seq_max;

      // We cannot improve after a power of 2.
      // https://stackoverflow.com/questions/600293/how-to-check-if-a-number-is-a-power-of-2
      if ((i & (i - 1)) == 0)
         break;
   }
   return hail_min;
}

//ex02_autograde [binary] [samples] [seed]
int main(int argc, char const* const* const argv)
{   
   random_device                 dev;
   string const                  executable((argc > 1) ? argv[1]       : "mmhailstone");
   auto const                    samples  = (argc > 2) ? atoi(argv[2]) : default_samples;
   auto const                    seed     = (argc > 3) ? atoi(argv[3]) : dev();
   mt19937                       rng(seed);
   uniform_int_distribution<int> dist(min_start, max_start); // distribution in range [1, 10000]

   cout << seed << endl;
   
   for(int i = 0; i < samples; ++i)
   {
      auto const n1  = dist(rng); 
      auto const n2  = dist(rng);
      auto const a   = min(n1, n2);
      auto const b   = max(n1, n2);
      auto const cmd = "./" + executable + " " + to_string(a) + " " + to_string(b);
      auto result    = stoi(exec(cmd));
      auto correct   = hailstone(a, b);

      cout << "Test " << a << " ... " << b << " -> " << result;
      
      if (result != correct)
      {
         cout << " but should be " << correct << endl;
         return EXIT_FAILURE;
      }
      cout << " ok\n";
   }
} 
