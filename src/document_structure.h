#pragma once

#include <string>
#include <vector>

enum class document_structure
{
  article,
  clause
};

struct text_title
{
  std::string title;
  int place_in_text;
};

std::vector<text_title> build_text_clauses (const std::string &text);
