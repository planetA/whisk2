#pragma once

#include <memory>

#include <boost/program_options.hpp>

#include <whisk/driver.hpp>

namespace Whisk
{
  class Context
  {
    Context(int &argc, char *argv[]);

    // static Scheduler *createScheduler(int &argc, char *argv[]);
  public:
    static void init(int &argc, char *argv[]);

    // Map of all configuration variables
    po::variables_map vm;

  private:
    std::unique_ptr<Driver> driver;
  };

  extern std::unique_ptr<Context> wh;
}
