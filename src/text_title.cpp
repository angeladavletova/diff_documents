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
}
