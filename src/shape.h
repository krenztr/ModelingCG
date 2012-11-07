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
    TransformTranslate trans;
    TransformRotate rot;
    TransformScale scale;
    //TransformShear shear;
    
protected:
    Shape(shape_type type, unsigned int shapeId, TransformTranslate trans, TransformRotate rot, TransformScale scale/*, TransformShear shear*/) : type(type), shapeId(shapeId), trans(trans), rot(rot), scale(scale)/*, shear(shear)*/ {}
    
public:
    virtual void drawShape() = 0;
};

class ShapeCube : public Shape
{
    public:
    ShapeCube(unsigned int shapeId, TransformTranslate trans, TransformRotate rot, TransformScale scale/*, TransformShear shear*/) : Shape(SHAPE_CUBE, shapeId, trans, rot, scale/*, shear*/) {}
    
    virtual void drawShape()
    {
        drawCube();
    }
};

class ShapeSphere : public Shape
{
public:
    int hseg;
    int vseg;
    
    ShapeSphere(unsigned int shapeId, int hseg, int vseg, TransformTranslate trans, TransformRotate rot, TransformScale scale/*, TransformShear shear*/) : hseg(hseg), vseg(vseg), Shape(SHAPE_CUBE, shapeId, trans, rot, scale/*, shear*/) {}
    
    virtual void drawShape()
    {
        drawSphere(hseg, vseg);
    }
};

class ShapeCylinder : public Shape
{
public:
    int seg;
    
    ShapeCylinder(unsigned int shapeId, int seg, TransformTranslate trans, TransformRotate rot, TransformScale scale/*, TransformShear shear*/) : seg(seg), Shape(SHAPE_CYLINDER, shapeId, trans, rot, scale/*, shear*/) {}
    
    virtual void drawShape()
    {
        drawCylinder(seg);
    }
};

class ShapeTetrahedron : public Shape
{
public:
    ShapeTetrahedron(unsigned int shapeId, TransformTranslate trans, TransformRotate rot, TransformScale scale/*, TransformShear shear*/) : Shape(SHAPE_TETRAHEDRON, shapeId, trans, rot, scale/*, shear*/) {}
    
    virtual void drawShape()
    {
        drawTetrahedron();
    }
};

class ShapePlane : public Shape
{
public:
    ShapePlane(unsigned int shapeId, TransformTranslate trans, TransformRotate rot, TransformScale scale/*, TransformShear shear*/) : Shape(SHAPE_PLANE, shapeId, trans, rot, scale/*, shear*/) {}
    
    virtual void drawShape()
    {
        drawPlane();
    }
};

class ShapeOctahedron : public Shape
{
public:
    ShapeOctahedron(unsigned int shapeId, TransformTranslate trans, TransformRotate rot, TransformScale scale/*, TransformShear shear*/) : Shape(SHAPE_OCTAHEDRON, shapeId, trans, rot, scale/*, shear*/) {}
    
    virtual void drawShape()
    {
        drawOctahedron();
    }
};

#endif
