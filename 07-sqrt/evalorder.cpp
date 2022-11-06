// Order of Evaluation
// TK03Nov2022

#include <iostream>

using namespace std;

int f1(void) { cout << "f1 "; return 2; }
int f2(void) { cout << "f2 "; return 3; }
int f3(void) { cout << "f3 "; return 5; }
int f4(void) { cout << "f4 "; return 7; }

int ff(int a, int b, int c)
{
   cout << "ff\n";
   return a + b + c;
}

int main()
{
   auto x = ff(f1(), f2() * f3(), f4());

   cout << "x=" << x << endl;
}
