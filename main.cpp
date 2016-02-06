#include <iostream>
#include <fstream>
#include "Representation.h"

int main(int argc, char** argv)
{
  using namespace nonogram;
  std::ifstream ifs("inputs/1.txt");
  Description d;
  
  if (ifs >> d) {
    std::cout << "W: " << d.width() << " H: " << d.height() << "\n";
    std::cout << d << "\n";
  }
  return 0;
}

