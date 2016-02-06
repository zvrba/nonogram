#pragma once
#include <vector>
#include <iostream>
#include <boost/coroutine2/all.hpp>

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