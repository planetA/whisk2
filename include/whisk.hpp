#pragma once

#include <memory>

#include <boost/program_options.hpp>

#include <whisk/driver.hpp>
#include <whisk/metrics.hpp>

namespace Whisk
{
  class Context
  {
    Context(int &argc, char *argv[]);

    // static Scheduler *createScheduler(int &argc, char *argv[]);
  private:
  public:
    static void init(int &argc, char *argv[]);

    // Map of all configuration variables
    po::variables_map vm;
    std::unique_ptr<Driver> driver;
    Metrics metrics;
  private:
  };

  extern std::unique_ptr<Context> wh;
}
