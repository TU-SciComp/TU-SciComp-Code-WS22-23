#include <iostream>

void f(int i)
{
   int a[100];

   a[1] = i + 1;
   
   if (i % 1000 == 0)
   {
      std::cout << i / 1000 << " ";
      std::cout.flush();
   }
   f(a[1]);
}

int main()
{
   f(1);
}
