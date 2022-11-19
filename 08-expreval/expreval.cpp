/**
 \file      expreval.cpp
 \brief     Recursive Decent Expression Parser 
 \author    Thorsten Koch
 \version   1.1
 \date      19Jun2022
 \copyright Copyright (C) 2022 by Thorsten Koch <koch@zib.de>,
            licened under LGPL version 3 or later
 \details

 The following implements an recursive decent expression parser.
 
 The expression to parse, e.g., "12+5*7^(2+4)" is given as a string.
 Arithmetic rules are obeyed.

 The implentation is based on a
 number of function who call each other recursively, depending
 on what input is expected next. The grammar looks like this:
 
 \verbatim
 E  -> T E'
 E' -> + T E' | - T E' | error | epsilon
 T  -> P T'
 T' -> * S T' | / S E' | error | epsilon
 S  -> - P S'  // ????
 S' -> 
 P  -> F P'
 P' -> F ^ S' | error | epsilon
 F  -> (E) | int | error | epsilon
 \endverbatim           
*/

//#define DEBUGING_RUN   1

#include <iostream>
#include <iomanip>
#include <string>
#include <map>
#include <ctype.h>
#include <stdexcept>
#include <cmath>
#include <cfenv>
#include <cassert>

using namespace std;

#include "expreval.h"

/** List of buildin functions and constants.
 *  Additional constants or functions can just be added.
 */
static map<string, double (*)(double)> const buildin = {
   { "pi",    [] (double) { return M_PI; } },
   { "euler", [] (double) { return M_E; } }, // e is bad because of writing FP numbers as 1e7
   { "abs",  fabs },
   { "sqrt", sqrt },
   { "log",  log  },
   { "sin",  sin  },
   { "cos",  cos  },
   { "tan",  tan  },
};


struct Expr
{
   string            expr_;
   string::size_type pos_ = 0;       ///< current position in expression.
   
#ifdef DEBUGING_RUN

   int               depth_ = 0;

#define value(v) value_(v, __func__)

   inline void trace(char const* const fun) const
   {
      char const* const tab = ".";
      cerr << setw(depth_) << tab << " trace: " << fun << " [" << expr_.substr(pos_) << "]" << endl;
      depth_++;
   }

   inline double value_(double const v, char const* const fun)
   {
      char const* const tab = ".";
      cerr << setw(depth_) << tab << " value: " << fun << " = " << v << " [" << expr_.substr(pos_) << "]" << endl;
      depth_--;

      return v;
   }
#else
   
#define trace(s) /**/
#define value(v) (v)

#endif // DEBUGING_RUN
   
   inline void go_next_token_eat_spaces()
   {
      pos_++;
      while(isspace(expr_[pos_]))
         pos_++;
   }

   inline bool is_token(char const c) const
   {
      return expr_[pos_] == c;
   }
   
   inline char curr_token() const
   {
      return expr_[pos_];
   }

   /** parse parameters of a function, apply the function to the parameter, and return the result.
    */
   double parse_func()
   {
      trace(__func__);

      assert(!at_end_of_expr());
      assert(!isdigit(curr_token()));
   
      auto r       = 0.0;
      auto pos_new = pos_;
      
      while(isalnum(expr_[pos_new]) || expr_[pos_new] == '_')
         pos_new++;

      // pos_new now points to first character after constant/function name
      string name = expr_.substr(pos_, pos_new - pos_);

      pos_ = pos_new;
      
      try
      {
         // is it a constant?
         if (expr_[pos_new] != '(')
            r = (*buildin.at(name))(0.0);
         else // no, a function xxx(...)
         {
            go_next_token_eat_spaces(); 

            r = (*buildin.at(name))(parse_expr());
      
            if (!is_token(')'))
               throw runtime_error("expected ')' or operator");
      
            go_next_token_eat_spaces();
         }
      }
      catch (out_of_range&)
      {
         throw runtime_error("unknown constant/function name");
      }
      return value(r);
   }

