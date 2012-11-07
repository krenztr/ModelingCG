#include "ShapeVertices.h"
#include <SFML/Window.hpp>
#include <math.h>
#include <iostream>


float rMod(float z)
{
	if( z>=1.0 || z<=-1.0 )
		return 0.0;
	// z^2+result^2=radius^2
	return sqrt(1.0 - pow(z,2.0));
}

//TODO: Make sure all shapes face outward.
void drawSphere(int hDetail, int vDetail)
{
	float z, thickness, inc;
	thickness = 2.0/vDetail;
	inc = 2*M_PI/hDetail;

	// Bottom
	glBegin(GL_TRIANGLE_FAN);
	glNormal3f(0.0, 0.0, -1.0);
	glVertex3f(0.0, 0.0, -1.0);
	z = -1 + (1.0/vDetail);
	for(int i = 0; i < hDetail+1; i++)
	{
		glNormal3f(rMod(z)*sin(i*inc),
			rMod(z)*cos(i*inc), z);
		glVertex3f(rMod(z)*sin(i*inc),
			rMod(z)*cos(i*inc), z);
	}
	glEnd();

	// Middle
	float j, k, offset;
	for(float i = 0.5; i < vDetail; i++)
	{
		z = -1 + i*(2.0/vDetail);
		if((int) (i+0.500001)%2 == 0)
			offset = 0.0;
		else offset = -0.5;

		glBegin(GL_TRIANGLE_STRIP);
		for(j = offset; j <= hDetail; j++)	
		{
			glNormal3f(rMod(z+thickness)*cos(j*inc),
				rMod(z+thickness)*sin(j*inc),
				z+thickness);
			glVertex3f(rMod(z+thickness)*cos(j*inc),
				rMod(z+thickness)*sin(j*inc),
				z+thickness);
			k = j + 0.5;
			glNormal3f(rMod(z)*cos(k*inc),
				rMod(z)*sin(k*inc),z);
			glVertex3f(rMod(z)*cos(k*inc),
				rMod(z)*sin(k*inc),z);
		}
		glEnd();
	}

	// Top
	glBegin(GL_TRIANGLE_FAN);
	glNormal3f(0.0, 0.0, 1.0);
	glVertex3f(0.0, 0.0, 1.0);
	
	if(vDetail % 2 == 1)
		offset = 0.5;
	else offset = 0.0;
	z = 1 - (1.0/vDetail);

	for(int i = 0; i < hDetail+1; i++)
	{
		glNormal3f(rMod(z)*cos(i*inc+offset),
			rMod(z)*sin(i*inc+offset), z);
		glVertex3f(rMod(z)*cos(i*inc+offset),
			rMod(z)*sin(i*inc+offset), z);
	}
	glEnd();
}
/*{
	float z, thickness, inc;
	thickness = 1.0/vDetail;
	inc = 2*M_PI/hDetail;

	glBegin(GL_TRIANGLE_FAN);
	glNormal3f(0.0, 0.0, -1.0);
	glVertex3f(0.0, 0.0, -1.0);
	z = -1 + (2.0/vDetail);
	for(int i = 0; i < hDetail+1; i++)
	{
		glNormal3f(rMod(z)*cos(i*inc),
			rMod(z)*sin(i*inc), z);
		glVertex3f(rMod(z)*cos(i*inc),
			rMod(z)*sin(i*inc), z);
	}
	glEnd();

	float j, k, offset;
	for(int i = 1; i < vDetail; i++)
	{
		z = -1 + i*(2.0/vDetail);
		if(i%2 == 0)
			offset = 0.0;
		else offset = -0.5;

		glBegin(GL_TRIANGLE_STRIP);
		for(j = offset; j <= hDetail; j++)	
		{
			glNormal3f(rMod(z+thickness)*cos(j*inc),
				rMod(z+thickness)*sin(j*inc),
				z+thickness);
			glVertex3f(rMod(z+thickness)*cos(j*inc),
				rMod(z+thickness)*sin(j*inc),
				z+thickness);
			k = j + 0.5;
			glNormal3f(rMod(z)*cos(k*inc),
				rMod(z)*sin(k*inc),z);
			glVertex3f(rMod(z)*cos(k*inc),
				rMod(z)*sin(k*inc),z);
		}
		glEnd();
	}

	glBegin(GL_TRIANGLE_FAN);
	glNormal3f(0.0, 0.0, 1.0);
	glVertex3f(0.0, 0.0, 1.0);
	
	if(vDetail % 2 == 1)
		offset = 0.5;
	else offset = 0.0;
	z = 1 - (2.0/vDetail);

	for(int i = 0; i < hDetail+1; i++)
	{
		glNormal3f(rMod(z)*cos(i*inc+offset),
			rMod(z)*sin(i*inc+offset), z);
		glVertex3f(rMod(z)*cos(i*inc+offset),
			rMod(z)*sin(i*inc+offset), z);
	}
	glEnd();
}*/

