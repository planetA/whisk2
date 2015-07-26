#include <whisk.hpp>

#include <boost/program_options.hpp>

int main(int argc, char *argv[])
{
  Whisk::Driver *driver = Whisk::Wh::createDriver(argc, argv);

  return 0;
}
