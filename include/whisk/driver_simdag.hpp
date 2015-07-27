#include "driver.hpp"

namespace Whisk
{
  class SimDagDrv : public Driver
  {
  public:
    SimDagDrv();
    SimDagDrv(int &argc, char *argv[]);
  };
}