void drawCylinder(int hDetail)
{
	float inc = 2*M_PI/hDetail;
	
	glBegin(GL_TRIANGLE_FAN);
	glNormal3f(0.0, 0.0, 1.0);
	glVertex3f(0.0, 0.0, 1.0);
	for(int i = 0; i < hDetail+1; i++)
	{
		glVertex3f(cos(i*inc), sin(i*inc), 1.0);
	}
	glEnd();
	
	glBegin(GL_TRIANGLE_FAN);
	glNormal3f(0.0, 0.0, -1.0);
	glVertex3f(0.0, 0.0, -1.0);
	for(int i = 0; i < hDetail+1; i++)
	{
		glVertex3f(cos(i*inc), sin(i*inc), -1.0);
	}
	glEnd();
	
	glBegin(GL_TRIANGLE_STRIP);
	for(int j = 0; j <= hDetail; j++)	
	{
		glNormal3f(cos(j*inc), sin(j*inc), 0.0);
		glVertex3f(cos(j*inc), sin(j*inc), 1.0);
		glVertex3f(cos(j*inc), sin(j*inc), -1.0);
	}
	glEnd();
}

void drawSphere()
{
	drawSphere(HORIZ_DETAIL, VERTI_DETAIL);
}

void drawCylinder()
{
	drawCylinder(HORIZ_DETAIL);
}

void drawCube()
{
	glBegin(GL_QUADS);
	glNormal3f(0.0,0.0,1.0);
	glVertex3f(1.0,1.0,1.0);
	glVertex3f(-1.0,1.0,1.0);
	glVertex3f(-1.0,-1.0,1.0);
	glVertex3f(1.0,-1.0,1.0);
	
	glNormal3f(0.0,0.0,-1.0);
	glVertex3f(1.0,1.0,-1.0);
	glVertex3f(1.0,-1.0,-1.0);
	glVertex3f(-1.0,-1.0,-1.0);
	glVertex3f(-1.0,1.0,-1.0);
	
	glNormal3f(0.0,1.0,0.0);
	glVertex3f(1.0,1.0,1.0);
	glVertex3f(-1.0,1.0,1.0);
	glVertex3f(-1.0,1.0,-1.0);
	glVertex3f(1.0,1.0,-1.0);
	
	glNormal3f(0.0,-1.0,0.0);
	glVertex3f(1.0,-1.0,1.0);
	glVertex3f(1.0,-1.0,-1.0);
	glVertex3f(-1.0,-1.0,-1.0);
	glVertex3f(-1.0,-1.0,1.0);
	
	glNormal3f(1.0,0.0,0.0);
	glVertex3f(1.0,1.0,1.0);
	glVertex3f(1.0,-1.0,1.0);
	glVertex3f(1.0,-1.0,-1.0);
	glVertex3f(1.0,1.0,-1.0);
	
	glNormal3f(-1.0,0.0,0.0);
	glVertex3f(-1.0,1.0,1.0);
	glVertex3f(-1.0,1.0,-1.0);
	glVertex3f(-1.0,-1.0,-1.0);
	glVertex3f(-1.0,-1.0,1.0);
	glEnd();
}

