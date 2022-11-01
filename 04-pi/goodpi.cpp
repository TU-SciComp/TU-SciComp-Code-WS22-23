// Algorithm 2.1 from Gander, Gander, Kwok: Scientific Computing, page 21
// stable computation of pi
// TK27Oct2022

#include <iostream>
#include <iomanip>
#include <cmath>
#include <cassert>

using namespace std;

void print(int n, double pi, double sine)
{
   cout << setw(14) << right << noshowpos << n << "   ";
   cout << setprecision(15) << left << fixed << showpos;
   cout << setw(22) << pi;
   cout << setw(22) << pi - M_PI ;
   cout << setw(22) << sine << endl;
}

int main()
{
   auto old_area = 0.0;
   auto sine     = sqrt(3.0) / 2.0; // = sin(60°) = sin(2pi/6)
   auto area     = 3.0 * sine;      // = 6/2 sin(2pi/6)   
   auto n        = 6;               // maybe use "long n" or "6L", check with print

   print(n, area, sine);
   
   while (area > old_area)
   {
      assert(n > 0);
      
      old_area = area;      
      sine     = sine / sqrt(2.0 * (1.0 + sqrt((1.0 + sine) * (1.0 - sine))));
      area     = n * sine; // n=2n => n/2 sin => n sin
      n       *= 2;

      print(n, area, sine);
   }
}
