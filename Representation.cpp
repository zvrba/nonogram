#include <boost/io/ios_state.hpp>
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/qi_match.hpp>
#include <boost/spirit/include/karma.hpp>
#include <boost/spirit/include/karma_format.hpp>
#include <boost/spirit/include/support_istream_iterator.hpp>
#include <boost/spirit/include/phoenix_core.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/spirit/include/phoenix_object.hpp>
#include <boost/fusion/include/adapt_struct.hpp>
#include "Representation.h"

// Must be in global scope.
BOOST_FUSION_ADAPT_STRUCT(
  nonogram::Description,
  (nonogram::LineVector, rows)
  (nonogram::LineVector, columns)
);

namespace nonogram
{

using namespace boost::spirit;

template<typename It>
struct parser : qi::grammar<It, Description(), qi::ascii::space_type>
{
  parser() : parser::base_type(start)
  {
    using namespace qi::labels;
    using qi::lit;
    using qi::uint_;
    
    start %= 
      lit("Nonogram") >> lit('{') >>
      line_vector(std::string("Rows")) >>
      line_vector(std::string("Columns")) >>
      lit('}');

    line_vector %= lit(_r1) >> lit('{') >> +line >> lit('}');

    line %= lit('[') >> *uint_ >> lit(']');
  }
  
  qi::rule<It, Description(), qi::ascii::space_type> start;
  qi::rule<It, LineVector(std::string), qi::ascii::space_type> line_vector;
  qi::rule<It, Line(), qi::ascii::space_type> line;
};

template<typename It>
struct formatter : karma::grammar<It, Description(), karma::ascii::space_type>
{
  formatter() : formatter::base_type(start)
  {
    using namespace karma::labels;
    using karma::lit;
    using karma::uint_;
    
    start =
      lit("Nonogram {") <<
      line_vector(std::string("Rows")) <<
      line_vector(std::string("Columns")) <<
      lit('}');
    
    line_vector = lit(_r1) << lit('{') << +line << lit('}');
    
    line = lit('[') << *uint_ << lit(']');
  }
  
  karma::rule<It, Description(), karma::ascii::space_type> start;
  karma::rule<It, LineVector(std::string), qi::ascii::space_type> line_vector;
  karma::rule<It, Line(), qi::ascii::space_type> line;
};

std::ostream& operator<<(std::ostream& os, const Description& d)
{
  using namespace boost::spirit;
  using ascii::space;
  
  static formatter<ostream_iterator> formatter;
  
  os << karma::format_delimited(formatter, space, d);
  return os;
}

std::istream& operator>>(std::istream& is, Description &d)
{
  using namespace boost::spirit;
  using ascii::space;

  static parser<istream_iterator> parser;
  
  boost::io::ios_flags_saver flags(is);
  Description tmp;
  if (is >> std::noskipws >> qi::phrase_match(parser, space, tmp))
    d = std::move(tmp);
  return is;
}

/////////////////////////////////////////////////////////////////////////////

class LineEnumeratorState
{
  const Line& _description;
  const Line _max;
  Line _current;
  
  static Line calculateMaximums(const Line& description, size_t size);
  
  bool reset(size_t startIndex);

public:
  LineEnumeratorState(const Line& description, size_t size, size_t lowestCell) :
    _description(description),
    _max(calculateMaximums(_description, size)),
    _current(_description.size())
  {
    _current[0] = lowestCell;
    reset(0);
  }

  void next(size_t i, LineVector& result);
};

Line LineEnumeratorState::calculateMaximums(const Line& description, size_t size)
{
  size_t count = description.size();
  Line maximums(count);

  while (count--) {
    if (description[count] > size)
      throw std::runtime_error("invalid constraint; line too short");
    if ((maximums[count] = size - description[count]) > 0)
      size = maximums[count] - 1;
  }
  return maximums;
}

bool LineEnumeratorState::reset(size_t startIndex)
{
  for (size_t i = startIndex+1; i < _description.size(); ++i)
  if ((_current[i] = _current[i-1] + _description[i-1] + 1) > _max[i])
    return false;
  return true;
}

void LineEnumeratorState::next(size_t i, LineVector& result)
{
  if (i == _description.size()) {
    result.push_back(_current);
    return;
  }
  
  while (_current[i] <= _max[i]) {
    next(i+1, result);
    ++_current[i];
    reset(i);
  }
}

LineVector enumerateLine(const Line& description, size_t size, size_t lowestCell)
{
  LineVector result;
  LineEnumeratorState state(description, size, lowestCell);
  state.next(0, result);
  return result;
}

}
