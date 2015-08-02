#pragma once

namespace Whisk {
  class Workstation
  {
  public:
    int index;
    SD_task_t task;
    SD_task_t channel;
  };
}
