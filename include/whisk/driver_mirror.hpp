#pragma once
#include "driver.hpp"

namespace Whisk
{
  class MirrorDrv : public Driver
  {
  public:
    void init(std::vector<std::string> &unparsed);
    MirrorDrv();
    ~MirrorDrv();
  };
}
