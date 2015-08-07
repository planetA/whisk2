#pragma once

#include <xbt.h>

namespace Whisk
{
  class TaskGraph
  {
    xbt_dynar_t dynar;
    int count;
    static inline TaskGraph dot_reader();
  public:
    static TaskGraph reader();
  };
}
