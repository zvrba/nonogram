#include <boost/io/ios_state.hpp>
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/qi_match.hpp>
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

Description parse(std::istream& is)
{
  using namespace boost::spirit;
  using ascii::space;
  static parser<istream_iterator> parser;
  
  boost::io::ios_flags_saver flags(is);
  Description tmp;
  is >> std::noskipws >> qi::phrase_match(parser, space, tmp);
  return tmp;
}

}
