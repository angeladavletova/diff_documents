#include "text_parser.h"
#include "text_title.h"

#include <iostream>
#include <fstream>

text_parser::text_parser (const std::string &str, std::set<std::string> &w_set, title_type type)
  : text (str), words_set (w_set)
{
  titles = build_text_titles (text, type);
  std::ofstream fout_time ("res.txt");
  for (const auto &title : titles)
    fout_time << title.title << "\n";// << title.place_in_text << "\n\n";
  fout_time.close ();
  parse ();
}

static int add_number_to_article (std::string &article, int i, const std::string &text)
{
  int length_text = text.length ();
  while (i < length_text && (is_num (text[i]) || text[i] == '.'))
    {
      article += text[i];
      i++;
    }
  return i;
}

void text_parser::parse ()
{
  std::string current_word;
  int length_text = text.length ();
  for (int i = 0; i < length_text; i++)
    {
      if (is_letter (text[i]) /*|| (text[i] >= '0' && text[i] <= '9')*/)
        {
          current_word += text[i];
        }
      else if (!current_word.empty ())
        {
          if (current_word == "������")
            i = add_number_to_article (current_word, i + 1, text);
          word_info w (words_set.insert (current_word).first, i - current_word.length ());
          words_indexes.push_back (w);
          current_word.clear ();
        }
    }
}

text_parser::~text_parser ()
{
}

const std::vector<word_info> &text_parser::get_indexes () const
{
  return words_indexes;
}

int text_parser::get_length_text () const
{
  return text.length ();
}

const std::vector<text_title> &text_parser::get_titles () const
{
  return titles;
}
