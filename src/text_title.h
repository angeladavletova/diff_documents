#pragma once

#include <string>
#include <vector>
#include <algorithm>

bool is_num (char c);

bool is_letter (char c);

enum class title_type
{
  article,
  clause
};

struct text_title
{
  std::string title;
  int place_in_text;
};

std::vector<text_title> build_text_titles (const std::string &text, title_type type);
std::string prefix_title (title_type type, const std::string &title);

template<class T, class Compare>
std::vector<T> get_max_increasing_subsequence (const std::vector<T> &a, Compare comp)
{
  int n = static_cast<int> (a.size ());
  std::vector<T> max_subseq;

  for (int i = 0; i < n; i++)
    {
      int j = static_cast<int> (upper_bound (max_subseq.begin(), max_subseq.end(), a[i], comp)
                                - max_subseq.begin());
      if (j == static_cast<int> (max_subseq.size ()))
        max_subseq.push_back (a[i]);
      else if (j == 0)
        max_subseq[j] = a[i];
      else if (comp (max_subseq[j - 1], a[i])) // (max_subseq[j - 1] < a[i])
        max_subseq[j] = a[i];
    }
  return max_subseq;
}
