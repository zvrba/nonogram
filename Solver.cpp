#include <boost/range/algorithm.hpp>
#include "Solver.h"

namespace nonogram
{


bool ColumnAgent::setCell(size_t pos)
{
  using std::get;
  
  if (pos >= _lineSize)
    throw std::logic_error("ColumnAgent::setCell: invalid pos");
  if (_blocks.size() > _description.size())
    throw std::logic_error("Agent::setCell: too many blocks");

  // Determine whether to extend existing block or create a new one.
  if (!_blocks.empty())
  {
    auto& lastBlock = _blocks.back();

    const auto lastBlockIndex = _blocks.size()-1;
    const auto blockBegin = get<0>(lastBlock);
    const auto blockEnd = get<1>(lastBlock);

    if (blockEnd - blockBegin > _description[lastBlockIndex])
      throw std::logic_error("Agent::setCell: too long block");
    if (pos < blockEnd-1)
      throw std::logic_error("Agent::setCell: going backwards");

    // Extending last block if possible.
    if (pos == blockEnd) {
      if (blockEnd - blockBegin == _description[lastBlockIndex])
        return false;
      ++get<1>(lastBlock);
      return true;
    }
    // Not extending last block, proceed to create new.
  }
  
  // New block.
  if (_blocks.size() == _description.size())
    return false;
  _blocks.push_back(Block(pos, pos+1));
  return true;
}

void ColumnAgent::clearCell(size_t pos)
{
  using std::get;
  
  if (pos >= _lineSize)
    throw std::logic_error("ColumnAgent::clearCell: invalid pos");
  if (_blocks.empty())
    throw std::logic_error("Agent::clearCell: no blocks");
  
  auto& lastBlock = _blocks.back();
  
  if (pos != get<1>(lastBlock)-1)
    throw std::logic_error("Agent::clearCell: not last cell");
  if (get<1>(lastBlock) == get<0>(lastBlock))
    throw std::logic_error("Agent::clearCell: empty block");
  
  if (--get<1>(lastBlock) == get<0>(lastBlock))
    _blocks.pop_back();
}

/////////////////////////////////////////////////////////////////////////////

//! Generate next valid line coloring.
bool RowAgent::next()
{
  if (_enumeratedColorings.empty())
    return true;
  if (_coloring == _enumeratedColorings.size())
    return false;

  ++_coloring;
  
  while(_coloring != _enumeratedColorings.size()) {
    _placedCells.clear();
    size_t invalidBlock = placeBlocks();
    if (invalidBlock == coloring().size())
      return true;
    skipColorings(invalidBlock);
  }
  return false;
}

//! Skip to the next coloring NOT containing the block equal to that with invalidBlock
//! index in the current coloring.  Colorings are sorted lexicographically.
void RowAgent::skipColorings(size_t invalidBlock)
{
  const Block block = coloring()[invalidBlock];
  for (; _coloring != _enumeratedColorings.size(); ++_coloring)
  if (boost::find(coloring(), block) == boost::end(coloring()))
    break;
}

//! Try to place blocks of the current coloring.  Returns the index of the first
//! block that could not be placed; or size() of the coloring on success.
size_t RowAgent::placeBlocks()
{
  using std::get;
  size_t block;
  
  //static size_t attempt = 0;
  //std::cout << std::setw(4) << attempt++ << " ROW " << _row << " TRY: " << coloring() << "\n";

  for (block = 0; block < coloring().size(); ++block) {
    size_t blockBegin = get<0>(coloring()[block]);
    size_t blockEnd = get<1>(coloring()[block]);
    
    for (; blockBegin < blockEnd; ++blockBegin) {
      _placedCells.push_back(CellPlacer(_columnAgents[blockBegin], _row));
      if (!_placedCells.back())
        goto bailout;
    }
  }
  
bailout:
  return block;
}

/////////////////////////////////////////////////////////////////////////////

void Solver::buildAgents()
{
  for (size_t i = 0; i < _description.width(); ++i)
    _columnAgents.push_back(ColumnAgent(i, _description.columns[i], _description.height()));
  for (size_t i = 0; i < _description.height(); ++i) {
    auto rowColorings = enumerateColorings(_description.rows[i], _description.width());
    _rowAgents.push_back(RowAgent(i, rowColorings, _columnAgents));
  }
}

void Solver::solve(size_t i)
{
  if (i == _rowAgents.size()) {
    Solution solution;
    for (const auto& a : _rowAgents)
      solution.push_back(a.coloring());
    _solutions.push_back(solution);
    return;
  }
  
  while (_rowAgents[i].next())
    solve(i+1);
  
  for (; i < _rowAgents.size(); ++i)
    _rowAgents[i].reset();
}

}
