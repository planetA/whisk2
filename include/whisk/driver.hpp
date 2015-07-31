#pragma once

#include <string>
#include <vector>

#include <boost/program_options.hpp>

namespace Whisk
{
  namespace po = boost::program_options;

  class Driver
  {
  protected:
  public:
    // If an application accepts command line options, it should
    // redefine this function
    virtual void append_options(po::options_description &) {};
    virtual void init(std::vector<std::string>&);

    // update variables map
    Driver();
    virtual ~Driver() {};
  };
}
