#include <whisk/driver.hpp>

using namespace Whisk;

Driver::Driver()
{
}

void Driver::init(std::vector<std::string>& unparsed)
{
  if (unparsed.size() > 0)
    throw std::runtime_error("Unrecognized options were passed");

}
