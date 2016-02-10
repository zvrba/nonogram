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

static void test_grid()
{
  using std::cout;
  using std::endl;
  
  Grid g(5,7);
  
  for (int i = 0; i < 35; ++i)
    g.cells()[i] = i;
  
  cout << "ROWS:\n";
  for (int r = 0; r < g.height(); ++r) {
    auto row = g.row(r);
    row[0] += 10;
    for (auto v: row) cout << v << " ";
    cout << endl;
  }

  cout << "\nCOLUMNS:\n";
  for (int c = 0; c < g.width(); ++c) {
    auto col = g.column(c);
    col[1] += 11;
    for (auto v: col) cout << v << " ";
    cout << endl;
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
    std::cout << "found " << solutions.size() << " solutions\n";
  } catch (std::exception& err) {
    std::cerr << "ERROR: " << err.what();
    return 1;
  }
  return 0;
}


