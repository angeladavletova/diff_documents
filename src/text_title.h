#pragma once

#include <string>
#include <vector>

enum class title_type
{
  article,
  clause
};

struct text_title
{
  std::string title;
  int place_in_text;
};

std::vector<text_title> build_text_titles (const std::string &text, title_type type);
