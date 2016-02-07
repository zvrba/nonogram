#pragma once
#include <boost/range/adaptors.hpp>
#include <boost/signals2.hpp>
#include "Representation.h"

namespace nonogram
{

namespace signals = boost::signals2;

//! Grid abstraction; contains convenience methods to return rows and columns as ranges.
class Grid
{
public:
  using Cells = std::vector<int>;

  Grid(size_t w, size_t h) :
    _width(w), _height(h), _cells(_width * _height)
  { }

  Cells& cells() { return _cells; }
  size_t width() const { return _width; }
  size_t height() const { return _height; }

  int& at(size_t r, size_t c)
  {
    if (r >= height() || c >= width())
      throw std::out_of_range("Grid::at()");
    return _cells[r*width()+c];
  }
  
  auto row(size_t row)
  {
    using namespace boost::adaptors;
    if (row >= height())
      throw std::invalid_argument("Grid::row()");
    auto b = row * width();
    return _cells | sliced(b, b + width());
  }
  
  auto column(size_t column)
  {
    using namespace boost::adaptors;
    if (column >= width())
      throw std::invalid_argument("Grid::column()");
    return _cells | sliced(column, _cells.size()) | strided(width());
  }
  
private:
  size_t _width, _height;
  Cells _cells;
};

/////////////////////////////////////////////////////////////////////////////


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

using LineChangedSignal = signals::signal_type<
        bool(size_t),
        signals::keywords::combiner_type<and_combiner>,
        signals::keywords::mutex_type<signals::dummy_mutex>>::type;



}