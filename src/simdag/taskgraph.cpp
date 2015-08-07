#include <whisk.hpp>

#include <simdag/simdag.h>
#include "taskgraph.hpp"

#include "xbt.h"

using namespace Whisk;

TaskGraph TaskGraph::dot_reader()
{
  TaskGraph dag;
  SD_task_t task;
  SD_task_t root;
  SD_task_t end;

  dag.dynar = NULL;
  int count = 0;
  std::vector<std::string> graph_files;
  graph_files = wh->vm["graph"].as<std::vector <std::string> >();
  for (auto graph_file : graph_files)
  {
    BOOST_LOG_TRIVIAL(trace) << "Add another graph file: "
                             << graph_file << std::endl;

    // load task graph
    xbt_dynar_t new_dag = SD_dotload(graph_file.c_str());
    // force to stop simulation as soon as any task starts or finishes
    unsigned int cpt;
    xbt_dynar_foreach(new_dag, cpt, task)
    {
      SD_task_watch(task, SD_RUNNING);
      SD_task_watch(task, SD_DONE);
      if (SD_task_get_kind(task) == SD_TASK_COMP_SEQ)
      {
        // Calculate number of computational tasks
        ++dag.count;
      }
    }

    // If we already read files, merge them with a new one
    if (dag.dynar)
    {
      SD_task_t new_root;
      SD_task_t new_end;

      xbt_dynar_shift(new_dag, &new_root);
      xbt_dynar_pop(new_dag, &new_end);

      {
        unsigned int cur;
        SD_task_t child;
        xbt_dynar_t children = SD_task_get_children(new_root);
        xbt_dynar_foreach(children, cur, child)
        {
          SD_task_dependency_add(NULL, NULL, root, child);
          SD_task_dependency_remove(new_root, child);
        }
        xbt_dynar_free(&children);
      }

      {
        unsigned int cur;
        SD_task_t parent;
        xbt_dynar_t parents = SD_task_get_parents(new_end);
        xbt_dynar_foreach(parents, cur, parent)
        {
          SD_task_dependency_add(NULL, NULL, parent, end);
          SD_task_dependency_remove(parent, new_end);
        }
        xbt_dynar_free(&parents);
      }
      xbt_dynar_merge(&dag.dynar, &new_dag);
    }
    else
    {
      // remove END, so that we can add new tasks dynamically
      xbt_dynar_shift(new_dag, &root);
      xbt_dynar_pop(new_dag, &end);

      dag.dynar = new_dag;
    }
  }

  xbt_dynar_unshift(dag.dynar, &root);
  xbt_dynar_push(dag.dynar, &end);

#if 0
  // XXX: This chunk of code used to be in old whisk
  unsigned int index;
  unsigned task_index = 0;
  xbt_dynar_foreach(*dag, index, task)
  {
    struct wh_task_data *data = calloc(1, sizeof(*data));
    if(SD_task_get_kind(task) == SD_TASK_COMP_SEQ)
      data->index = task_index++;
    data->ws = -1;
    data->visit = 0;
    SD_task_set_data(task, data);
  }
  // XXX: This chunk of code used to be in old whisk
  xbt_dynar_get_cpy(wh->dag, xbt_dynar_length(wh->dag) - 1, &wh->end);
#endif

  return dag;
}

TaskGraph TaskGraph::reader()
{
  if (wh->vm["graph-type"].as<std::string>() == "dot")
  {
    return dot_reader();
  } else
  {
    BOOST_LOG_TRIVIAL(fatal) << "Graph file of type "
                             << wh->vm["graph-type"].as<std::string>()
                             << "is unknown";
    throw po::invalid_option_value("Unknown graph-type specified.");
  }

}
