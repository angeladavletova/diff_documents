#pragma once

#include <string>

enum class document_structure;

std::string read_text_from_file (std::string &text, std::string file_path, document_structure &structure);
std::string read_documents (std::string &text1, std::string &text2, document_structure &structure);
