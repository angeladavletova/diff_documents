#include <string>
#include <fstream>
#include <ctime>
#include <math.h>
#include <iostream>
#include "diff_searcher.h"
#include "io.h"
#include "text_title.h"

int main ()
{
//  int n = 10;
//  std::string a = std::to_string (n);
//  std::string b = "1.3.";
//  if (a > b)
//    std::cout << "true";
//  else
//    std::cout << "false";
  setlocale (LC_ALL, "Russian");

  clock_t start = clock();

  std::string text1, text2;
  title_type type;
  std::string error = read_documents (text1, text2, type);
  if (!error.empty ())
    {
      std::cout << error;
      return 0;
    }

  diff_searcher searcher (text1, text2, type);
  const std::vector<simple_diff> &diffs = searcher.build_diff ();

  clock_t end = clock();
  double seconds = floor(((double)(end - start) / CLOCKS_PER_SEC * 100) + 0.5 ) / 100;

  std::ofstream fout_time ("time.txt");
  fout_time << seconds;

  std::ofstream fout ("diff.txt");
  for (const simple_diff &diff : diffs)
    diff.print (fout, text1, text2, type);

  fout_time.close ();
  fout.close ();

  return 0;
}

