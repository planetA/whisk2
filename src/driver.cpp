#include <whisk/driver.hpp>

using namespace Whisk;

Driver::Driver()
{
}

void Driver::init(std::vector<std::string>& unparsed)
{
  if (unparsed.size() > 0)
    throw po::unknown_option(unparsed[0]);

}
