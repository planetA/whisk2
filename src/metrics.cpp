#include <fstream>

#include <whisk.hpp>
#include <whisk/metrics.hpp>

using namespace Whisk;

Metrics::Metrics()
  : active(false)
{}

void Metrics::init()
{
  if (!wh->vm.count("metrics"))
  {
    // Failed to open the file
    BOOST_LOG_TRIVIAL(info) << "Metrics file has not been provided:"
                            << std::endl;
    // Metrics table is inactive. Just return
    return;
  }

  std::string config_file(wh->vm["metrics"].as<std::string>());
  std::ifstream ifs(config_file);
  if (ifs)
  {
  }
  else
  {
    // Failed to open the file
    BOOST_LOG_TRIVIAL(error) << "Could not open metrics file:"
                             << config_file << std::endl;
    throw std::runtime_error("Could not open config file.");
  }
}
