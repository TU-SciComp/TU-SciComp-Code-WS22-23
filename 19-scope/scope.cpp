//TK08Dec2022

#include <iostream>

int x = 1;

class X
{
   int x = 4;

public:
   X() { x = ::x; };
   X(int x) : x(x) {};
   void print() { std::cout << x; };
};


int f(int x)
{
   return x + 1;
}

int g(int z)
{
   return x + z;
}

int main()
{
   std::cout << x;

   int x = 2;

   std::cout << x;   
   {
      for(int x = 1; x < 7; ++x)
         ;
      std::cout << x;
      int x = 3;
      std::cout << x;
   }
   X z(5);
   z.print();
   std::cout << x;
   std::cout << f(x);
   {
      std::cout << g(x);
      X x;
      x.print();      
   }
   std::cout << std::endl;
}
