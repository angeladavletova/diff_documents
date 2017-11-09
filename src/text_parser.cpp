#include "text_parser.h"

text_parser::text_parser (const std::string &str, std::set<std::string> &w_set) : text (str), words_set (w_set)
{
  parse ();
}

void text_parser::parse () 
{
  std::string current_word;
  int length_text = text.length (); 
  for (int i = 0; i < length_text; i++) 
    {
      if ((text[i] >= 'à' && text[i] <= 'ÿ') || (text[i] >= 'À' && text[i] <= 'ß') || (text[i] >= '0' && text[i] <= '9'))
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
