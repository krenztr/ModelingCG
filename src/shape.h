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

struct Shape
{
    unsigned int shapeId;
    shape_type type;
    TransformTranslate trans;
    TransformRotate rot;
    TransformScale scale;
    //TransformShear shear;
    
    Shape(shape_type type, unsigned int shapeId, TransformTranslate trans, TransformRotate rot, TransformScale scale/*, TransformShear shear*/) : type(type), shapeId(shapeId), trans(trans), rot(rot), scale(scale)/*, shear(shear)*/ {}
    
    void drawShape(){}
};

struct ShapeCube : Shape
{
    ShapeCube(unsigned int shapeId, TransformTranslate trans, TransformRotate rot, TransformScale scale/*, TransformShear shear*/) : Shape(SHAPE_CUBE, shapeId, trans, rot, scale/*, shear*/) {}
    
    void drawShape()
    {
        drawCube();
    }
};

struct ShapeSphere : Shape
{
    int hseg;
    int vseg;
    
    ShapeSphere(unsigned int shapeId, int hseg, int vseg, TransformTranslate trans, TransformRotate rot, TransformScale scale/*, TransformShear shear*/) : hseg(hseg), vseg(vseg), Shape(SHAPE_CUBE, shapeId, trans, rot, scale/*, shear*/) {}
    
    void drawShape()
    {
        drawSphere(hseg, vseg);
    }
};

struct ShapeCylinder : Shape
{
    int seg;
    
    ShapeCylinder(unsigned int shapeId, int seg, TransformTranslate trans, TransformRotate rot, TransformScale scale/*, TransformShear shear*/) : seg(seg), Shape(SHAPE_CYLINDER, shapeId, trans, rot, scale/*, shear*/) {}
    
    void drawShape()
    {
        drawCylinder(seg);
    }
};

struct ShapeTetrahedron : Shape
{
    ShapeTetrahedron(unsigned int shapeId, TransformTranslate trans, TransformRotate rot, TransformScale scale/*, TransformShear shear*/) : Shape(SHAPE_TETRAHEDRON, shapeId, trans, rot, scale/*, shear*/) {}
    
    void drawShape()
    {
        drawTetrahedron();
    }
};

struct ShapePlane : Shape
{
    ShapePlane(unsigned int shapeId, TransformTranslate trans, TransformRotate rot, TransformScale scale/*, TransformShear shear*/) : Shape(SHAPE_PLANE, shapeId, trans, rot, scale/*, shear*/) {}
    
    void drawShape()
    {
        drawPlane();
    }
};

struct ShapeOctahedron : Shape
{
    ShapeOctahedron(unsigned int shapeId, TransformTranslate trans, TransformRotate rot, TransformScale scale/*, TransformShear shear*/) : Shape(SHAPE_OCTAHEDRON, shapeId, trans, rot, scale/*, shear*/) {}
    
    void drawShape()
    {
        drawOctahedron();
    }
};

#endif
