/**
 \file   fpprec.cpp
 \brief  demonstrate floating point precision
 \author Thorsten Koch
 \date   04 Oct 2014 / 25 Oct 2022

 On x86 compile with
 g++ -std=c++17 -Wall -mfpmath=387 -o fpprec fpprec.cpp
*/

#include <iostream>

using namespace std;

int test1()
{
   //double s[2048];
   double e = 1.0;
   int    n = 0;

   do
   {
      n    = n + 1;
      e    = e / 2.0;
      //s[n] = 1 + e;
   }
   while(e > 0.0);

   return n;
}

int test2()
{
   //double s[2048];
   double e = 1.0;
   int    n = 0;

   do
   {
      n    = n + 1;
      e    = e / 2.0;
      //s[n] = 1 + e;
   }
   while(1.0 + e > 1.0);

   return n;
}

int test3()
{
   double s[2048];
   double e = 1.0;
   int    n = 0;

   do
   {
      n    = n + 1;
      e    = e / 2.0;
      s[n] = 1 + e;
   }
   while(s[n] > 1.0);

   return n;
}

int main()
{
   cout << "while(e     > 0) n = " << test1() << endl;
   cout << "while(1 + e > 1) n = " << test2() << endl;
   cout << "while(s[n]  > 1) n = " << test3() << endl;
}
