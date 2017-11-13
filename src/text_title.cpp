#include "text_title.h"
#include <string>
#include <regex>

std::vector<text_title> build_text_clauses (const std::string &text)
{
  std::string current_clause;
  std::vector<text_title> clauses;
  int length_text = text.length ();
  for (int i = 0; i < length_text; i++)
    {
      if ((text[i] >= '0' && text[i] <= '9') || (text[i] == '.'))
        {
          current_clause += text[i];
        }
      else if (!current_clause.empty ())
        {
          if (   current_clause[current_clause.length () - 1] == '.'
              && current_clause[0] != '.')
            {
              text_title t;
              t.title = current_clause;
              t.place_in_text = i - current_clause.length ();
              clauses.push_back (t);
            }
          current_clause.clear ();
        }
    }
  return clauses;
}

std::vector<text_title> build_text_articles (const std::string &text)
{
  std::regex txt_regex ("Статья ([0-9]+[0-9 .]*)\\.");
  std::smatch match;
  std::vector<text_title> articles;
  auto first_it = text.cbegin ();
  auto last_it = text.cend ();
  text_title current_title;

  while (first_it < last_it)
    {
      if (std::regex_search (first_it, last_it, match, txt_regex))
        {
          std::string piece = match[1];
          current_title.title = piece;
          current_title.place_in_text = std::distance (text.cbegin (), first_it) + match.position (0);
          articles.push_back (current_title);
          first_it = text.cbegin () + current_title.place_in_text + match[0].length ();
        }
      else
        break;
    }
  return articles;
}

std::vector<text_title> build_text_titles (const std::string &text, title_type type)
{
  std::vector<text_title> titles;
  switch (type)
    {
    case title_type::article:
      titles = build_text_articles (text);
      break;
    case title_type::clause:
      titles = build_text_clauses (text);
      break;
    }

  return titles;
//  return get_max_increasing_subsequence (titles, [] (const text_title &title1, const text_title &title2) {
//      std::string val1 = title1.title;
//      std::string val2 = title2.title;
//      int len1 = static_cast<int> (val1.length ());
//      int len2 = static_cast<int> (val2.length ());
//      int i = 0;
//      while (i < len1 && i < len2)
//        {
//          if (val1[i] == val2[i])
//            {
//              i++;
//              continue;
//            }
//          if (val1[i] != '.' && val2[i] != '.' )
//            return val1[i] < val2[i];
//          if (val1[i] == '.')
//            return true;
//          return false;
//        }
//      return len1 < len2;
//    });
}

std::string prefix_title (title_type type, const std::string &title)
{
  if (title.empty ())
    return "";
  switch (type)
    {
    case title_type::article:
      return std::string ("В статье ") + title;
    case title_type::clause:
      return std::string ("В пункте ") + title;
    }
  return "";
}
