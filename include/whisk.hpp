#pragma once

#include <whisk/driver.hpp>

namespace Whisk
{
  class Wh
  {
  public:
    static Driver *createDriver(int &argc, char *argv[]);
    // static Scheduler *createScheduler(int &argc, char *argv[]);
  };
}
