#include <fstream>

#include <boost/program_options.hpp>
#include <boost/log/trivial.hpp>

#include <whisk.hpp>
#include <whisk/driver_simdag.hpp>
#include <whisk/driver_miror.hpp>

namespace po = boost::program_options;

using namespace Whisk;

std::unique_ptr<Context> wh;

Context::Context(int &argc, char *argv[])
{
  // Path to config file
  std::string config_file;

  // Declare generic options
  po::options_description generic("Generic options");
  generic.add_options()
    ("help", "Show this help message")
    ("config,c", po::value<std::string>(&config_file)->default_value(".whisk"),
      "Name of a file of a configuration.")
    ;

  // Declare options which configure general simulation parameters
  po::options_description simulation("Simulator options");
  simulation.add_options()
    ("driver,d", po::value<std::string>(), "Set simulation driver")
    ("algorithm,a", po::value<std::string>(), "Set simulation driver")
    ;

  // Object which comprises options which are accessible from the
  // command line
  po::options_description cmdline_options;
  cmdline_options.add(generic).add(simulation);

  // Object which comprises options which are accessible in the
  // configuration file
  po::options_description config_file_options;
  config_file_options.add(simulation);

  // Parse command line options
  po::store(po::parse_command_line(argc, argv, cmdline_options), vm);
  po::notify(vm);

  // Open configuration file
  std::ifstream ifs(config_file);
  if (ifs)
  {
    // Trigger configuration file parsing
    store(parse_config_file(ifs, config_file_options), vm);
    notify(vm);
  }
  else
  {
    // Failed to open the file
    BOOST_LOG_TRIVIAL(error) << "Could not open config file: "
                             << config_file << std::endl;
    // throw std::runtime_error("Could not open config file.");
  }

  if (vm.count("help"))
  {
    std::cout << cmdline_options << std::endl;
    return;
  }

  if (vm.count("driver"))
  {
    std::cout << "Driver was chosen to be "
              << vm["driver"].as<std::string>()
              << ".\n";
  } else
  {
    std::cout << "Driver has not been chosen.\n";
  }

  createDriver(argc, argv);
}

void Context::createDriver(int &argc, char *argv[])
{
  if (!vm.count("driver"))
  {
    BOOST_LOG_TRIVIAL(info) << "No driver specified. Using 'miror' driver.";
    driver.reset(new MirorDrv(argc, argv));
  } else if (vm["driver"].as<std::string>() == "miror")
  {
  } else if (vm["driver"].as<std::string>() == "simdag")
  {
  } else
  {
    BOOST_LOG_TRIVIAL(fatal) << "Driver " << vm["driver"].as<std::string>()
                             << "is unknown";
    throw std::runtime_error("Unknown driver specified.");
  }

}

void Context::init(int &argc, char *argv[])
{
  if (wh)
    throw std::runtime_error("Can't initialize context more than once.");

  wh.reset(new Context(argc, argv));
}
