#include <iostream>
#include <fstream>
#include "text_parser.h"
#include "text_title.h"

text_parser::text_parser (const std::string &str, std::set<std::string> &w_set, title_type type)
  : text (str), words_set (w_set)
{
  titles = build_text_titles (text, type);
  std::ofstream fout_time ("res.txt");
  for (const auto &title : titles)
    fout_time << title.title << "\n" << title.place_in_text << "\n\n";
  fout_time.close ();
  parse ();
}

void text_parser::parse () 
{
  std::string current_word;
  int length_text = text.length (); 
  for (int i = 0; i < length_text; i++) 
    {
      if ((text[i] >= 'à' && text[i] <= 'ÿ') || (text[i] >= 'À' && text[i] <= 'ß') /*|| (text[i] >= '0' && text[i] <= '9')*/)
        {
          current_word += text[i];
        }
      else if (!current_word.empty ()) 
        {
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
