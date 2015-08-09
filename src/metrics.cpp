#include <fstream>
#include <iterator>
#include <string>

#include <whisk.hpp>
#include <whisk/metrics.hpp>

#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix_core.hpp>
#include <boost/spirit/include/phoenix_stl.hpp>
#include <boost/spirit/include/qi_char_class.hpp>
#include <boost/fusion/include/adapt_struct.hpp>

using namespace Whisk;
namespace qi   = boost::spirit::qi;
namespace ascii = boost::spirit::ascii;

class ParamParser
{
};

class GlobalParser
{
};

Metrics::Metrics()
  : active(false)
{}

struct Parameter
{
  std::string name;
  double val;
// public:
//   Parameter(std::string name, double val) : name(name), val(val)
//     {
//       std::cout << "Hi from parameter creation"  << std::endl;
//     }

  friend std::ostream& operator<<(std::ostream& os, const Parameter& obj)
    {
      os << obj.name << " = " << obj.val;
      // write obj to stream
      return os;
    }
};

BOOST_FUSION_ADAPT_STRUCT(
  ::Parameter,
  (std::string, name)
  (double, val)
)

template <typename Iterator>
struct ParameterParser : qi::grammar<Iterator, Parameter(), ascii::space_type>
{
  ParameterParser() : ParameterParser::base_type(start)
  {
    using qi::int_;
    using qi::lit;
    using qi::double_;
    using qi::lexeme;
    using ascii::char_;
    using ascii::alpha;
    using ascii::alnum;

    start %=
      attr_name_rule
      >> double_
      // >>  val_rule
      ;
  }

  qi::rule<Iterator, std::string()> attr_name_rule = ascii::alpha >> *ascii::alnum;
  // qi::rule<Iterator, double> val_rule = qi::double_;
  qi::rule<Iterator, Parameter(), ascii::space_type> start;
};

void Metrics::read_file(std::ifstream &ifs)
{
  namespace phoenix = boost::phoenix;
  using boost::spirit::ascii::space;
  using boost::spirit::ascii::char_;
  using phoenix::push_back;
  using qi::phrase_parse;
  using qi::_1;

  std::string line;

  std::vector<std::string> a;
  std::vector<double> v;
  std::vector<Parameter> p;
  typedef std::string::const_iterator iterator_type;
  ParameterParser<iterator_type> g;
  bool ok = true;

  qi::rule<iterator_type, std::vector<Parameter>(), ascii::space_type> parameters_parser = '{' >> g % ',' >> '}';


  while (ok && std::getline<char>(ifs, line))
  {
    std::string::const_iterator begin = line.begin(), end = line.end();
    ok = qi::phrase_parse(begin, end, parameters_parser, space, p);
    // ok = qi::phrase_parse(begin, end, parameter_rule, space, p);
    // if (!ok)
    //   std::cout << "Failed to parse line: " << line << std::endl;
    // ok = qi::phrase_parse(begin, end,
    //                       // begin grammar
    //                       // "{" >>
    //                       val_rule
    //                       // parameter_rule
    //                       // parameters_parser
    //                       // +(+char_)[std::cout << _1 << std::endl] >>
    //                       // *(+char_) % qi::space >>
    //                       // (+alpha[push_back(phoenix::ref(_1), a)] >> qi::double_) % ',' >>
    //                       // (+char_ - ',')[phoenix::push_back(phoenix::ref(a), _1)] % ','>>
    //                       // +(char_ - ',')[phoenix::push_back(phoenix::ref(a), _1)] % ','>>
    //                       // (+char_ >> qi::double_)[push_back(phoenix::ref(_2), v),push_back(phoenix::ref(_1), a)] % ',' >>
    //                       // +char_ >>
    //                       // "}"
    //                       // end grammar
    //                       , space);

  }
  std::cout << "Print parsed metrics file: " << ok << std::endl;

  for (auto i : v)
    std::cout << i << std::endl;
  for (auto i : a)
    std::cout << i << std::endl;
  for (auto i : p)
    std::cout << i << std::endl;
  std::cout << "Print end" << std::endl;
}

void Metrics::init()
{
  if (!wh->vm.count("metrics"))
  {
    // Failed to open the file
    BOOST_LOG_TRIVIAL(info) << "Metrics file has not been provided:"
                            << std::endl;
    // Metrics table is inactive. Just return
    return;
  }

  std::string config_file(wh->vm["metrics"].as<std::string>());
  std::ifstream ifs(config_file);
  if (ifs)
  {
    read_file(ifs);
  }
  else
  {
    // Failed to open the file
    BOOST_LOG_TRIVIAL(error) << "Could not open metrics file:"
                             << config_file << std::endl;
    throw std::runtime_error("Could not open config file.");
  }
}
