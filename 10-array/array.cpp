// TK10Nov2022
// g++ -std=c++17 -Wall -Wextra -Ofast array.cpp -o array

#include <iostream>
#include <iomanip>
#include <chrono>
#include <vector>
#include <cassert>

auto constexpr test_size = 100'000'000;

int main()
{
   using std::cout;
   using std::endl;
   using std::setprecision;
   using std::fixed;
   using std::chrono::high_resolution_clock;
   using std::chrono::duration_cast;
   using std::chrono::duration;
   using std::chrono::milliseconds;
   using std::vector;
   
   vector<double> x(test_size);

   double* const y = new double[test_size];
   
   assert(x.size() == test_size);
   
   for(auto i = 0; i < test_size; ++i)
   {
      x[i] = i + 1.0;
      y[i] = i + 1.0;
   }
      
   // 0 ======================================================================
   auto sum           = 0.0;
   auto start_time_ms = high_resolution_clock::now();

   for(auto i =  0; i < test_size; i++)
      sum += y[i];

   // Getting number of milliseconds as a double.
   duration<double, std::milli> duration_ms = high_resolution_clock::now() - start_time_ms;
   cout << "0 time= " << setprecision(0) << fixed << duration_ms.count() << " ms, sum= " << sum << endl; 

   // 1 ======================================================================
   sum           = 0.0;
   start_time_ms = high_resolution_clock::now();

   for(auto i =  0; i < test_size; i++)
      sum += x[i];

   duration_ms = high_resolution_clock::now() - start_time_ms;
   cout << "1 time= " << setprecision(0) << fixed << duration_ms.count() << " ms, sum= " << sum << endl; 

   // 2 ======================================================================
   sum           = 0.0; 
   start_time_ms = high_resolution_clock::now();

   for(auto k = 0; k < 100; ++k)
      for(auto i =  0; i < test_size / 100; ++i)
         sum += x[k * (test_size / 100) + i];

   duration_ms = high_resolution_clock::now() - start_time_ms;
   cout << "2 time= " << setprecision(0) << fixed << duration_ms.count() << " ms, sum= " << sum << endl; 

   // 3 ======================================================================
   sum           = 0.0; 
   start_time_ms = high_resolution_clock::now();
   
   for(auto i =  0; i < test_size / 100; ++i)
      for(auto k = 0; k < 100; ++k)
         sum += x[k * (test_size / 100) + i];
   
   duration_ms = high_resolution_clock::now() - start_time_ms;
   cout << "3 time= " << setprecision(0) << fixed << duration_ms.count() << " ms, sum= " << sum << endl; 

   // 4 ======================================================================
   sum           = 0.0; 
   start_time_ms = high_resolution_clock::now();
   
   for(auto i =  0; i < test_size / 1000; ++i)
      for(auto k = 0; k < 1000; ++k)
         sum += x[k * (test_size / 1000) + i];

   duration_ms = high_resolution_clock::now() - start_time_ms;
   cout << "4 time= " << setprecision(0) << fixed << duration_ms.count() << " ms, sum= " << sum << endl; 

   // 5 ======================================================================
   sum           = 0.0; 
   start_time_ms = high_resolution_clock::now();
   
   for(auto i =  0; i < test_size / 10000; ++i)
      for(auto k = 0; k < 10000; ++k)
         sum += x[k * (test_size / 10000) + i];

   duration_ms = high_resolution_clock::now() - start_time_ms;
   cout << "5 time= " << setprecision(0) << fixed << duration_ms.count() << " ms, sum= " << sum << endl; 

   // 6 ======================================================================
   sum           = 0.0; 
   start_time_ms = high_resolution_clock::now();
   
   for(auto i = 0; i < test_size / 100000; ++i)
      for(auto k = 0; k < 100000; ++k)
         sum += x[k * (test_size / 100000) + i];

   duration_ms = high_resolution_clock::now() - start_time_ms;
   cout << "6 time= " << setprecision(0) << fixed << duration_ms.count() << " ms, sum= " << sum << endl; 

   // 7 ======================================================================
   sum           = 0.0; 
   start_time_ms = high_resolution_clock::now();
   
   for(auto i =  test_size / 100000 - 1; i >= 0; --i)
      for(auto k = 0; k < 100000; ++k)
         sum += x[k * (test_size / 100000) + i];

   duration_ms = high_resolution_clock::now() - start_time_ms;
   cout << "7 time= " << setprecision(0) << fixed << duration_ms.count() << " ms, sum= " << sum << endl; 

   // 8 ======================================================================
   sum           = 0.0; 
   start_time_ms = high_resolution_clock::now();
   
   for(auto i =  test_size / 100000 - 1; i >= 0; --i)
      for(auto k = 100000 - 1; k >= 0; --k)
         sum += x[k * (test_size / 100000) + i];

   duration_ms = high_resolution_clock::now() - start_time_ms;
   cout << "8 time= " << setprecision(0) << fixed << duration_ms.count() << " ms, sum= " << sum << endl; 

   delete y;
}

