#pragma once
#include "Representation.h"

namespace nonogram
{

class Grid
{
  using Cells = std::vector<int>;
  
  const Description& _description;
  Cells _cells;
  
public:
  Grid(const Description& d) : _description(d), _cells(d.width()*d.height())
  { }
  
  int& cell(size_t row, size_t col)
  {
    return _cells[row*_description.width()+col];
  }
};

}