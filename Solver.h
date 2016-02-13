#pragma once
#include <boost/range/adaptors.hpp>
#include "Representation.h"

namespace nonogram
{

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
  class CellPlacer
  {
    ColumnAgent& _columnAgent;
    size_t _row;
    bool _placed;
    
  public:
    CellPlacer(ColumnAgent& columnAgent, size_t row) :
      _columnAgent(columnAgent), _row(row), _placed(_columnAgent.setCell(_row))
    { }
    
    CellPlacer(CellPlacer&& other) :
      _columnAgent(other._columnAgent), _row(other._row), _placed(other._placed)
    {
      other._placed = false;
    }

    ~CellPlacer()
    {
      if (_placed)
        _columnAgent.clearCell(_row);
    }
    
    CellPlacer(const CellPlacer&) = delete;
    CellPlacer& operator=(const CellPlacer&) = delete;
    CellPlacer& operator=(CellPlacer&&) = delete;

    operator bool() const
    {
      return _placed;
    }
  };
  
  static constexpr size_t nocoloring = static_cast<size_t>(-1);
  const size_t _row;
  const std::vector<LineColoring> _enumeratedColorings;
  std::vector<ColumnAgent>& _columnAgents;
  size_t _coloring;
  std::vector<CellPlacer> _placedCells;

  size_t placeBlocks();
  void skipColorings(size_t invalidBlock);
  
public:
  RowAgent(size_t row, const std::vector<LineColoring>& enumeratedColorings, std::vector<ColumnAgent>& columnAgents) :
    _row(row), _enumeratedColorings(enumeratedColorings), _columnAgents(columnAgents), _coloring(nocoloring)
  {
  }
    
  RowAgent(RowAgent&& other) :
    _row(other._row), _enumeratedColorings(std::move(other._enumeratedColorings)),
    _columnAgents(other._columnAgents), _coloring(other._coloring),
    _placedCells(std::move(other._placedCells))
  { }

  void reset()
  {
    if (_coloring < _enumeratedColorings.size())
      throw std::logic_error("RowAgent::next: agent not exhausted");
    _coloring = static_cast<size_t>(-1);
    _placedCells.clear();
  }
  
  const LineColoring& coloring() const
  {
    return _enumeratedColorings[_coloring];
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