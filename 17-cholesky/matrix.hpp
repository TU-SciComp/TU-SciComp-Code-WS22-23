/** 
 \file      matrix.hpp
 \brief     Template class for Matrix including Cholesky (LL^t) decomposition.
 \author    Thorsten Koch
 \version   1.0
 \date      15Dec2022
*/

#ifndef MATRIX_HPP
#define MATRIX_HPP

#include <vector>
#include <limits>
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <exception>
#include <random>
#include <string>
#include <cassert>

#include "squareroot.hpp"

template <typename T>
class Matrix
{
protected:
   size_t         size_;   //< Size of the suqare matrix
   std::vector<T> value_;  //< values of the matrix stored as a(i,j) = value_(i * size_ + j).

   bool     is_valid() const                { return size_ * size_ == value_.size(); }
   T&       value(size_t r, size_t c)       { assert(r < size_ and c < size_); return value_[r * size_ + c]; };
   T const& value(size_t r, size_t c) const { assert(r < size_ and c < size_); return value_[r * size_ + c]; };

public:
   Matrix()                                  : size_(0), value_(0)     { assert(is_valid()); };
   explicit Matrix(size_t n)                 : size_(n), value_(n * n) { assert(is_valid()); };
   Matrix(size_t n, std::vector<T> const& a) : size_(n), value_(a)     { assert(is_valid()); };
   Matrix(size_t n, unsigned seed);

   T&       operator()(size_t r, size_t c)       { return value(r, c); };
   T const& operator()(size_t r, size_t c) const { return value(r, c); };
   
   Matrix(Matrix const&)            = default; 
   Matrix(Matrix&&)                 = default; 
   Matrix& operator=(Matrix const&) = default;
   Matrix& operator=(Matrix&&)      = default;
   ~Matrix()                        = default;

   size_t size() const { return size_; };
   
   Matrix cholesky() const;
   Matrix transpose() const;
   std::vector<T> triangular_solve(std::vector<T> const& b) const;
   void read(std::string const& filename);
};

template <typename T>
Matrix<T>::Matrix(size_t n, unsigned seed) : size_(n), value_(n * n)
{
   std::mt19937                      rng(seed);
   std::uniform_real_distribution<float> dist(0.0, 1.0); // distribution in range [0, 1]

   for(size_t i = 0; i < size_; i++)
      for(size_t j = 0; j <= i; j++)
         value_[i * n + j] = static_cast<T>(dist(rng));

   assert(is_valid());
};


template <typename T>
Matrix<T> Matrix<T>::cholesky() const
{
   Matrix l_result(size_);
      
   for(size_t i = 0; i < size_; i++)
   {
      for (size_t j = 0; j <= i; j++)
      {
         T sum = value(i, j);

         for(size_t k = 0; k < j; k++)
            sum -= l_result(i, k) * l_result(j, k);

         if (i == j)
            l_result(i, j) = squareroot(sum); 
         else
            l_result(i, j) = sum / l_result(j, j);
      }
   }
   return l_result;
};


template <typename T>
Matrix<T> Matrix<T>::transpose() const
{
   Matrix t(size_);

   for(size_t i = 0; i < size_; i++)
      for(size_t j = 0; j < size_; j++)
         t(j, i) = value(i, j);

   return t;
};


template <typename T>
std::vector<T> Matrix<T>::triangular_solve(std::vector<T> const& b) const
{
   assert(size_ == b.size());

   std::vector<T> x(size_);
      
   for(size_t i = 0; i < size_; i++)
   {
      T s = b[i];
      
      for(size_t k = 0; k < i; k++)
         s -= value(i, k) * x[k];
      
      x[i] = s / value(i, i);
   }
#if 0
   for(int i = static_cast<int>(size_) - 1; i >= 0; --i)
   {
      x[i] /= value(i, i);

      for(int k = i - 1; k >= 0; --k)
         x[k] -= value(i, k) * x[i];
   }
#else
   if (size_ > 0)
   {
      size_t i = size_;
      
      do
      {
         i--;
         
         x[i] /= value(i, i);

         if (i > 0)
         {
            size_t k = i;

            do
            {
               k--;
               
               x[k] -= value(i, k) * x[i];
            }
            while(k != 0);
         }
      }
      while(i != 0);
#endif
   }
   
   return x;
}

