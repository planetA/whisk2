#include <whisk.hpp>

#include <simdag/simdag.h>
#include <xbt.h>

#include "platform.hpp"
#include "workstations.hpp"
#include "taskgraph.hpp"

using namespace Whisk;

Platform::CheckSD::CheckSD()
{
  if (!wh)
    throw std::runtime_error("Whisk context is not initialized.");

  // XXX: do we need such check. If it is not initialized in
  // constructor, it means that the constructor is going to throw an
  // exception
  // if (!wh->driver->sd_init)
  //   throw std::runtime_error("SimDag environment should be initialized.");
}

Platform::Platform()
  : checksd(), ws(), dag(Whisk::TaskGraph::reader())
{

}
