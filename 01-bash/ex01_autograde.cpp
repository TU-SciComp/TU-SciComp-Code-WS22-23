// TK22Oct2022
// g++ -std=c++17 -Wall -Wextra -O -s -static -o ex01_autograde ex01_autograde.cpp
// stip ex02_
#include <iostream>
#include <fstream>
#include <map>
#include <string>
#include <cmath>

using namespace std;

string const default_filename("Assignment1.txt");
double const required_precision{1e-8};

struct result_data
{
   double p1_diff;
   int    p2_count;
   int    p4_count;
};

map<int, result_data> const m_results =
   {
      { 20220201, { 190.9770219777565, 100, 47 }},
      { 20220202, { 192.0160273032117, 100, 55 }},
      { 20220203, { 195.3796140673593, 100, 50 }},
      { 20220204, { 189.8168339109404, 100, 45 }},
      { 20220205, { 195.2788785965182, 100, 50 }},
      { 20220206, { 197.7899395369983, 100, 47 }},
      { 20220207, { 197.6542608650689, 100, 56 }},
      { 20220208, { 198.7361801853944, 100, 52 }},
      { 20220209, { 196.1614492295769, 100, 51 }},
      { 20220210, { 198.1621152203348, 100, 53 }},
      { 20220211, { 196.5142736359552, 100, 53 }},
      { 20220212, { 191.7813593213962, 100, 49 }},
      { 20220213, { 194.6253893270580, 100, 62 }},
      { 20220214, { 190.7460096015464, 100, 50 }},
      { 20220215, { 198.3540260831805, 100, 48 }},
      { 20220216, { 192.6317508893522, 100, 46 }},
      { 20220217, { 197.9849810238966, 100, 52 }},
      { 20220218, { 195.8295133616876, 100, 48 }},
      { 20220219, { 195.5721408143639, 100, 50 }},
      { 20220220, { 195.9562483912663, 100, 55 }},
      { 20220221, { 199.3674336743769, 100, 53 }},
      { 20220222, { 196.8165536086814, 100, 55 }},
      { 20220223, { 196.4477164059682, 100, 47 }},
      { 20220224, { 199.1664783399389, 100, 48 }},
      { 20220225, { 191.6607087211571, 100, 54 }},
      { 20220226, { 193.0444941782531, 100, 41 }},
      { 20220227, { 197.8552899966898, 100, 60 }},
      { 20220228, { 197.5490255111075, 100, 38 }}
   };

// Program, should return 0 = EXIT_SUCCESS if the result matches and non-zero otherwise.
int main()
{
   ifstream input_file(default_filename);
   size_t   ok_count    = 0;
   size_t   line_count = 0;
   
   if (!input_file.is_open())
      cout << "File \"" << default_filename << "\" not found\n";
   else
   {
      string line;

      while(getline(input_file, line))
      {
         ++line_count;

         int    key_date = 0; // important if sscanf fails to fill key_date.
         double p1_diff;
         int    p2_count;
         int    p4_count;

         auto const ret = sscanf(line.c_str(), "%8d|%lf|%d|%d", &key_date, &p1_diff, &p2_count, &p4_count);
         auto const pos = m_results.find(key_date);

         if (pos == m_results.end())
            cout << "Date not correct: " << line << "\n";
         else
         {
            if (ret != 4
               || (fabs(pos->second.p1_diff - p1_diff) > required_precision)
               || pos->second.p2_count != p2_count
               || pos->second.p4_count != p4_count)               
               cout << "Line not correct: " << line << "\n";
            else
               ++ok_count;
         }
      }
   }
   return ok_count != line_count || ok_count != m_results.size();
}

