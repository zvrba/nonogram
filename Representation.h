#pragma once
#include <vector>
#include <iostream>

namespace nonogram
{
typedef std::vector<unsigned> Line;   //! Description of a single row or column
typedef std::vector<Line> LineVector;

//! This is input to the solver.
struct Description
{
  LineVector rows;
  LineVector columns;
  
  size_t width() const { return columns.size(); }
  size_t height() const { return rows.size(); }
};

std::ostream& operator<<(std::ostream&, const Description&);
std::istream& operator>>(std::istream&, Description&);

//! Returns a list of all valid colorings of a line.  Each element gives the
//! starting position of a cell block of length corresponding to the description.
LineVector enumerateLine(const Line& description, size_t size, size_t lowestCell);

}