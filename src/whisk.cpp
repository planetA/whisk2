#include <fstream>

#include <boost/log/trivial.hpp>

#include <whisk.hpp>

using namespace Whisk;

std::unique_ptr<Context> Whisk::wh;

Context::Context(int &argc, char *argv[])
{
}

// helpers for init function
static inline po::options_description create_options_generic();
static inline po::options_description create_options_simulation();
static inline void parse_config_file(const po::options_description &);

static inline po::parsed_options
parse_command_line(int argc, char *argv[], const po::options_description &);

po::options_description create_options_generic()
{
  // Declare generic options
  po::options_description generic("Generic options");
  generic.add_options()
    ("help", "Show this help message")
    ("config,c", po::value<std::string>(), "Name of a file of a configuration.")
    ;
  return generic;
}

po::options_description create_options_simulation()
{
  // Declare options which configure general simulation parameters
  po::options_description simulation("Simulator options");
  simulation.add_options()
    ("driver,d", po::value<std::string>(), "Set simulation driver")
    ("algorithm,a", po::value<std::string>(), "Set simulation driver")
    ("time,t", po::value<std::string>(), "Set task timing configuration file")
    ("graph,g",
     po::value< std::vector<std::string> >()->composing()->required(),
     "Set DAG file")
    ("sched,s", po::value<std::string>(), "Set scheduling algorithm")
    ("ccr", po::value<double>(),
     "Set communication-to-computation ration (CCR)")
    ("cv", po::value<double>(), "Set coefficient of variation (CV)")
    ;
  return simulation;
}

void parse_config_file(const po::options_description & cfgfile_options)
{
  // Fail if user specified config file, but it is not available. Do
  // not fail if the config file is not specified and the default
  // config file is also unavailable.
  bool fail;
  std::string config_file;
  if (wh->vm.count("config"))
  {
    config_file = wh->vm["config"].as<std::string>();
    fail = true;
  }
  else
  {
    // Default location of the config file
    config_file = ".whisk";
    fail = false;
  }

  // Open configuration file
  std::ifstream ifs(config_file);
  if (ifs)
  {
    // Trigger configuration file parsing
    po::store(po::parse_config_file(ifs, cfgfile_options), wh->vm);
    po::notify(wh->vm);
  }
  else if (fail)
  {
    // Failed to open the file
    BOOST_LOG_TRIVIAL(error) << "Could not open config file: "
                             << config_file << std::endl;
    throw std::runtime_error("Could not open config file.");
  }
  else
  {
    // Default location is not available. Just make a warning
    BOOST_LOG_TRIVIAL(warning) << "Could not open config file: "
                               << config_file << std::endl;
  }

}

po::parsed_options
parse_command_line(int argc, char *argv[],
                   const po::options_description &cmdline_options)
{
  // Use basic_command_line_parser to allow unrecognized options
  // Parse command line options
  po::parsed_options parsed = po::command_line_parser(argc, argv).
    options(cmdline_options).
    allow_unregistered().
    run();

  po::store(parsed, wh->vm);
  po::notify(wh->vm);
  return parsed;
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

    po::options_description generic = create_options_generic();
    po::options_description simulation = create_options_simulation();

    // Cmd line options visible for everyone, so that everyone can add
    // its own options, which are shown in the help message
    po::options_description cmdline_options;

    // Object which comprises options which are accessible from the
    // command line
    cmdline_options.add(generic).add(simulation);

    // Object which comprises options which are accessible in the
    // configuration file
    po::options_description config_file_options;
    config_file_options.add(simulation);

    po::parsed_options parsed = parse_command_line(argc, argv, cmdline_options);
    parse_config_file(config_file_options);

    // Create driver and parse driver's options
    wh->driver.reset(Driver::create());

    wh->driver->append_options(cmdline_options);

    if (wh->vm.count("help"))
    {
      std::cout << cmdline_options << std::endl;
      return;
    }

    // Parse driver specific options
    std::vector<std::string> to_pass_further =
      po::collect_unrecognized(parsed.options, po::include_positional);

    parsed = po::command_line_parser(to_pass_further).
      options(cmdline_options).
      allow_unregistered().
      run();

    po::store(parsed, wh->vm);
    po::notify(wh->vm);

    // Pass further what is left
    to_pass_further =
      po::collect_unrecognized(parsed.options, po::include_positional);

    wh->driver->init(to_pass_further);
  }
  catch (std::exception &e)
  {
    std::cerr << "Error while processing configuration: " << e.what() << "\n";
    exit(1);
  }
}
