// Tim Finer 
// tfiner@csu.fullerton.edu
// CPSC-566
//


#include "function.h"
#include "hit_record.h"
#include "simplexnoise1234.h"
#include "log_msg.h"
#include "noise_factory.h"

#include "module/module.h"

#include <boost/lexical_cast.hpp>

#include <cassert>
#include <stack>
#include <cmath>
#include <cstdlib>
#include <sstream>


using namespace glow;


namespace {


    const auto H = 0.001;


} // namespace {


Function::Function(int matIdx, const KeyValuesSeq& kvs) : 
    Object(matIdx), evalFunc(noop) {

    evalFunc = CreateFunctions( modules, kvs );
    assert( !modules.empty() );
}



bool Function::HitImpl( HitRecord&, const Ray& ) const {
    return false;
}


Vector3 Function::GetNormal( const Point3& pt ) const {
    auto const & f = evalFunc;

    Vector3 n;
    // Finite Difference (Central) method.
    auto const h2 = H * 2.0f;
    n[0] = ( f(pt[0]+H, pt[1],      pt[2])   - f(pt[0]-H,   pt[1],      pt[2])   )  /  h2;
    n[1] = ( f(pt[0],   pt[1]+H,    pt[2])   - f(pt[0],     pt[1]-H,    pt[2])   )  /  h2; 
    n[2] = ( f(pt[0],   pt[1],      pt[2]+H) - f(pt[0],     pt[1],      pt[2]-H) )  /  h2; 
    return n.normalized();
}


bool Function::InsideImpl( const Point3& pt ) const {
    // TODO:1 make this conditional part of the kv in the ctor.
    return evalFunc(pt[0], pt[1], pt[2]) < pt[2];
    // return evalFunc(pt[0], pt[1], pt[2]) < 0;
}

