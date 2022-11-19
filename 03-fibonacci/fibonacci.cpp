/**
 \file      fibonacci.cpp
 \brief     Examples for computing the fibonacci number
 \author    Thorsten Koch
 \version   1.0
 \date      18Oct2022
*/ 

#include <iostream>
#include <type_traits>
#include <cmath>
#include <numbers>

using namespace std;

/* example of unreadable code from Pete Goodliffe, Code Craft, page 59
 * additionally, it also gives a wrong result :-)
 */
unsigned int fval(int i)
{
   int ret = 2;

   for(int n1 = 1, n2 = 1, i2 = i - 3; i2 >= 0; --i2)
   {
      n1  = n2; n2  = ret; ret = n2 + n1;
   }
   return (i < 2) ? 1 : ret;
}


unsigned int fib_recursive1(unsigned int const n)
{
   if (n <= 2)
      return 1;
  
   return fib_recursive1(n - 1) + fib_recursive1(n - 2);   
}


/* Same in one line. Less readable, and under some circumstances optimized worse
 */
unsigned int fib_recursive2(unsigned int const n)
{
   return (n <= 2) ? 1 : (fib_recursive2(n - 1) + fib_recursive2(n - 2));   
}


unsigned int fib_iterative1(unsigned int n)
{
   unsigned int a = 0;
   unsigned int b = 1;

   while(n > 0)
   {
      auto t = a + b;
      a      = b;
      b      = t;
      --n;
   }
   return a;
}

/* One can do without the temorary variable t
 * and usung a for loop
 */
unsigned int fib_iterative2(unsigned int n)
{
   unsigned int a = 0;
   unsigned int b = 1;

   for(; n > 0; --n)
   {
      b += a;
      a  = b - a;
   }
   return a;
}

/* Maybe these variable names are easier to understand
 */
unsigned int fib_iterative3(unsigned int n)
{
   if (n <= 2)
      return 1;

   unsigned int prev_prev = 1;
   unsigned int prev      = 1;
   unsigned int result    = 2;

   for(unsigned i = 3; i < n; ++i)
   {
      prev_prev = prev;
      prev      = result;
      result    = prev + prev_prev;
   }
   return result;
}


/* Or naming this way and counting in the other direction
 */
unsigned int fib_iterative4(unsigned int n)
{
   if (n <= 2)
      return 1;

   unsigned int a_2 = 1;
   unsigned int a_1 = 1;
   unsigned int a   = 2;

   for(n -= 3; n > 0; --n)   
   {
      a_2 = a_1;
      a_1 = a;
      a   = a_2 + a_1;
   }
   return a;
}

/* Without the if in the beginning
 */
unsigned int fib_iterative5(int n)
{
   unsigned int a_2 = 1;
   unsigned int a_1 = 1;
   unsigned int a   = 2;

   for(n -= 2; n > 0; --n)   
   {
      a_2 = a_1;
      a_1 = a;
      a   = a_2 + a_1;
   }
   return a_1;
}

/* Actually, there exists and instruction on x86 for this.
 * WARNING: will only compile on x86 architecture (i486 upwards)
 */
unsigned int fib_iterative_asm(unsigned int n)
{
   unsigned int a = 0;
   unsigned int b = 1;

   while(n-- > 0)
   {
      /* TEMP <- SRC + DEST;
         SRC  <- DEST;
         DEST <- TEMP;
      */
      asm("xadd %1, %0" : "+r" (b), "+r" (a)); 
   }
   return a;
}


long fib_direct(unsigned int n)
{
   // long double sqrtl();
   // c++20 numbers::phi
   double constexpr phi = (1.0 + sqrt(5.0)) / 2.0;

   return lround(pow(phi, n) / sqrt(5.0));
}

/* Note:
   https://godbolt.org/
   for(; n > 0; --) { }
   while(n-- > 0) { }
   while(n > 0) { --n }

   all generate identical assembler code
*/

int main(int const argc, char const* const* const argv)
{
   if (argc < 3)
   {
      cerr << "usage: " << argv[0] << " function-no n\n" 
         "       compute fibonacci(n) using function 1..10\n";

      return EXIT_FAILURE;
   }
   int          const function_no = atoi(argv[1]);
   unsigned int const n           = make_unsigned_t<int>(atoi(argv[2]));

   cout << "n = " << n << " : ";

   switch(function_no)
   {
   case 1 :
      cout << "iterative 1   = " << fib_iterative1(n);
      break;
   case 2 :
      cout << "iterative 2   = " << fib_iterative2(n);
      break;
   case 3 :      
      cout << "iterative 3   = " << fib_iterative3(n);
      break;
   case 4 : 
      cout << "iterative 4   = " << fib_iterative4(n);
      break;
   case 5 :
      cout << "iterative 5   = " << fib_iterative5(n);
      break;
   case 6 : 
      cout << "iterative asm = " << fib_iterative_asm(n);
      break;
   case 7 : 
      cout << "recursive 1   = " << fib_recursive1(n);
      break;
   case 8 : 
      cout << "recursive 2   = " << fib_recursive2(n);
      break;
   case 9 : 
      cout << "direct        = " << fib_direct(n);
      break;
   case 10 : 
      cout << "bad code      = " << fval(n);
      break;
   default :
      cerr << "error: illegal function number " << function_no << endl;
      break;
   }
   cout << endl;
}
