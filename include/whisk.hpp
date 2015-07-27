#pragma once

#include <memory>

#include <boost/program_options.hpp>

#include <whisk/driver.hpp>

namespace Whisk
{
  class Context
  {
    Context(int &argc, char *argv[]);

    void createDriver(int &argc, char *argv[]);
    // static Scheduler *createScheduler(int &argc, char *argv[]);
  public:
    static void init(int &argc, char *argv[]);
  private:
    // Map of all configuration variables
    boost::program_options ::variables_map vm;
    std::unique_ptr<Driver> driver;
  };

  // extern std::unique_ptr<Context> wh;
}
