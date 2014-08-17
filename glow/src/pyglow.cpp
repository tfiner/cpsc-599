// Tim Finer 
// tfiner@csu.fullerton.edu
// CPSC-599

#include <boost/python/module.hpp>
#include <boost/python/def.hpp>

char const* greet()
{
   return "hello, world";
}

BOOST_PYTHON_MODULE(glow)
{
    using namespace boost::python;
    def("greet", greet);
}

