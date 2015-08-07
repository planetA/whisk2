#pragma once

#include <map>
#include <vector>

namespace Whisk
{
  // Attributes of a task on a concrete processor type
  class TaskProcessorType
  {
    // I will be surprised to see a task with no cost attribute
    double costs;
    std::map<std::string, double> attributes;
  };

  // Description of a task
  class TaskDesc
  {
    std::vector<TaskProcessorType> types;
  };

  class Metrics
  {
    bool active;
    std::vector<TaskDesc> tasks;
  public:
    Metrics();
    void init();
  };
}