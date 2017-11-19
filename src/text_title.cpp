#include "text_title.h"
#include <string>
#include <regex>

static bool is_clause (int i, const std::string &clause, const std::string &text)
{
  int len_clause = clause.length ();
  int previous_pos = i - len_clause - 1;
  return clause[len_clause - 1] == '.'
      && clause[0] != '.'
      && (previous_pos < 0 || text[previous_pos] == '\n');
}

std::vector<text_title> build_text_clauses (const std::string &text)
{
  std::string current_clause;
  std::vector<text_title> clauses;
  int length_text = text.length ();
  for (int i = 0; i < length_text; i++)
    {
      if (is_num (text[i]) || (text[i] == '.'))
        {
          current_clause += text[i];
        }
      else if (!current_clause.empty ())
        {
          if (is_clause (i, current_clause, text))
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

  auto parse_title = [] (const std::string &title) {
      std::vector<int> title_numbers;
      int current_val = 0;

      auto add_val = [&current_val, &title_numbers] () {
          if (current_val != 0)
            title_numbers.push_back (current_val);
        };

      for (char t: title)
        {
          if (t != '.')
            current_val = current_val * 10 + atoi (&t);
          else
            {
              add_val ();
              current_val = 0;
            }
        }
      add_val ();
      return title_numbers;
    };

  auto compare = [parse_title] (const text_title &title1, const text_title &title2) {
      std::vector<int> title_number1 = parse_title (title1.title);
      std::vector<int> title_number2 = parse_title (title2.title);
      int size1 = static_cast<int> (title_number1.size ());
      int size2 = static_cast<int> (title_number2.size ());
      int i = 0;

      while (i < size1 && i < size2)
        {
          if (title_number1[i] < title_number2[i])
            return true;
          if (title_number1[i] > title_number2[i])
            return false;
          i++;
        }
      return size1 < size2;
    };

  return titles;
  //return get_max_increasing_subsequence (titles, compare);
}

std::string prefix_title (title_type type, const std::string &title)
{
  if (title.empty ())
    return "";
  switch (type)
    {
    case title_type::article:
      return std::string ("В Cтатье ") + title + " ";
    case title_type::clause:
      return std::string ("В пункте ") + title + " "; // Bad work
      break;
    }
  return "";
}


bool is_num (char c)
{
  return c >= '0' && c <= '9';
}


bool is_letter (char c)
{
  return (c >= 'а' && c <= 'я') || (c >= 'А' && c <= 'Я');
}
