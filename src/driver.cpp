#include <whisk.hpp>
#include <whisk/driver.hpp>

#include <whisk/driver_simdag.hpp>
#include <whisk/driver_mirror.hpp>

using namespace Whisk;

Driver::Driver()
{
}

void Driver::init(std::vector<std::string>& unparsed)
{
  if (unparsed.size() > 0)
    throw po::unknown_option(unparsed[0]);

}

Driver *Driver::create()
{
  if (!wh->vm.count("driver"))
  {
    BOOST_LOG_TRIVIAL(info) << "No driver specified. Using 'miror' driver.";
    return new MirrorDrv();
  } else if (wh->vm["driver"].as<std::string>() == "miror")
  {
    return new MirrorDrv();
  } else if (wh->vm["driver"].as<std::string>() == "simdag")
  {
    return new SimDagDrv();
  } else
  {
    BOOST_LOG_TRIVIAL(fatal) << "Driver " << wh->vm["driver"].as<std::string>()
                             << "is unknown";
    throw po::invalid_option_value("Unknown driver specified.");
  }

}
