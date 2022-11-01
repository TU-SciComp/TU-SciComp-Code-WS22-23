// Algorithm 2.1 from Gander, Gander, Kwok: Scientific Computing, page 11
// unstable computation of pi
// TK27Oct2022

#include <iostream>
#include <iomanip>
#include <cmath>

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
   int    n    = 6;
   double sine = sqrt(3.0) / 2.0; // = sin(60°) = sin(2pi/6)
   double area = 3.0 * sine;      // = 6/2 sin(2pi/6)

   print(n, area, sine);
   
   while (sine > 1e-10)
   {
      sine  = sqrt((1.0 - sqrt(1.0 - sine * sine)) / 2.0);
      area  = n * sine; // n=2n => n/2 sin => n sin
      n    *= 2;

      print(n, area, sine);
   }
}
