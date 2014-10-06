// Tim Finer 
// tfiner@csu.fullerton.edu
// CPSC-599

// An mailing list post led me to include this to get rid of a linker error:
// https://mail.python.org/pipermail/cplusplus-sig/2011-March/015897.html
#include <python.h>

#include "loader.h"
#include "unwind_dump.h"


#include <boost/python/module.hpp>
#include <boost/python/def.hpp>
#include <boost/python/list.hpp>
#include <boost/python/extract.hpp>

#include <iostream>

namespace {


    std::vector<std::string> ListToVector(boost::python::list& bpl) {
        boost::python::ssize_t len = boost::python::len(bpl);

        std::vector<std::string> vt;
        vt.reserve(len);
        for(int i=0; i<len;i++){
            auto t = boost::python::extract<std::string>(bpl[i])();
            vt.push_back(t);
        }

        return vt;
    }


    int WrapRun(boost::python::list args){
        int res = -1000;
        try {
            auto vargs = ListToVector(args);        
            res = glow::Run(vargs);

        } catch(const std::exception& e) {
            std::cerr << "Caught a std::exception: " << e.what() << "\n";
            res = -100;

        } catch(...) {
            std::cerr << "Caught an unspecified c++ exception: \n";
            glow::UnwindDumpException();
        }
        return res;

    }

}

// Things to test.
// 1. Use boost signals to communicate progress, cancellation.
// 2. Write a python script to test.

// Simple test wrapper.
char const* greet()
{
   return "hello, world";
}


BOOST_PYTHON_MODULE(glow)
{
    using namespace boost::python;

    def("greet", greet);
    def("run", WrapRun);
}

