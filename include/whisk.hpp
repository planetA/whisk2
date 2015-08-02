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
  private:
    // helpers for init function
    static inline po::options_description create_options_generic();
    static inline po::options_description create_options_simulation();
    static inline void parse_config_file(const po::options_description &);
    static inline void parse_command_line(int argc, char *argv[],
                                          const po::options_description &);
  public:
    static void init(int &argc, char *argv[]);

    // Map of all configuration variables
    po::variables_map vm;

  private:
    std::unique_ptr<Driver> driver;
  };

  extern std::unique_ptr<Context> wh;
}
