#pragma once
#include <boost/range/adaptors.hpp>
#include "Representation.h"

namespace nonogram
{

//! Grid abstraction; contains convenience methods to return rows and columns as
//! mutable random-access ranges.
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

//! Verifies whether RowAgent's placement of the cell is consistent with column description.
class ColumnAgent
{
  const size_t _column;
  const LineDescription& _description;
  const size_t _lineSize;
  std::vector<Block> _blocks;

public:
  ColumnAgent(size_t column, const LineDescription& description, size_t lineSize) :
    _column(column), _description(description), _lineSize(lineSize)
  { }

  bool setCell(size_t pos);
  void clearCell(size_t pos);
};

/////////////////////////////////////////////////////////////////////////////

//! Tries all line colorings consistent with column descriptions.
class RowAgent
{
  const size_t _row;
  const std::vector<LineColoring> _enumeratedColorings;
  std::vector<ColumnAgent>& _columnAgents;
  std::vector<LineColoring>::const_iterator _coloring;
  
  class CellPlacer
  {
    ColumnAgent& _columnAgent;
    size_t _row;
    bool _rollback;
    bool _placed;
    
  public:
    CellPlacer(ColumnAgent& columnAgent, size_t row) :
      _columnAgent(columnAgent), _row(row), _rollback(true), _placed(_columnAgent.setCell(_row))
    { }
    
    CellPlacer(CellPlacer&& other) :
      _columnAgent(other._columnAgent), _row(other._row), _rollback(other._rollback), _placed(other._placed)
    {
      other._rollback = false;
    }

    ~CellPlacer()
    {
      if (_rollback)
        _columnAgent.clearCell(_row);
    }
    
    CellPlacer(const CellPlacer&) = delete;
    CellPlacer& operator=(const CellPlacer&) = delete;
    CellPlacer& operator=(CellPlacer&&) = delete;

    operator bool() const
    {
      return _placed;
    }
    
    void commit()
    {
      _rollback = false;
    }
  };
  
  size_t placeBlocks();
  void skipColorings(size_t invalidBlock);
  
public:
  RowAgent(size_t row, const std::vector<LineColoring>& enumeratedColorings, std::vector<ColumnAgent>& columnAgents) :
    _row(row), _enumeratedColorings(enumeratedColorings), _columnAgents(columnAgents)
  {
    reset();
  }
    
  void reset()
  {
    _coloring = _enumeratedColorings.begin();
  }
  
  const LineColoring& coloring() const
  {
    return *_coloring;
  }
  
  bool next();
};

/////////////////////////////////////////////////////////////////////////////

class Solver
{
public:
  //! Solution is a vector of row colorings.
  using Solution = std::vector<LineColoring>;
  
private:
  const Description& _description;
  std::vector<Solution>& _solutions;
  std::vector<ColumnAgent> _columnAgents;
  std::vector<RowAgent> _rowAgents;
  
  void buildAgents();
  void solve(size_t i);
  
public:
  Solver(const Description& description, std::vector<Solution>& solutions) :
    _description(description), _solutions(solutions)
  {
    buildAgents();
  }
  
  void solve()
  {
    solve(0);
  }
};

}