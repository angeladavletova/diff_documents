#pragma once

#include <string>
#include <vector>
#include <set>
#include <memory>
#include <fstream>

class text_parser;
struct word_info;
enum class title_type;

enum class simple_diff_type
{
  adding,
  changing,
  deleting,
  null
};

struct simple_diff
{
  std::pair<int, int> range_str1 = {-1, -1};
  std::pair<int, int> range_str2 = {-1, -1};
  simple_diff_type type = simple_diff_type::null;
  int index_last_word = -1;

  void clear ();

  bool empty () const;

  void print (std::ofstream &file, const std::string &text1, const std::string &text2) const;
};

class diff_searcher
{
  std::unique_ptr<text_parser> parser1; // умный указатель
  std::unique_ptr<text_parser> parser2;
  std::set<std::string> words_set;
  std::vector<word_info> subsequence;
  std::vector<simple_diff> result;
  std::vector<int> matrix;

public:
  diff_searcher (const std::string &text1, const std::string &text2, title_type type);
  ~diff_searcher ();
  const std::vector<simple_diff> &build_diff ();
};
