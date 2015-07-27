#pragma once
#include "driver.hpp"

namespace Whisk
{
  class MirorDrv : public Driver
  {
  public:
    MirorDrv(int &argc, char *argv[]);
  };
}
