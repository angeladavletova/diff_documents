#include <algorithm>
#include "diff_searcher.h"
#include "text_parser.h"
#include "max_subsequence.h"
#include "text_title.h"

diff_searcher::diff_searcher (const std::string &text1, const std::string &text2, title_type type)
{
  parser1.reset (new text_parser (text1, words_set, type));
  parser2.reset (new text_parser (text2, words_set, type));
  matrix = build_matrix (parser1->get_indexes (), parser2->get_indexes ());
  get_max_subsequence (parser1->get_indexes (), parser2->get_indexes (), matrix, subsequence);
}

diff_searcher::~diff_searcher ()
{
}

static bool pair_is_empty (const std::pair<int, int> &range)
{
  return range.second - range.first <= 0;
}

static void add_to_str (std::pair<int, int> &range_str, word_info w_info, int length_text)
{
  if (pair_is_empty (range_str))
    {
      range_str.first = w_info.place_in_text > 0 ? w_info.place_in_text - 1 : w_info.place_in_text;
    }
  int end_of_word = w_info.place_in_text + (*w_info.id).length ();
  range_str.second = end_of_word < length_text ? end_of_word + 1 : end_of_word;
}

const std::vector<simple_diff> &diff_searcher::build_diff ()
{
  int i1 = 0, i2 = 0, k = 0;
  const std::vector<word_info> &indexes1 = parser1->get_indexes ();
  const std::vector<word_info> &indexes2 = parser2->get_indexes ();
  simple_diff current_diff;
  int m = static_cast<int> (indexes2.size ());
  int length_text1 = parser1->get_length_text ();
  int length_text2 = parser2->get_length_text ();

  while (k < static_cast<int> (subsequence.size ()))
    {
      if (indexes1[i1] == indexes2[i2] && indexes2[i2] != subsequence[k])
        printf ("subsequence is not maximum \n");

      if (indexes1[i1] == indexes2[i2]) // c c c
        {
          if (!current_diff.empty ())
            result.push_back (current_diff);
          i1++;
          i2++;
          k++;
          current_diff.clear ();
        }
      else if (indexes1[i1] != subsequence[k] && indexes2[i2] != subsequence[k]) // a b c
        {
          current_diff.init_title (parser1->get_titles (), indexes1[i1].place_in_text);
          add_to_str (current_diff.range_str1, indexes1[i1], length_text1);
          add_to_str (current_diff.range_str2, indexes2[i2], length_text2);
          current_diff.type = simple_diff_type::changing;
          i1++;
          i2++;
        }
      else if (indexes1[i1] == subsequence[k]) // c a c
        {
          int number_of_passes = result.empty () ? 0 : i2 - result.back ().index_last_word - 1;
          if ( current_diff.empty ()
               && !result.empty ()
               && result[result.size () - 1].type == simple_diff_type::adding
               &&   matrix[(i1 - number_of_passes) * m + i2 - number_of_passes]
                 == matrix[(i1 - number_of_passes) * m + i2])
            {
              get_max_subsequence (indexes1, indexes2, matrix, subsequence,
                                   i1 - number_of_passes, i2, k - number_of_passes);
              current_diff = result.back ();
              result.erase (result.end () - 1);
              add_to_str (current_diff.range_str2, indexes2[i2 - 1], length_text2);
              current_diff.index_last_word = i2 - 1;
              i1 -= number_of_passes;
              k -= number_of_passes;
            }
          else
            {
              if (current_diff.empty ())
                {
                  current_diff.type = simple_diff_type::adding;
                  current_diff.init_title (parser2->get_titles (), indexes2[i2].place_in_text);
                }
              add_to_str (current_diff.range_str2, indexes2[i2], length_text2);
              current_diff.index_last_word = i2;
              i2++;
            }
        }
      else // a c c
        {
          int number_of_passes = result.empty () ? 0 : i1 - result.back ().index_last_word - 1;
          if ( current_diff.empty ()
               && !result.empty ()
               && result[result.size () - 1].type == simple_diff_type::deleting
               &&   matrix[i1 * m + i2 - number_of_passes]
                 == matrix[(i1 - number_of_passes) * m + i2 - number_of_passes])
            {
              get_max_subsequence (indexes1, indexes2, matrix, subsequence,
                                   i1, i2 - number_of_passes, k - number_of_passes);
              current_diff = result.back ();
              result.erase (result.end () - 1);
              add_to_str (current_diff.range_str1, indexes1[i1 - 1], length_text1);
              current_diff.index_last_word = i1 - 1;
              i2 -= number_of_passes;
              k -= number_of_passes;
            }
          else
            {
              if (current_diff.empty ())
                {
                  current_diff.type = simple_diff_type::deleting;
                  current_diff.init_title (parser1->get_titles (), indexes1[i1].place_in_text);
                }
              add_to_str (current_diff.range_str1, indexes1[i1], length_text1);
              current_diff.index_last_word = i1;
              i1++;
            }
        }
    }
  int count1 = static_cast<int> (indexes1.size ());
  int count2 = static_cast<int> (indexes2.size ());

  for (int i = i1; i < count1; i++)
    add_to_str (current_diff.range_str1, indexes1[i], length_text1);

  for (int i = i2; i < count2; i++)
    add_to_str (current_diff.range_str2, indexes2[i], length_text2);

  if (!current_diff.empty ())
    {
      if (pair_is_empty (current_diff.range_str1))
        current_diff.type = simple_diff_type::adding;
      else if (pair_is_empty (current_diff.range_str2))
        current_diff.type = simple_diff_type::deleting;
      else
        current_diff.type = simple_diff_type::changing;
      result.push_back (current_diff);
    }
  return result;
}

void simple_diff::init_title (const std::vector<text_title> &titles, int i)
{
  if (!title.empty ())
    return;

  auto it = std::upper_bound (titles.cbegin (), titles.cend (), i, [] (int i, const text_title &title) {
      return i < title.place_in_text;
    });
  if (it == titles.cbegin ())
    return;

  it--;
  title = (*it).title;
}

void simple_diff::clear ()
{
  range_str1 = {-1, -1};
  range_str2 = {-1, -1};
  title.clear ();
}

bool simple_diff::empty () const
{
  return pair_is_empty (range_str1) && pair_is_empty (range_str2);
}

void simple_diff::print (std::ofstream &file, const std::string &text1, const std::string &text2,
                         title_type t_type) const
{
  switch (type)
    {
    case simple_diff_type::adding:
      {
        file << prefix_title (t_type, title)
             << "ÄÎÁÀÂÈËÈ:\n"
             << text2.substr (range_str2.first, range_str2.second - range_str2.first)
             << "\n\n";
        break;
      }
    case simple_diff_type::changing:
      {
        file << prefix_title (t_type, title)
             << "ÇÀÌÅÍÈËÈ:\n"
             << text1.substr (range_str1.first, range_str1.second - range_str1.first)
             << "\n íà: \n"
             << text2.substr (range_str2.first, range_str2.second - range_str2.first)
             << "\n\n";
        break;
      }
    case simple_diff_type::deleting:
      {
        file << prefix_title (t_type, title)
             << "ÓÄÀËÈËÈ:\n"
             << text1.substr (range_str1.first, range_str1.second - range_str1.first)
             << "\n\n";
        break;
      }
    case simple_diff_type::null:
      break;
    }
}