void drawTetrahedron()
{
	float top = 1.0/sqrt(2.0);
	float nOne = sqrt(2.0/3.0);
	float nTop = 1/sqrt(3.0);

	glBegin(GL_TRIANGLES);
	glNormal3f(nOne, 0.0, nTop);
	glVertex3f(0,1,top);
	glVertex3f(0,-1,top);
	glVertex3f(1,0,-top);

	glNormal3f(-nOne, 0.0, nTop);
	glVertex3f(0,-1,top);
	glVertex3f(0,1,top);
	glVertex3f(-1,0,-top);

	glNormal3f(0.0, nOne, -nTop);
	glVertex3f(1,0,-top);
	glVertex3f(-1,0,-top);
	glVertex3f(0,1,top);

	glNormal3f(0.0, -nOne, -nTop);
	glVertex3f(-1,0,-top);
	glVertex3f(1,0,-top);
	glVertex3f(0,-1,top);
	glEnd();
}

/*TODO: Finish this.
void drawIcosahedron()
{
	float rphi = 2.0/(1+sqrt(5));
	glBegin(GL_TRIANGLES);
	//0,rphi,1,0,rphi,1,...
	glVertex3f(0, rphi, 1);
	glVertex3f(0, -rphi, 1);
	glVertex3f(1, 0, rphi);
	
	glVertex3f(0, -rphi, 1);
	glVertex3f(0, rphi, 1);
	glVertex3f(1, 0, -rphi);
	
	glEnd();
}*/

void drawOctahedron()
{
	float n = 1.0/sqrt(3.0);

	glBegin(GL_TRIANGLES);
	glNormal3f(n,n,n);
	glVertex3f(0.0,0.0,1.0);
	glVertex3f(1.0,0.0,0.0);
	glVertex3f(0.0,1.0,0.0);
	
	glNormal3f(-n,n,n);
	glVertex3f(0.0,0.0,1.0);
	glVertex3f(0.0,1.0,0.0);
	glVertex3f(-1.0,0.0,0.0);
	
	glNormal3f(-n,-n,n);
	glVertex3f(0.0,0.0,1.0);
	glVertex3f(-1.0,0.0,0.0);
	glVertex3f(0.0,-1.0,0.0);
	
	glNormal3f(n,-n,n);
	glVertex3f(0.0,0.0,1.0);
	glVertex3f(0.0,-1.0,0.0);
	glVertex3f(1.0,0.0,0.0);
	
	glNormal3f(n,-n,-n);
	glVertex3f(0.0,0.0,-1.0);
	glVertex3f(1.0,0.0,0.0);
	glVertex3f(0.0,-1.0,0.0);
	
	glNormal3f(-n,-n,-n);
	glVertex3f(0.0,0.0,-1.0);
	glVertex3f(0.0,-1.0,0.0);
	glVertex3f(-1.0,0.0,0.0);
	
	glNormal3f(-n,n,-n);
	glVertex3f(0.0,0.0,-1.0);
	glVertex3f(-1.0,0.0,0.0);
	glVertex3f(0.0,1.0,0.0);
	
	glNormal3f(n,n,-n);
	glVertex3f(0.0,0.0,-1.0);
	glVertex3f(0.0,1.0,0.0);
	glVertex3f(1.0,0.0,0.0);
	glEnd();
}

void drawPlane()
{
	glBegin(GL_QUADS);
	glNormal3f(0.0, 0.0, 1.0);
	glVertex2f(1.0, -1.0);
	glVertex2f(1.0, 1.0);
	glVertex2f(-1.0, 1.0);
	glVertex2f(-1.0, -1.0);
	glEnd();
}

//	For testing the sphere.
void drawSphereAt(float radius, float x, float y, float z)
{
	glScalef(radius, radius, radius);
	glTranslatef(x, y, z);
	drawSphere();
}