// Tim Finer 
// tfiner@csu.fullerton.edu
// 
// This file contains all of the std::ostream dumpers for the param objects.

#include "memento.h"
#include <iostream>
#include <iomanip>


using namespace std;
using namespace glow;


namespace {


    void DumpGeomObj(std::ostream& os, const ObjectParamsPtr& op) {
        if (auto c = dynamic_cast<const CompositeParams*>(op) ) {
            os << *c;

        } else if (auto s = dynamic_cast<const SphereParams*>(op) ) {
            os << *s;
            
        } else if (auto pp = dynamic_cast<const PlaneParams*>(op) ) {
            os << *pp;

        } else if (auto fp = dynamic_cast<const FunctionParams*>(op) ) {
            os << *fp;

        } else {
            os << op;
        }
    }


} // namespace {}


namespace glow {


    SphereParams::SphereParams(VectorParams center_, float radius_, int matIdx) : 
        ObjectParams("sphere", matIdx), center(center_), radius(radius_) {}


    CompositeParams::CompositeParams(int matIdx, ObjectParamsSeq& objs) : 
        ObjectParams("composite", matIdx), objects(objs) {
        objs.clear();
    }


} // namespace glow


std::ostream& operator<<(std::ostream& os, const VectorParams& vp) {
    os <<   "   x: " << vp.x
       <<   "   y: " << vp.y
       <<   "   z: " << vp.z;
    return os;
}


std::ostream& operator<<(std::ostream& os, const CameraParams& cp) {
    os  <<   "Camera:\n"
        <<   "   type:     " << cp.type <<"\n"
        <<   "   position: " << cp.position << "\n"
        <<   "   dir:      " << cp.direction << "\n"
        <<   "   up:       " << cp.up << "\n"
        <<   "   right:    " << cp.right << "\n"
        <<   "   fov:      " << cp.fov << "\n"
        <<   "   distance: " << cp.distance << "\n";
    return os;
}


std::ostream& operator<<(std::ostream& os, const LightParams& lp) {
    os  <<   "Light:\n"
        <<   "   type:       " << lp.type <<"\n"
        <<   "   position:   " << lp.position << "\n"
        <<   "   dir:        " << lp.direction << "\n"
        <<   "   color:      " << lp.color << "\n"
        <<   "   intensity:  " << lp.intensity << "\n";

    return os;
}


std::ostream& operator<<(std::ostream& os, const ViewPlaneParams& vp) {
    os  <<   "ViewPlane:\n"
        <<   "   width:     " << vp.width << "\n"
        <<   "   height:    " << vp.height << "\n"
        <<   "   pixelSize: " << vp.pixelSize << "\n";
    return os;
}


std::ostream& operator<<(std::ostream& os, const MaterialParams& mp) {
    os  <<   "Material:\n"
        <<   "   type:    " << mp.type << "\n"
        <<   "   diffuse: " << mp.diffuse << "\n";
    return os;
}


std::ostream& operator<<(std::ostream& os, const ObjectParams& op) {
    os  <<   "Object:\n"
        <<   "   type:           " << op.type << "\n"
        <<   "   materialIndex:  " << op.materialIndex << "\n";
    return os;
}


std::ostream& operator<<(std::ostream& os, const SphereParams& sp) {
    os  <<   "Sphere:\n"
        <<   "   type:           " << sp.type << "\n"
        <<   "   materialIndex:  " << sp.materialIndex << "\n"
        <<   "   center:         " << sp.center << "\n"
        <<   "   radius:         " << sp.radius << "\n";
    return os;
}


std::ostream& operator<<(std::ostream& os, const FunctionParams& fp) {
    os  <<   "FunctionParams:\n"
        <<   "   type:           " << fp.type << "\n"
        <<   "   materialIndex:  " << fp.materialIndex << "\n";

    for ( auto const & kvs : fp.keyVals )
        for ( auto const & kv : kvs )
            os << "   " << kv.first << ": " << kv.second << "\n";

    return os;
}


std::ostream& operator<<(std::ostream& os, const PlaneParams& pp) {
    os  <<   "Plane:\n"
        <<   "   type:           " << pp.type << "\n"
        <<   "   materialIndex:  " << pp.materialIndex << "\n"
        <<   "   normal:         " << pp.normal << "\n"
        <<   "   offset:         " << pp.offset << "\n";
    return os;
}


std::ostream& operator<<(std::ostream& os, const CompositeParams& cp) {
    os  <<   "Composite Object:\n"
        <<   "   type:           " << cp.type << "\n"
        <<   "   materialIndex:  " << cp.materialIndex << "\n";

    if ( !cp.objects.empty() )
        os << "   Children:\n";

    for ( auto const & o : cp.objects )
        DumpGeomObj( os, o );

    return os;
}


std::ostream& operator<<(std::ostream& os, const SceneSamplerParams& ssp) {
    os  <<   "Scene Sampler:\n";

    for ( auto const & kv : ssp.keyVals )
        os << "   " << kv.first << ": " << kv.second << "\n";

    return os;
}


std::ostream& operator<<(std::ostream& os, const FogParams& fp) {
    os  <<   "Fog:\n"
        <<   "   density:  " << fp.density << "\n"
        <<   "   color:    " << fp.color << "\n"
        <<   "   maximum:  " << fp.maximum << "\n";
    return os;
}


std::ostream& operator<<(std::ostream& os, const SceneParams& sp) {
    os  <<  "Scene: \n"
        <<  sp.camera << "\n"
        <<  sp.sceneSampler << "\n"
        <<  sp.viewPlane << "\n"
        <<  "Background:\n" << sp.background << "\n"
        <<  "Ambient:\n" << sp.ambient << "\n\n";

    os << "Materials (" << sp.materials.size() << "):\n";
    int i = 0;
    for ( auto const & m : sp.materials ){
        os << i << ": " << m;
        i++;
    }
    os << "\n";

    os << "Objects (" << sp.objects.size() << "):\n";
    for ( auto const & o : sp.objects )
        DumpGeomObj( os, o );

    os << "Lights (" << sp.lights.size() << "):\n";
    for ( auto const & l : sp.lights )
        os << l;

    os << "Fog: " << sp.fogParams;

    return os;
}
