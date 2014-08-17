// Tim Finer 
// tfiner@csu.fullerton.edu
// CPSC-599
//

#pragma once

#include "geometry.h"

namespace glow {
    

    struct HitRecord;
    struct Ray;


    // The Object class that all renderable objects are derived from.
    // This class uses the NVI (non-virtual interface) idiom aka Template Pattern.
    class Object {
    public:
        Object(int matIdx) : materialIndex(matIdx) {}        
        virtual ~Object() {}

        // Does the ray intersect the object?
        bool Hit( HitRecord& hr, const Ray& r ) const;
        
        // Is the point inside of the object?
        bool Inside( HitRecord& hr, const Point3& pt ) const;
        bool Inside( const Point3& pt ) const;

        int GetMaterialIndex() const { return materialIndex; }

    private:
        int materialIndex;
        
        virtual bool HitImpl( HitRecord& hr, const Ray& r ) const = 0;
        virtual bool InsideImpl( const Point3& pt ) const = 0;

        virtual Vector3 GetNormal(const Point3& pt) const = 0;

    };


} // namespace glow