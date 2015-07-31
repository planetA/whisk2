#include <simdag/simdag.h>

#include <boost/program_options.hpp>
#include <boost/log/trivial.hpp>

#include <whisk.hpp>
#include <whisk/driver_simdag.hpp>

using namespace Whisk;

SimDagDrv::SimDagDrv()
{
}

void SimDagDrv::append_options(po::options_description &cmdline_options)
{
  po::options_description drv_opt("SimDag driver options");
  drv_opt.add_options()
    ("platform,p", po::value<std::string>(), "Set platform configuration file")
    ;
  cmdline_options.add(drv_opt);
}

void SimDagDrv::init(std::vector<std::string> &unparsed)
{
  // auto unparsed = po::collect_unrecognized(po, po::include_positional);
  int argc = unparsed.size();

  // one extra for the null and one extra because simgrid ignores
  // argument argv[0]
  char * argv[argc + 1];

  argv[0] = 0;
  for (std::size_t i = 0; i != argc; ++i)
  {
    argv[i+1] = &unparsed[i][0];
  }

  argc++;
  argv[argc] = 0;

  SD_init(&argc, argv);

  if (argv[1])
  {
    throw po::unknown_option(argv[1]);
  }
}

SimDagDrv::~SimDagDrv()
{
  SD_exit();
}
