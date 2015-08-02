#pragma once

#include <simdag/simdag.h>

namespace Whisk {
  class Workstations
  {
    friend class Platform;

    const int num_pe;
    const SD_workstation_t *pe_list;
  public:
    Workstations();
  };
}
