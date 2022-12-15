/** 
 \file      squareroot.hpp
 \brief     Polymorphic squareroot functions
 \author    Thorsten Koch
 \version   1.0
 \date      15Dec2022
*/

#ifndef SQUAREROOT_H
#define SQUAREROOT_H

#include <cmath>
#include <cassert>
#include <quadmath.h>

inline float squareroot(float const x)
{
   assert(x >= 0.0f);
   return sqrtf(x);
}


inline double squareroot(double const x)
{
   assert(x >= 0.0);
   return sqrt(x);
}


inline long double squareroot(long double const x)
{
   assert(x >= 0.0L);
   return sqrtl(x);
}


inline __float128 squareroot(__float128 const x)
{
   assert(x >= 0.0L);
   return sqrtq(x);
}

#endif // !SQUAREROOT_H

