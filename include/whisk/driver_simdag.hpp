#include "driver.hpp"

namespace Whisk
{
  class Platform;

  class SimDagDrv : public Driver
  {
    friend Platform;

  protected:
    void append_options(po::options_description &);
    void init(std::vector<std::string>&);
  public:
    void init();
    SimDagDrv();
    ~SimDagDrv();
  private:
    std::unique_ptr<Platform> platform;
    bool sd_init;
  };
}
