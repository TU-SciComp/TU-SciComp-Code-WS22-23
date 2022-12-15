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

inline float absval(float const x)
{
   return fabsf(x);
}


inline double absval(double const x)
{
   return fabs(x);
}


inline long double absval(long double const x)
{
   return fabsl(x);
}


inline __float128 absval(__float128 const x)
{
   return fabsq(x);
}


#endif // !SQUAREROOT_H