template<typename T>
void Matrix<T>::read(std::string const& filename)
{
   using std::runtime_error, std::to_string;
      
   std::ifstream input(filename);
   bool          head_line = true;
   size_t        line_no   = 0;
   std::string   line;
   int           rows      = 0;
   int           cols      = 0;
   int           nnzs      = 0;

   if (not input)
      throw runtime_error("Cannot open file: " + filename);
         
   while(getline(input, line))
   {
      line_no++;

      std::istringstream iss(line);
         
      // Remove commens starting with %
      if (size_t n = line.find_first_of('%'); n < line.length())
         line.erase(n); // n is start position for erase 

      // If we have an empty line, ignore
      if (std::all_of(line.begin(), line.end(), isspace))
         continue;

      if (head_line)
      {
         head_line = false;

         if ((iss >> rows >> cols >> nnzs).fail() or rows < 1 or cols < 1 or nnzs < 1 or rows != cols)
            throw runtime_error("Syntax error line " + to_string(line_no));

         size_ = rows; //lint !e732
         value_.resize(size_ * size_, 0.0);
      }
      else
      {
         int row;
         int col;
         T   val;

         if ((iss >> row >> col >> val).fail() or row < 1 or col < 1 or row > rows or col > cols)
            throw runtime_error("Syntax error line " + to_string(line_no));

         value(row - 1, col - 1) = val; //lint !e732
      }
   }
   std::cout << "Read " << filename << " with " << line_no << " lines "
             << rows << " rows " << cols << " columns " << nnzs << " nonzeros,\n";
}


// C = A * B
template <typename T>
Matrix<T> operator*(Matrix<T> const& a, Matrix<T> const& b)
{
   assert(a.size() == b.size());
   
   size_t const n = a.size();

   Matrix<T> c(n);
   
   for(size_t i = 0; i < n; i++)
   {
      for(size_t j = 0; j < n; j++)
      {
         T sum = 0.0;

         for(size_t k = 0; k < n; k++)
            sum += a(i, k) * b(k, j);

         c(i, j) = sum;
      }
   }
   return c;   
}


template<typename T>
std::ostream & operator<<(std::ostream & os, std::vector<T> const& vec)
{
   os << "[ ";
   for(auto x : vec)
      os << x << ' ';
   os << "]";

   return os;
}


template<typename T>
std::ostream & operator<<(std::ostream& os, Matrix<T> const& mat)
{
   os.setf(std::ios_base::fixed);
      
   for(size_t i = 0; i < mat.size(); i++)
   {
      os << "|";
      for(size_t j = 0; j < mat.size(); j++)
         os << " " << std::setw(9) << std::setprecision(5) << mat(i, j);
      os << " |\n";
   }
   return os;
}


// r = A * x 
template <typename T>
std::vector<T> operator*(Matrix<T> const& a, std::vector<T> const& x)
{
   assert(a.size() == x.size());

   std::vector<T> r(x.size(), 0.0);
   
   for(size_t i = 0; i < x.size(); i++)
      for(size_t j = 0; j < x.size(); j++)
         r[i] += a(i, j)* x[j]; 

   return r;   
}

#if 1 // Which is nicer and easier to understand?
template <typename T>
T two_norm(std::vector<T> const& vec)
{
   T sum = 0.0;
   
   for(auto x : vec)
      sum += x * x;

   return squareroot(sum);
}
#else
template <typename T>
T two_norm(std::vector<T> const& vec)
{
   return squareroot(std::accumulate(vec.begin(), vec.end(), static_cast<T>(0.0),
         [](T const& sum, T const& x) { return sum + x * x; }));
}
#endif


template <typename T>
T max_norm(std::vector<T> const& vec)
{
   return *max_element(vec.begin(), vec.end());
}


// c = a - b
template <typename T>
std::vector<T> operator-(std::vector<T> const& a, std::vector<T> const& b)
{
   assert(a.size() == b.size());

   std::vector<T> r(a.size(), 0.0);
   
   for(size_t i = 0; i < a.size(); ++i)
      r[i] = a[i] - b[i];

   return r;
}

#endif // !MATRIX_HPP
