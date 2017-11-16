#pragma once

#include <vector>
#include <ctime>
#include <iostream>
#include <math.h>

template<typename T>
std::vector<int> build_matrix (const std::vector<T> &a, const std::vector<T> &b)
{
  int n = static_cast<int> (a.size ());
  int m = static_cast<int> (b.size ());
  std::vector<int> lens_matrix (n * m, 0);

  for (int i = n - 1; i >= 0; i--)
    for (int j = m - 1; j >= 0; j--)
      {
        if (a[i] == b[j])
          {
            if (i == n - 1 || j == m - 1)
              lens_matrix[i * m + j] = 1;
            else
              lens_matrix[i * m + j] = lens_matrix[(i + 1) * m + j + 1] + 1;
          }
        else
          {
            int right = j == m - 1 ? 0 : lens_matrix[i * m + j + 1];
            int down = i == n - 1 ? 0 : lens_matrix[(i + 1) * m + j];
            lens_matrix[i * m + j] = std::max (right, down);
          }
      }
  return lens_matrix;
}

template<typename T>
void get_max_subsequence (const std::vector<T> &a, const std::vector<T> &b,
                          const std::vector<int> &lens_matrix, std::vector<T> &max_subsequence,
                          const int i1 = 0, const int i2 = 0, const int k1 = 0)
{
  int n = static_cast<int> (a.size ());
  int m = static_cast<int> (b.size ());
  int length_max_subsequence = lens_matrix[0];
  //std::vector<T> max_subsequence (length_max_subsequence);
  max_subsequence.resize (length_max_subsequence);
  int k = k1;

  //for (int i = 0, j = 0; i < n && j < m && lens_matrix[i * m + j] != 0; )
  for (int i = i1, j = i2; i < n && j < m && lens_matrix[i * m + j] != 0; )
    {
      int right = j == m - 1 ? 0 : lens_matrix[i * m + j + 1];
      int down = i == n - 1 ? 0 : lens_matrix[(i + 1) * m + j];
      if (lens_matrix[i * m + j] > right)
        {
          if (lens_matrix[i * m + j] > down)
            {
              max_subsequence[k] = a[i];
              i++; j++; k++;
            }
          else
            i++;
        }
      else
        j++;
    }
  if (k != length_max_subsequence)
    std::cout << "error in subsequence build\n";
  //return max_subsequence;
}

