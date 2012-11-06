//
//  transform.h
//  ShaderBox
//
//  Created by Eric Henderson on 11/4/12.
//
//

#ifndef transform_h
#define transform_h

#include "vector.h"

typedef enum {TRANSFORM_TRANSLATE, TRANSFORM_ROTATE, /*TRANSFORM_SHEAR, */TRANSFORM_SCALE} transform_type;

struct Transform
{
    transform_type type;
    vec3 trans;
    
    Transform(transform_type type, vec3 trans) : type(type), trans(trans) {}
};

struct TransformTranslate : Transform
{
    TransformTranslate(vec3 trans) : Transform(TRANSFORM_TRANSLATE, trans) {}
};

struct TransformRotate : Transform
{
    TransformRotate(vec3 trans) : Transform(TRANSFORM_ROTATE, trans) {}
};

/*struct TransformShear : Transform
{
    TransformShear(vec3 trans) : Transform(TRANSFORM_SHEAR, trans) {}
};*/

struct TransformScale : Transform
{
    TransformScale(vec3 trans) : Transform(TRANSFORM_SCALE, trans) {}
};

#endif
