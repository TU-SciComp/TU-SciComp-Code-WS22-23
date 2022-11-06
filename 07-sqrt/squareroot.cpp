/* TK03Nov2022
 *
 * g++ -std=c++17 -Wall -Wextra -O squareroot.cpp -o squareroot  
*/
#include <iostream>
#include <iomanip>
#include <cassert>
#include <cmath>

using namespace std;

// Compute squareroot of x
double squareroot(double x)
{
   assert(x >= 0); // precondition

   auto const max_error = 1e-15;
   auto       result    = 0.5 * x;
   auto       residual  = fabs(result * result - x);
      
   while(residual > max_error)
   {      
      result   = 0.5 * (result + x / result);

      assert(fabs(result * result - x) < residual); // Invariant
      
      residual = fabs(result * result - x);
      
      cout << setprecision(15) << result << endl;
   }
   assert(fabs(result * result - x) <= max_error); // Postcondition

   return result;
}


int main(int const argc, char const* const* const argv)
{
   assert(argc > 1);
   
   auto const x = atof(argv[1]);
   
   cout << setprecision(15) << squareroot(x) << endl;
}

