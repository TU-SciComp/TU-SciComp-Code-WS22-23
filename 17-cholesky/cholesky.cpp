/** 
 \file      cholesky.cpp
 \brief     Testdriver for Matrix/Cholesky template class
 \author    Thorsten Koch
 \version   1.0
 \date      15Dec2022
*/

#include <chrono>

#include "matrix.hpp"

using namespace std;

using quad = __float128;

constexpr unsigned default_seed = 20010313U; // arbitrary number


// This is a cheap hack to not implement op<< with snprintf etc.
inline std::ostream & operator<<(std::ostream & os, quad const& q) //xlint !e759 !e765
{
   return os << static_cast<long double>(q);
}


/** Template to generate tests for a particular floatingpoint type
 */
template <typename Value_T>
void test(
   string const& text,
   size_t const  beg,
   size_t const  end)
{
   using std::chrono::high_resolution_clock;
   using std::chrono::duration_cast;
   using std::chrono::duration;
   using std::chrono::milliseconds;

   assert(beg < end);
   
   cout << text << endl;
   cout << "   N         max-norm        two-norm time[ms]\n";
   for(size_t n = beg; n < end; ++n)
   {
      cout << setw(4) << n << " ";

      vector<Value_T> r(n, 1.0);
      Matrix<Value_T> a(n, default_seed); 

      auto const a_t = a.transpose();
      auto const aa  = a * a_t;
      
      auto const start_time_ms = high_resolution_clock::now();

      auto const l = aa.cholesky();
      auto const x = l.triangular_solve(r);

      duration<double, std::milli> const duration_ms = high_resolution_clock::now() - start_time_ms;

      auto const t = aa * x;

      cout << setw(16) << fixed << setprecision(12) << max_norm(r - t);
      cout << setw(16) << fixed << setprecision(12) << two_norm(r - t);
      cout << setw(9)  << fixed << setprecision(3)  << duration_ms.count() << endl; 
   }
   cout << endl;
}


/** Testdriver for Cholesky decomposition.
 *
 *  There are three ways to call this routine:
 * ./cholesky                     -> Will demonstracte decomposition according to example in the lecture slide
 * ./cholesky filename.mm         -> Will read in matrix filenanme.mm and solve for constant vector 2.
 * ./cholesky begin end precision -> For n = begin to end - 1 will generate a random matrix of size n and solve for constant vector 1.
 *                                   Precision: 1 = float, 2 = double, 3 = long double, 4 = quad precision.
 */
int main(int argc, char const* const* const argv)
{
   if (argc > 3)
   {
      int beg  = stoi(argv[1]);
      int end  = stoi(argv[2]);
      int prec = stoi(argv[3]);
      
      if (beg < 1 or end < beg or prec < 1 or prec > 4)
      {
         cerr << "usage: " << argv[0] << " N-begin N-end Precison[1-4]\n";
         return -1;
      }

      switch(prec) 
      {
      case 1:
         test<float>      ("Single Precision", beg, end); //lint !e732
         break;
      case 2: 
         test<double>     ("Double Precision", beg, end);  //lint !e732
         break;
      case 3: 
         test<long double>("Extended Precision", beg, end); //lint !e732
         break;
      case 4:
         test<quad>       ("Quad Precision", beg, end);  //lint !e732
         break;
      default:
         abort();
      }
   }
   else
   {
      Matrix<double> a{ 3, { 1,  2,  3,
                             2,  1,  2,
                             2,  1,  0 } };
      vector<double> b{ 3, 2, 9 };

      if (argc > 1)
      {
         a.read(argv[1]);
         b = vector<double>(a.size(), 2);
      }
      cout << "A=\n" << a << endl;
      cout << "b= " << b << endl;
      auto const a_t = a.transpose();
      cout << "A^t=\n" << a_t << endl;
      auto const aa = a * a_t;
      cout << "AA^t=\n" << aa << endl;
      auto const l = aa.cholesky();
      cout << "L=\n" << l << endl;
      auto const l_t = l.transpose();
      cout << "L^t=\n" << l_t << endl;
      auto const ll = l * l_t;   
      cout << "LL^t=\n" << ll << endl;
      auto const x = l.triangular_solve(b);
      cout << "x= " << x << endl;
      auto const y = aa * x;
      cout << "y= " << y << endl;
      cout << "Error max norm= " << setprecision(12) << max_norm(y - b) << endl;
      cout << "Error two norm= " << setprecision(12) << two_norm(y - b) << endl;
   }
}
