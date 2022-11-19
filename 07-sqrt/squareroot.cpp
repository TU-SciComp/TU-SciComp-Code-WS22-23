/* TK03Nov2022
 *
 * g++ -std=c++17 -Wall -Wextra -O squareroot.cpp -o squareroot  
 */
#include <iostream>
#include <iomanip>
#include <cassert>
#include <cmath>

using namespace std;

auto constexpr max_error = 1e-15;

// Compute squareroot of x
// See Warren: Hacker's Delight, Appendix B
double squareroot_newton(double x)
{
   assert(x >= 1); // precondition

   auto result   = 0.5 * x;
   auto residual = fabs(result * result - x);
      
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

// Compute squareroot of x
// See Hugardy, Vygen: Algorithmic Mathematics, Chapter 5
double squareroot_binsearch(double x)
{
   assert(x >= 1); // precondition

   auto lower    = 1.0;
   auto upper    = x;
   auto mid      = (lower + upper) / 2.0;
   auto residual = fabs(mid * mid - x);
   
   while(residual > max_error)
   {
      cout << setprecision(16) << mid << endl;
         
      if ((mid * mid) < x)
         lower = mid;
      else
         upper = mid;

      mid      = (lower + upper) / 2.0;
      residual = fabs(mid * mid - x);
   }
   assert(fabs(mid * mid - x) <= max_error); // Postcondition

   return mid;
}
   
int main(int const argc, char const* const* const argv)
{
   assert(argc > 1);
   
   auto const x = atof(argv[1]);
   
   cout << setprecision(16) << squareroot_binsearch(x) << endl;

   cout << setprecision(16) << squareroot_newton(x) << endl;

}

