#ifndef __CAMERA
#define __CAMERA

class Camera
{
public:
	Camera()
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
		glMultMatrixf(translation);
		glMultMatrixf(rotation);
	}

private:
	float x;
	float y;
	float z;
	float translation[16], rotation[16];
};
#endif
