#ifndef _resources_hpp_INCLUDED
#define _resources_hpp_INCLUDED

#include <cstdint>

#include "cadical_export.hpp"

namespace CaDiCaL {

CADICAL_API double absolute_real_time ();
CADICAL_API double absolute_process_time ();

CADICAL_API uint64_t maximum_resident_set_size ();
CADICAL_API uint64_t current_resident_set_size ();

} // namespace CaDiCaL

#endif // ifndef _resources_hpp_INCLUDED
