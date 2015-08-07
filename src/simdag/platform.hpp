#pragma once

#include "workstations.hpp"
#include "taskgraph.hpp"

namespace Whisk {

  class Platform
  {
    // Helper class that checks that SimDag environment is created.
    class CheckSD
    {
    public:
      CheckSD();
    };
  public:
    Platform();
  private:
    CheckSD checksd;
    Workstations ws;
    TaskGraph dag;
  };
}
