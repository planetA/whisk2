#include <string>
#include <boost/program_options.hpp>

#include <whisk.hpp>

using namespace std;
namespace po = boost::program_options;

int main(int argc, char *argv[])
{
  Whisk::Context::init(argc, argv);

  return 0;
}
