#pragma once
#include <vector>
#include <iostream>
#include <tuple>

namespace nonogram
{
typedef std::vector<unsigned> LineDescription;   //! Description of a single row or column

//! This is input to the solver.
struct Description
{
  std::vector<LineDescription> rows;
  std::vector<LineDescription> columns;
  
  size_t width() const { return columns.size(); }
  size_t height() const { return rows.size(); }
};

std::ostream& operator<<(std::ostream&, const Description&);
std::istream& operator>>(std::istream&, Description&);

//! Block is a consecutive line of colored cells.  The tuple is [start,end) range.
//! In a valid coloring, length must match the corresponding length from description.
using Block = std::tuple<size_t, size_t>;
using LineColoring = std::vector<Block>;

std::ostream& operator<<(std::ostream& os, const LineColoring&);

//! Returns a list of all valid colorings of a line; the coloring is described as a vector of blocks.
std::vector<LineColoring> enumerateColorings(const LineDescription& description, size_t size);

}