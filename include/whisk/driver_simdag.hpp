#include "driver.hpp"

namespace Whisk
{
  class SimDagDrv : public Driver
  {
  protected:
    void append_options(po::options_description &);
    void init(std::vector<std::string>&);
  public:
    void init();
    SimDagDrv();
    ~SimDagDrv();
  };
}
