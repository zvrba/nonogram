#include <iostream>
#include <fstream>
#include "Representation.h"

int main(int argc, char** argv)
{
  using namespace nonogram;
  
  Line line{1,1};
  auto e = enumerateLine(line, 5, 0);
  
  for (const auto& l : e) {
    for (auto v : l) std::cout << v << ' ';
    std::cout << '\n';
  }
  
  return 0;
}

