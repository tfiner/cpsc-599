// Tim Finer 
// tfiner@csu.fullerton.edu
// CPSC-599
//


#include "loader.h"
#include "unwind_dump.h"


int main(int argc, char** argv) {
    int res = 0;

    try {
        res = glow::run(argc, argv);

    } catch(...) {
        glow::UnwindDumpException();
    }

    return res;
}

