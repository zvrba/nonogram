#include <iostream>
#include <fstream>
#include "Representation.h"
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
  test_enumerate();
  return 0;
}


