#include <string>
#include <fstream>
#include <ctime>
#include <math.h>
#include <iostream>
#include "diff_searcher.h"
#include "io.h"
#include "document_structure.h"

#define INPUTFILE

//#include <regex>
int main ()
{
  setlocale (LC_ALL, "Russian");

  clock_t start = clock();

  std::string text1, text2;
  document_structure structure;
  std::string error = read_documents (text1, text2, structure);
  if (!error.empty ())
    {
      std::cout << error;
      return 0;
    }

  diff_searcher searcher (text1, text2);
  const std::vector<simple_diff> &diffs = searcher.build_diff ();

  clock_t end = clock();
  double seconds = floor(((double)(end - start) / CLOCKS_PER_SEC * 100) +0.5 ) / 100;

  std::ofstream fout_time ("time.txt");
  fout_time << seconds;

  std::ofstream fout ("diff.txt");
  for (const simple_diff &diff : diffs)
    diff.print (fout, text1, text2);

  fout_time.close ();
  fout.close ();

  return 0;
}