   /** Collect digits (1232345) into a number or parse function invocation.
    */
   double parse_numb()
   {
      trace(__func__);

      assert(!at_end_of_expr());

      auto r = 0.0;
   
      if (!isdigit(curr_token()))
         r = parse_func();
      else
      {
         string::size_type after;
         r = stod(expr_.substr(pos_), &after);
         pos_ += after;
         
         if (isspace(curr_token()))
            go_next_token_eat_spaces();
      }
      return value(r);
   }


   /** Get number or parenthesised expression.
    * e.g., 1626273 (...) sin(6.6) 
    */
   double parse_fact()
   {
      trace(__func__);

      auto r = 0.0;
 
      if (!at_end_of_expr())  // needed otherwise error would be unkown constant ""
      {
         if (is_token('('))
         {
            go_next_token_eat_spaces();

            r = parse_expr();

            if (is_token(')'))
               go_next_token_eat_spaces();
            else
               throw runtime_error("')' expected");
         }
         else
         {
            r = parse_numb();
         }
      }
      return value(r);
   }

   /** Collect powers, e.g. a^b.
    */
   double parse_powr()
   {
      trace(__func__);

      auto r = parse_fact();

      while(is_token('^'))
      {
         go_next_token_eat_spaces();
      
         r = pow(r, parse_sign());      
      }
      return value(r);
   }

   /** Remove extra unary signs, e.g., --++--g -> g.
    */
   double parse_sign() 
   {
      trace(__func__);

      auto r = 1.0;

      while(is_token('-') || is_token('+'))
      {
         if (is_token('-'))
            r *= -1.0;
      
         go_next_token_eat_spaces();
      }
      r *= parse_powr();

      return value(r);
   }

   /** Multiply (signed) factors: d * e / f.
    */
   double parse_term() 
   {
      trace(__func__);

      auto r = parse_sign();

      while((is_token('/') || is_token('*')))
      {
         auto is_divide = is_token('/');

         go_next_token_eat_spaces();

         if (is_divide)
            r /= parse_sign();
         else // token == '*'
            r *= parse_sign();
      }
      return value(r);
   }

   inline bool at_end_of_expr() const
   {
      return pos_ == expr_.length();
   }

   /** Sum-up additive terms: a + b - c.
    */
   double parse_expr()
   {
      trace(__func__);

      auto r = parse_term();

      while((is_token('-') || is_token('+')))
      {
         auto const is_minus = is_token('-');

         go_next_token_eat_spaces();

         if (is_minus)
            r -= parse_term();
         else // token == '+'
            r += parse_term();
      }
      return value(r);
   }

   explicit Expr(const string& s) : expr_{s}
   {
      while(isspace(expr_[pos_]))
         pos_++;
   }
   Expr(const Expr&) = delete; // default;
   Expr& operator=(const Expr&) = delete; // default
};


/** Parse a string with an expression, like "4+7*22.3-7^4+sin(90)".
 */
double expreval(const string& s)
{
   Expr ex{s};
   auto r = 0.0;
   
   //trace( __func__);

   feclearexcept(FE_ALL_EXCEPT);

   try
   {
      r = ex.parse_expr();

      if (!ex.at_end_of_expr())
         throw runtime_error("unexpected character");

      int const ferror = fetestexcept(FE_ALL_EXCEPT & ~FE_INEXACT & ~FE_UNDERFLOW);

      if (ferror || !isfinite(r))
      {
         if      (ferror & FE_DIVBYZERO)
            cerr << "Divide by zero\n";
         else if (ferror & FE_INVALID)
            cerr << "Invalid argument\n";
         else if (ferror & FE_OVERFLOW)
            cerr << "FP Overflow\n";
         else
            cerr << "FP error " << ferror << endl;
         
         r = NAN;
      }
      feclearexcept(FE_ALL_EXCEPT);
   }
   catch (runtime_error& e)
   {
      char const pointer = '^';
      cerr << "Syntax Error: " << e.what() << endl;
      cerr << s << endl;
      cerr << setw(ex.pos_ + 1) << right << pointer << endl; //lint !e712
      r = 0.0;
   }  
   return r;
}

/*
 * This code is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public License
 * as published by the Free Software Foundation; either version 3
 * of the License, or (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
