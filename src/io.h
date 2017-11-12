#pragma once

#include <string>

enum class title_type;

std::string read_text_from_file (std::string &text, std::string file_path, title_type &structure);
std::string read_documents (std::string &text1, std::string &text2, title_type &structure);
