#include <iostream>
#include <fstream>
#include "Solver.h"

using namespace nonogram;
using std::cout;

static void print_solution(const Solver::Solution& solution)
{
  for (const LineColoring& line : solution) {
    size_t column = 0;
    for (const Block& block : line) {
      const auto blockBegin = std::get<0>(block);
      const auto blockEnd = std::get<1>(block);
      for (; column < blockBegin; ++column)
        cout << '.';
      for (; column < blockEnd; ++column)
        cout << '#';
    }
    cout << '\n';
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
    
    cout << "FOUND " << solutions.size() << " SOLUTIONS\n";
    for(size_t i = 0; i < solutions.size(); ++i) {
      cout << "\n\nSOLUTION " << i << '\n';
      print_solution(solutions[i]);
    }
  } catch (std::exception& err) {
    std::cerr << "ERROR: " << err.what() << '\n';
    return 1;
  }
  return 0;
}


