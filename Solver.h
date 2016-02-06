#pragma once
#include <boost/signals2.hpp>
#include "Representation.h"

namespace nonogram
{

namespace signals = boost::signals2;

//! Grid abstraction; contains convenience methods to return rows and columns as ranges.
class Grid
{
  const Description& _description;
  
public:
  using Cells = std::vector<int>;
  Cells cells;

  Grid(const Description& d) :
    _description(d),
    cells(_description.width() * _description.height())
  { }

  size_t width() const { return _description.width(); }
  size_t height() const { return _description.height(); }
  
  const Line& rowDescription(size_t row) const { return _description.rows.at(row); }
  const Line& columnDescription(size_t column) const { return _description.columns.at(column); }
  
  auto row(size_t row)
  {
    using namespace boost::adaptors;
    if (row >= height())
      throw std::invalid_argument("Grid::row()");
    auto b = row * width();
    return cells | sliced(b, b + width());
  }
  
  auto column(size_t column)
  {
    using namespace boost::adaptors;
    if (column >= width())
      throw std::invalid_argument("Grid::column()");
    return cells | sliced(column, cells.size()) | strided(width());
  }
};

/////////////////////////////////////////////////////////////////////////////

//! Checks whether the current state of the board satisfies the column description.
class ColumnAgent
{
  Grid& _grid;
  size_t _column;
  
public:
  ColumnAgent(Grid& grid, size_t column) : _grid(grid), _column(column)
  { }
  bool operator()(size_t column) const;
};

/////////////////////////////////////////////////////////////////////////////

class RowAgent
{
  Grid& _grid;
  LineVector _validLines;
  
  struct and_combiner
  {
    typedef bool result_type;

    template<typename It>
    bool operator()(It begin, It end) const
    {
      while (begin != end)
      if (!*begin++)
        return false;
      return true;
    }
  };
  
public:
  RowAgent(Grid& grid, const LineVector& lv);
  void run();
  // TODO! Disable mutex!!
  signals::signal<bool(size_t), and_combiner> columnChanged;
};


}