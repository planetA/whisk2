#include <whisk.hpp>

#include "workstations.hpp"

using namespace Whisk;

Workstations::Workstations()
  : num_pe(SD_workstation_get_number()),
    pe_list(SD_workstation_get_list())
{
#if 0
  // XXX: This chunk of code used to be in old whisk
  // set all PEs to sequential access mode
  // for (int i = 0; i < num_pe; i++)
  // {
    // set access mode
    // SD_workstation_set_access_mode(pe_list[i], SD_WORKSTATION_SHARED_ACCESS);

    // struct wh_workstation_data *data = calloc(1, sizeof(*data));
    // Workstation *ws = new Workstation;
    // ws->index = i;
    // SD_workstation_set_data(pe_list[i], ws);
    /* SD_workstation_dump(wh->pe_list[i]); */
    /* printf( "PE%02d: %s\n", i,  SD_workstation_get_name( wh->pe_list[i] ) ); */
  // }
#endif
}
