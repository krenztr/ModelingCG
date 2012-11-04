#ifndef __CAMERA
#define __CAMERA

#define TRACKBALLSIZE (0.8)          // trackball size in percentage of window
#define Z_SENSITIVITY 0.01           // used to scale translations in z
#define XY_SENSITIVITY 0.01 

// find the z coordinate corresponding to the mouse position 
float
project_to_sphere(float r, float x, float y)
{
	float d, t, z;
	
	d = sqrt(x*x + y*y);
	t = r / M_SQRT2;
	if (d < t) {       // Inside sphere
		z = sqrt(r*r - d*d);
	}
	else {             // On hyperbola
		z = t*t / d;
	}
	
	return z;
}

class Camera
{
public:
	Camera()
	{
		reset();
	}

	void reset()
	{
		x = 0;
		y = 0;
		z = 0;
		glPushMatrix();
		glLoadIdentity();
		glGetFloatv(GL_MODELVIEW_MATRIX, (GLfloat *)&translation);
		glGetFloatv(GL_MODELVIEW_MATRIX, (GLfloat *)&rotation);
		glPopMatrix();
	}

	void translate(float dx, float dy, float dz)
	{
		x += dx;
		y += dy;
		z += dz;
		glPushMatrix();
		glLoadIdentity();
		glMultMatrixf(translation);
		glTranslatef(-dx, -dy, -dz);
		glGetFloatv(GL_MODELVIEW_MATRIX, (GLfloat *)&translation);
		glPopMatrix();
	}

	void rotate(float angle, float dx, float dy, float dz)
	{
		glPushMatrix();
		glLoadIdentity();
		glMultMatrixf(rotation);
		glRotatef(-angle, dx, dy, dz);
		glGetFloatv(GL_MODELVIEW_MATRIX, (GLfloat *)&rotation);
		glPopMatrix();
	}
	
	void apply_transformations()
	{
		glLoadIdentity();
		glMultMatrixf(translation);
		glMultMatrixf(rotation);
	}

	float getX()
	{
		return x;
	}

	float getY()
	{
		return y;
	}

	float getZ()
	{
		return z;
	}

	void trackball_translate_z(int x1, int y1, int x2, int y2)
	{
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glLoadIdentity();
		glMultMatrixf(translation);
		glTranslatef(0,0,(y2-y1)*Z_SENSITIVITY);
		glGetFloatv(GL_MODELVIEW_MATRIX, (GLfloat *)&translation);
		glPopMatrix();
  		apply_transformations();
	}

	void
	trackball_translate(int x1, int y1, int x2, int y2)
	{
		glMatrixMode(GL_MODELVIEW);
  		glPushMatrix();
 		glLoadIdentity();
 		glMultMatrixf(translation);
 		glTranslatef((x2-x1)*XY_SENSITIVITY, -(y2-y1)*XY_SENSITIVITY, 0);
 		glGetFloatv(GL_MODELVIEW_MATRIX, (GLfloat *)&translation);
 		glPopMatrix();
  		apply_transformations();
	}

	void trackball_rotate(int x1, int y1, int x2, int y2, float xsize, float ysize){
		float axis[3], p1[3], p2[3];
		float phi, t;
	
		if (x1 == x2 && y1 == y2) { // if there's no movement, no rotation
			axis[0] = 1;
			axis[1] = 0;
			axis[2] = 0;
			phi = 0;
		}
		else {
			// first vector
			p1[0] = (2.0*x1)/(float)xsize - 1.0;
			p1[1] = 1.0 - (2.0*y1)/(float)ysize;
			p1[2] = project_to_sphere((float)TRACKBALLSIZE, p1[0], p1[1]);
			// second vector
			p2[0] = (2.0*x2)/(float)xsize - 1.0;
			p2[1] = 1.0 - (2.0*y2)/(float)ysize;
			p2[2] = project_to_sphere((float)TRACKBALLSIZE, p2[0], p2[1]);
			
			// the axis of rotation is given by the cross product of the first
			// and second vectors
			axis[0] = p1[1]*p2[2] - p1[2]*p2[1];
			axis[1] = p1[2]*p2[0] - p1[0]*p2[2];
			axis[2] = p1[0]*p2[1] - p1[1]*p2[0];
			t = axis[0]*axis[0] + axis[1]*axis[1] + axis[2]*axis[2];
			axis[0] /= t;
			axis[1] /= t;
			axis[2] /= t;
			
			// the amount of rotation is proportional to the magnitude of the
			// difference between the vectors
			t = sqrt((p1[0]-p2[0])*(p1[0]-p2[0]) + (p1[1]-p2[1])*(p1[1]-p2[1]) +
					 (p1[2]-p2[2])*(p1[2]-p2[2]))/(2.0*TRACKBALLSIZE);
			
			if (t > 1.0) {
				t = 1.0;
			}
			if (t < -1.0) {
				t = -1.0;
			}
			phi = 360.0*asin(t)/M_PI;
		}

		glPushMatrix();
		glLoadIdentity();
		glRotatef(phi, axis[0], axis[1], axis[2]);
		glMultMatrixf(rotation);
		glGetFloatv(GL_MODELVIEW_MATRIX, (GLfloat *)&rotation);
		glPopMatrix();
		apply_transformations();
	}

private:
	float x;
	float y;
	float z;
	float translation[16], rotation[16];
};
#endif
