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
    if (pos < blockEnd)
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
  if (_coloring == _enumeratedColorings.end())
    return false;
  if (_coloring != _enumeratedColorings.begin())
    ++_coloring;
  
  while(true) {
    size_t invalidBlock = placeBlocks();
    if (invalidBlock == _coloring->size())
      return true;
    skipColorings(invalidBlock);
  }
  return false;
}

//! Try to place blocks of the current coloring.  Returns the index of the first
//! block that could not be placed; or size() of the coloring on success.
size_t RowAgent::placeBlocks()
{
  using std::get;
  
  const LineColoring& coloring = *_coloring;
  
  std::vector<CellPlacer> placedCells;
  size_t block;
  
  for (block = 0; block < coloring.size(); ++block) {
    size_t blockBegin = get<0>(coloring[block]);
    size_t blockEnd = get<1>(coloring[block]);
    
    for (; blockBegin < blockEnd; ++blockBegin) {
      placedCells.push_back(CellPlacer(_columnAgents[blockBegin].get(), _row));
      if (!placedCells.back())
        goto bailout;
    }
  }
  
bailout:
  if (block == coloring.size())
  for (auto& p : placedCells)
    p.commit();
  
  return block;
}

//! Skip to the next coloring NOT containing the block equal to that with invalidBlock
//! index in the current coloring.
void RowAgent::skipColorings(size_t invalidBlock)
{

}

}
