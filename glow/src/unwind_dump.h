// Tim Finer 
// tfiner@csu.fullerton.edu
// CPSC-599

#pragma once

#include <iosfwd>

namespace glow {
    

    // Unwinds stack and dumps the current exception.
    void UnwindDumpException();
    
    // Unwinds stack and dumps to stream.
    void UnwindDump( std::ostream& os );


} // glow
