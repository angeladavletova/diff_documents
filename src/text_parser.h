#pragma once

#include <string>
#include <vector>
#include <set>

typedef std::set<std::string>::iterator index; 

enum class title_type;
struct text_title;

struct word_info
{
  index id;
  int place_in_text;

  word_info () {}
  word_info (index i, int place)
  {
    id = i;
    place_in_text = place;
  }
  bool operator== (const word_info &other) const
  {
    return id == other.id;
  }
  bool operator!= (const word_info &other) const
  {
    return id != other.id;
  }
};

class text_parser
{
  const std::string &text;
  std::set<std::string> &words_set;
  std::vector<word_info> words_indexes;
  std::vector<text_title> titles;

  void parse ();
public:
  text_parser(const std::string &str, std::set<std::string> &w_set, title_type type);
  ~text_parser();
  const std::vector<word_info> &get_indexes () const;
  int get_length_text () const;
  const std::vector<text_title> &get_titles () const;
};

