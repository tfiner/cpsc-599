// Tim Finer 
// tfiner@csu.fullerton.edu
// CPSC-599
//

#pragma once

#include <boost/date_time/posix_time/posix_time.hpp>
#include <sstream>
#include <iostream>
#include <cstring>


extern int logVerbosity;


#define FILE_ONLY__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)

#if defined(USE_TBB)
#   include <tbb/tbb.h>
    extern tbb::mutex coutMutex;
#   define COUT_LOCK tbb::mutex::scoped_lock lock(coutMutex)
#else
#   define COUT_LOCK 
#endif

#define GET_DBG_FUNC_STR__(str)  \
    do { \
        auto ptime = boost::posix_time::microsec_clock::local_time().time_of_day(); \
        str << FILE_ONLY__ << "(" << __LINE__ << "): " \
            << ptime << ": "; \
    } while( false )

#define LOG_MSG__(msg) \
    do { \
      std::stringstream ss; \
      GET_DBG_FUNC_STR__(ss); \
      ss << msg << "\n"; \
      COUT_LOCK; \
      std::cout << ss.str();  \
    } while( false )

#define LOG_MSG(level,msg) \
    if ( logVerbosity >= (level) ){ \
      LOG_MSG__(msg); \
    }