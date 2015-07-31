#include <fstream>

#include <boost/log/trivial.hpp>

#include <whisk.hpp>
#include <whisk/driver_simdag.hpp>
#include <whisk/driver_mirror.hpp>

namespace po = boost::program_options;

using namespace Whisk;

std::unique_ptr<Context> Whisk::wh;

Context::Context(int &argc, char *argv[])
{
}

void Context::createDriver()
{
  if (!vm.count("driver"))
  {
    BOOST_LOG_TRIVIAL(info) << "No driver specified. Using 'miror' driver.";
    driver.reset(new MirrorDrv());
  } else if (vm["driver"].as<std::string>() == "miror")
  {
    driver.reset(new MirrorDrv());
  } else if (vm["driver"].as<std::string>() == "simdag")
  {
    driver.reset(new SimDagDrv());
  } else
  {
    BOOST_LOG_TRIVIAL(fatal) << "Driver " << vm["driver"].as<std::string>()
                             << "is unknown";
    throw std::runtime_error("Unknown driver specified.");
  }

}

void Context::init(int &argc, char *argv[])
{

  // XXX: The main purpose of this function is to parse command line
  // arguments. Important command line arguments are "platform",
  // "sched", and "config". The hard thing about these arguments is
  // that I have parse them "out" first, and only then I will know
  // complete list of available options. So I do all my best to write
  // code as clear as possible, but I'm always happy to get any
  // feedback on how can I improve this part.

  if (wh)
    throw std::runtime_error("Can't initialize context more than once.");

  try
  {
    wh.reset(new Context(argc, argv));

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
      ("time,t", po::value<std::string>(), "Set task timing configuration file")
      ("graph,g", po::value<std::string>(), "Set DAG file")
      ("sched,s", po::value<std::string>(), "Set scheduling algorithm")
      ("ccr", po::value<double>(),
       "Set communication-to-computation ration (CCR)")
      ("cv", po::value<double>(), "Set coefficient of variation (CV)")
      ;

    // Cmd line options visible for everyone, so that everyone can add
    // its own options, which are shown in the help message
    po::options_description cmdline_options;

    // Object which comprises options which are accessible in the
    // configuration file
    po::options_description config_file_options;
    config_file_options.add(simulation);

    // Object which comprises options which are accessible from the
    // command line
    cmdline_options.add(generic).add(simulation);

    // Use basic_command_line_parser to allow unrecognized options
    // Parse command line options
    po::parsed_options parsed = po::command_line_parser(argc, argv).
      options(cmdline_options).
      allow_unregistered().
      run();

    po::store(parsed, wh->vm);
    po::notify(wh->vm);

    // Open configuration file
    std::ifstream ifs(config_file);
    if (ifs)
    {
      // Trigger configuration file parsing
      store(parse_config_file(ifs, config_file_options), wh->vm);
      notify(wh->vm);
    }
    else
    {
      // Failed to open the file
      BOOST_LOG_TRIVIAL(error) << "Could not open config file: "
                               << config_file << std::endl;
      // throw std::runtime_error("Could not open config file.");
    }

    // Create driver and parse driver's options
    wh->createDriver();

    wh->driver->append_options(cmdline_options);

    if (wh->vm.count("help"))
    {
      std::cout << cmdline_options << std::endl;
      return;
    }

    parsed = po::command_line_parser(argc, argv).
      options(cmdline_options).
      allow_unregistered().
      run();

    po::store(parsed, wh->vm);
    po::notify(wh->vm);

    std::vector<std::string> to_pass_further =
      po::collect_unrecognized(parsed.options, po::include_positional);

    wh->driver->init(to_pass_further);
  }
  catch (std::exception &e)
  {
    std::cerr << "Error while processing configuration: " << e.what() << "\n";
    exit(1);
  }
}
