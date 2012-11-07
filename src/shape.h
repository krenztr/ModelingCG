//
//  shape.h
//  ShaderBox
//
//  Created by Eric Henderson on 11/4/12.
//
//

#ifndef shape_h
#define shape_h

#include "transform.h"
#include "ShapeVertices.h"

typedef enum {SHAPE_CUBE, SHAPE_SPHERE, SHAPE_TETRAHEDRON, SHAPE_PLANE, SHAPE_OCTAHEDRON/*, SHAPE_ICOSAHEDRON*/, SHAPE_CYLINDER} shape_type;

class Shape
{
public:
    unsigned int shapeId;
    shape_type type;
    vec3 color;
    int hseg;
    int vseg;
    TransformTranslate trans;
    TransformRotate rot;
    TransformScale scale;
    //TransformShear shear;
    
    Shape(shape_type type, unsigned int shapeId, TransformTranslate trans, TransformRotate rot, TransformScale scale/*, TransformShear shear*/) : type(type), shapeId(shapeId), color(vec3(0.5)), hseg(HORIZ_DETAIL), vseg(VERTI_DETAIL), trans(trans), rot(rot), scale(scale)/*, shear(shear)*/ {}
    
    void drawShape()
    {
        switch(this->type)
        {
            case SHAPE_CUBE:
                drawCube(); break;
            case SHAPE_SPHERE:
                drawSphere(this->hseg, this->vseg); break;
            case SHAPE_TETRAHEDRON:
                drawTetrahedron(); break;
            case SHAPE_PLANE:
                drawPlane(); break;
            case SHAPE_OCTAHEDRON:
                drawOctahedron(); break;
            case SHAPE_CYLINDER:
                drawCylinder(this->hseg); break;
        }
    }
};

#endif
