#pragma once
#include <vector>
#include <iostream>

namespace nonogram
{
typedef std::vector<unsigned> Line;
typedef std::vector<Line> LineVector;

struct Description
{
  LineVector rows;
  LineVector columns;
  
  size_t width() const { return columns.size(); }
  size_t height() const { return rows.size(); }
};

std::ostream& operator<<(std::ostream&, const Description&);
std::istream& operator>>(std::istream&, Description&);

LineVector enumerateLine(const Line& description, size_t size, size_t lowestCell);

/////////////////////////////////////////////////////////////////////////////

class Grid
{
  using Cells = std::vector<int>;
  
  const Description& _description;
  Cells _cells;
  
public:
  Grid(const Description& d) : _description(d), _cells(d.width()*d.height())
  { }
};

/////////////////////////////////////////////////////////////////////////////



}