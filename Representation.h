#pragma once
#include <vector>
#include <iostream>

namespace nonogram
{
typedef std::vector<unsigned> Line;
typedef std::vector<Line> LineVector;

struct Description
{
  LineVector rows;
  LineVector columns;
};

Description parse(std::istream&);

}