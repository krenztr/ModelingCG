#define _USE_MATH_DEFINES

// Shapes: Sphere Cube, Tetrahedron, Icosahedron (not implemented),
//	Octahedron, plane (quad)
// all shapes are normal and centered at the origin

#define VERTI_DETAIL 13
#define HORIZ_DETAIL 16

float rMod(float z);

void drawSphere(int hDetail, int vDetail);

void drawCylinder(int hDetail);

void drawSphere();

void drawCylinder();

void drawCube();

void drawTetrahedron();

void drawOctahedron();

void drawPlane();

void drawSphereAt(float radius, float x, float y, float z);