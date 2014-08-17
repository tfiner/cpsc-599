// Tim Finer 
// tfiner@csu.fullerton.edu
// CPSC-566

#include "unwind_dump.h"
#include "log_msg.h"

#define UNW_LOCAL_ONLY
#include <libunwind.h>

#include <tbb/tbb.h>

#include <iostream>
#include <sstream>

#include <cxxabi.h>


namespace glow {


    void UnwindDump() {
        LOG_MSG(1, "Unwinding dump.");

        unw_cursor_t cursor; 
        unw_context_t uc;
        unw_word_t offset;

        unw_getcontext(&uc);
        unw_init_local(&cursor, &uc);

        char buf[128] = {};
        char* unmangled = static_cast<char*>(malloc(80));
        while (unw_step(&cursor) > 0) {
            memset( buf, 0, sizeof(buf) );
            unw_get_proc_name(&cursor, buf, sizeof(buf), &offset);
            int status = 0;

            char* realname = abi::__cxa_demangle(buf, unmangled, 0, &status);
            if ( status == 0 && realname != nullptr ){
                LOG_MSG(1, realname);
            } else { 
                LOG_MSG(1, buf);
            }
        }
        delete unmangled;
    }


    void UnwindDumpException() {
        LOG_MSG(0, "Unwinding exception.");

        try {
            UnwindDump();
            throw;
        } catch (tbb::captured_exception& e) {
            LOG_MSG(0, "Caught tbb::captured_exception " << e.what() << "\n");
        } catch ( std::exception& e ) {
            LOG_MSG(0, "Caught std::exception: " << e.what() << "\n");
        } catch (...) {
            LOG_MSG(0, "Caught unknown exception, continuing...\n");
        } 
    }


} // glow
