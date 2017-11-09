#include "document_structure.h"
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
          if (current_clause[current_clause.length () - 1] == '.')
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

//std::vector<text_title> build_text_articles (const std::string &text)
//{
//  std::regex txt_regex("Статья [0-9]+\\."); //   "Статья ([0-9]+)\\."
//  std::smatch match;
//  std::vector<text_title> articles;
//  auto first_it = text.begin ();
//  auto last_it = text.end ();

//  while (first_it < last_it)
//    {
//      std::regex_match (first_it, last_it, match, txt_regex);


//    }
//}







