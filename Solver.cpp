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



}
