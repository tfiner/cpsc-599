// Tim Finer 
// tfiner@csu.fullerton.edu
// CPSC-599
//


#include "function.h"
#include "hit_record.h"
#include "simplexnoise1234.h"
#include "log_msg.h"

#include "module/module.h"

#include <boost/lexical_cast.hpp>

#include <cassert>
#include <stack>
#include <cmath>
#include <cstdlib>
#include <sstream>


using namespace glow;
using namespace noise::module;


MaterialFunction::MaterialFunction(const KeyValuesSeq& kvs) : 
    evalFunc(noop) {

    evalFunc = CreateFunctions( modules, kvs );
    assert( !modules.empty() );
}


Color MaterialFunction::ShadeImpl(const HitRecord& hr, const LightPtr&) const override {
    return Color(0,1,0);
}

