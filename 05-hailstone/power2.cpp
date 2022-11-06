/**
 \file      power2.cpp
 \brief     Show this actually works
 \author    Thorsten Koch
 \version   1.0
 \date      25 Oct 2022

 From
 https://stackoverflow.com/questions/600293/how-to-check-if-a-number-is-a-power-of-2
 Read about two-complement storage of numbers.
 Note: 0 is wrongly giving as true,
*/ 

#include <iostream>

int main()
{
   for(int i = 0; i < 150000; i++)
   {
      if ((i & (i - 1)) == 0)
         std::cout << i << std::endl;
   }
}
