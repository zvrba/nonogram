#include <iostream>
#include <fstream>
#include <fstream>
#include "Solver.h"

using namespace nonogram;

static void test_enumerate()
{
  LineDescription line{2,1};
  auto e = enumerateColorings(line, 5);
  
  for (const auto& l : e) {
    std::cout << l << '\n';
  }
}

int main(int argc, char** argv)
{
  try {
    if (argc != 2)
      throw std::runtime_error("input file argument missing");

    std::ifstream ifs(argv[1]);
    if (!ifs)
      throw std::runtime_error("cannot open file");
    
    Description d;
    if (!(ifs >> d))
      throw std::runtime_error("cannot parse input file");
    
    std::vector<Solver::Solution> solutions;
    Solver solver(d, solutions);
    solver.solve();
    std::cout << "found " << solutions.size() << " solutions\n";
  } catch (std::exception& err) {
    std::cerr << "ERROR: " << err.what() << '\n';
    return 1;
  }
  return 0;
}


