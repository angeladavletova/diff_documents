#include <fstream>
#include <iostream>
#include "io.h"
#include "document_structure.h"

//#define INPUTFILE

std::string read_text_from_file (std::string &text, std::string file_path, document_structure &structure)
{
  std::string s;

  std::ifstream file (file_path); // for example ./srcdata/pdd19.txt
  if (!file)
    return file_path + " does not exist\n";

  structure = document_structure::clause;
  while (std::getline(file, s))
    {
      text += " " + s;
      if (s.find ("Статья 1.") != std::string::npos)
        structure = document_structure::article;
    }
  file.close();

  return {};
}

std::string read_documents (std::string &text1, std::string &text2, document_structure &structure)
{

  std::string file1_path = "pdd18.txt", file2_path = "pdd19.txt";
  //std::string file1_path = "../tests/fz1.txt", file2_path = "../tests/fz2.txt";
#ifdef INPUTFILE
  std::cout << "Enter file1 path\n";
  std::cin >> file1_path;

  std::cout << "Enter file2 path\n";
  std::cin >> file2_path;
#endif

  //std::ifstream file1 (file1_path); // for example ./srcdata/pdd19.txt
  //std::ifstream file2 (file2_path);

  document_structure structure1 = document_structure::article;
  document_structure structure2 = document_structure::article;

  std::string error1 = read_text_from_file (text1, file1_path, structure1);
  std::string error2 = read_text_from_file (text2, file2_path, structure2);

  std::string all_error = error1 + error2;
  if (!all_error.empty ())
    return all_error;

  if (structure1 != structure2)
    return "Documents have different structures\n";

  structure = structure1;
  return {};
}
