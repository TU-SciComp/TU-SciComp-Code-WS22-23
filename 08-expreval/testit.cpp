/**
 \file      testit.cpp
 \brief     usage demonstrator and regression test 
 \author    Thorsten Koch
 \version   1.0
 \date      19Jun2022
 \copyright Copyright (C) 2020 by Thorsten Koch <koch@zib.de>,
            licened under LGPL version 3 or later
*/ 

#include <iostream>
#include <iomanip>
#include <string>
#include <cstring>
#include <cmath>
#include <cassert>

#include "expreval.h"

using namespace std;

/** Demo main program
 *  Call either as
 * \code testit -i "3*9+27"
 *  or
 * \code testit <expression_list_file_1.txt 
 *  or
 * \code testit 
 * and type input directly.
 */
int main(const int argc, char** argv)
{
   // no arguments? => print usage info
   if ((argc > 1) && !strcmp(argv[1], "-h"))
   {
      cerr << "usage: " << argv[0] << " expr or " << argv[0] << " <file" << endl;
   }
   else
   {
      // expression to evaluate given on command line
      if (argc > 1)
      {
         for(auto i = 1; i < argc; i++)
         {
            cout << "[" << argv[i] << "]" << endl;
            cout << "= " << setprecision(17) << expreval(argv[i]) << endl;
         }
      }
      else // otherwise read from stdin
      {
         int    fails    = 0;
         int    count    = 0;
         string line;
         double expected = 0.0;

         while(getline(cin, line))
         {
            auto i = line.find_first_of('=');
            
            if (i != string::npos)
               expected = stod(line.substr(i + 1));

            string expr_text = line.substr(0, i - 1);
            double result    = expreval(expr_text);
            //lint -e{777} testing floating point values for equality
            bool   is_ok     = result == expected || (isnan(result) && isnan(expected));

            if (!is_ok)
               fails++;

            count++;

            cerr << setw(64) << left << expr_text;
            cerr << " = " << setw(23) << right << setprecision(16) << scientific << result;
            cerr << " | " << setw(23) << right << setprecision(16) << scientific << expected;
            cerr << (is_ok ? " Ok" : " Fail") << endl;
         }
         cout << "Result: " << fails << " fails out of " << count << " tests\n";
      }
   }
   return EXIT_SUCCESS;
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
