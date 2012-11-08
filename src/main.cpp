#include "glew/glew.h"
#include <SFML/Window.hpp>
#include <math.h>
#include <stdio.h>

#include "Config.h"
#include "ShaderManager.h"
#include "TextureBufferObject.h"
#include "ShowTexture.h"
#include "Camera.h"
#include "shape.h"
#include <list>
#include <map>

#ifdef __APPLE__
#include <mach-o/dyld.h>
#include <unistd.h>
#include <string.h>
#endif
#define RESOLUTION_X 800
#define RESOLUTION_Y 800
#define SELECT_BOX_SIZE 1

typedef enum {X_AXIS, Y_AXIS, Z_AXIS} axis_mode;
typedef enum {TRANS_TRANSLATION, TRANS_ROTATION, TRANS_SCALE} trans_mode;

GLuint textureTarget;
GLuint shaderProg;
GLuint noLightProg;
sf::Window *App;
sf::Clock Clock;
ShowTexture texRender;
int currentRes[2];
float lightPos[3];
float cameraPos[3];
Camera camera;
int lastPos[2] = {0,0};
int buttonDown[3] = {0,0};
std::list<Shape> listOfShapes;
unsigned int idCounter;
unsigned int selected;
axis_mode axisM;
trans_mode transM;

FILE * logFile;
bool GL20Support;

void renderScene();
void handleEvents();
void init();
void setupTargetTexture();
void setShaderVariables(GLuint shaderProg);
void __glewInit();
void update_perspective();
void draw_grid(int w, int h, int dx, int dy);
void listSelected(GLint hits, GLuint *buffer);

Shape* findShape(unsigned int id)
{
	for (std::list<Shape>::iterator it = listOfShapes.begin(); it != listOfShapes.end(); it++)
	{
		if(it->shapeId == id)
		{
			return &(*it);
		}
	}
	return NULL;
}

void drawSelectCube()
{
	glColor3f(1.0,1.0,1.0);
	glBegin(GL_LINE_STRIP);
	glVertex3f(SELECT_BOX_SIZE,SELECT_BOX_SIZE,SELECT_BOX_SIZE);
	glVertex3f(-SELECT_BOX_SIZE,SELECT_BOX_SIZE,SELECT_BOX_SIZE);
	glVertex3f(-SELECT_BOX_SIZE,-SELECT_BOX_SIZE,SELECT_BOX_SIZE);
	glVertex3f(SELECT_BOX_SIZE,-SELECT_BOX_SIZE,SELECT_BOX_SIZE);
	glVertex3f(SELECT_BOX_SIZE,SELECT_BOX_SIZE,SELECT_BOX_SIZE);
	glEnd();

	glBegin(GL_LINE_STRIP);
	glVertex3f(SELECT_BOX_SIZE,SELECT_BOX_SIZE,-SELECT_BOX_SIZE);
	glVertex3f(-SELECT_BOX_SIZE,SELECT_BOX_SIZE,-SELECT_BOX_SIZE);
	glVertex3f(-SELECT_BOX_SIZE,-SELECT_BOX_SIZE,-SELECT_BOX_SIZE);
	glVertex3f(SELECT_BOX_SIZE,-SELECT_BOX_SIZE,-SELECT_BOX_SIZE);
	glVertex3f(SELECT_BOX_SIZE,SELECT_BOX_SIZE,-SELECT_BOX_SIZE);
	glEnd();

	glBegin(GL_LINES);
	glVertex3f(SELECT_BOX_SIZE,SELECT_BOX_SIZE,SELECT_BOX_SIZE);
	glVertex3f(SELECT_BOX_SIZE,SELECT_BOX_SIZE,-SELECT_BOX_SIZE);

	glVertex3f(-SELECT_BOX_SIZE,SELECT_BOX_SIZE,SELECT_BOX_SIZE);
	glVertex3f(-SELECT_BOX_SIZE,SELECT_BOX_SIZE,-SELECT_BOX_SIZE);

	glVertex3f(-SELECT_BOX_SIZE,-SELECT_BOX_SIZE,SELECT_BOX_SIZE);
	glVertex3f(-SELECT_BOX_SIZE,-SELECT_BOX_SIZE,-SELECT_BOX_SIZE);

	glVertex3f(SELECT_BOX_SIZE,-SELECT_BOX_SIZE,SELECT_BOX_SIZE);
	glVertex3f(SELECT_BOX_SIZE,-SELECT_BOX_SIZE,-SELECT_BOX_SIZE);
	glEnd();
}

void deleteShape(unsigned int id)
{
	for (std::list<Shape>::iterator it = listOfShapes.begin(); it != listOfShapes.end(); it++)
	{
		if(it->shapeId == id)
		{
			listOfShapes.erase(it);
			break;
		}
	}
}

void drawShapes()
{
	for (std::list<Shape>::iterator it = listOfShapes.begin(); it != listOfShapes.end(); it++)
	{
		glPushMatrix();
		unsigned int id = it->shapeId;
		vec3 transV = it->trans.trans;
		vec3 rotV = it->rot.trans;
		vec3 scaleV = it->scale.trans;
		vec3 color = it->color;
		glVertexAttrib3f(1,color[0],color[1],color[2]);
		glTranslatef(transV[0],transV[1],transV[2]);
		glRotatef(rotV[0],1.0,0.0,0.0);
		glRotatef(rotV[1],0.0,1.0,0.0);
		glRotatef(rotV[2],0.0,0.0,1.0);
		glScalef(scaleV[0],scaleV[1],scaleV[2]);
		glPushName(id);
		setShaderVariables(shaderProg);
		it->drawShape();
		glPopName();
		if(id == selected)
		{
			if(GL20Support)
				glUseProgram(noLightProg);
			else
				glUseProgramObjectARB(noLightProg);
			setShaderVariables(noLightProg);
			drawSelectCube();
			if(GL20Support)
				glUseProgram(shaderProg);
			else
				glUseProgramObjectARB(shaderProg);
		}
		glPopMatrix();
	}
}

void createShape(shape_type st)
{
	Shape s = Shape(st, idCounter, TransformTranslate(vec3(0.0,0.0,3.0)), TransformRotate(vec3(45.0,0.0,0.0)), TransformScale(vec3(2.0,2.0,2.0)));
	listOfShapes.push_back(s);
	selected = idCounter;
	idCounter++;
}

void handleSelection(int x, int y)
{
	glInitNames();
	GLuint hitBuffer[64] = {0};
	glSelectBuffer(64, hitBuffer);
	glRenderMode(GL_SELECT);
	GLint vp[4];
	glGetIntegerv(GL_VIEWPORT,vp);
 	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluPickMatrix(x,currentRes[1]-y,50.0,50.0,vp);
	gluPerspective(45.0,RESOLUTION_X/RESOLUTION_Y, 0.5, 50.0);
	glScalef((float)RESOLUTION_X/currentRes[0], (float)RESOLUTION_Y/currentRes[1], 1.0);
  
	glMatrixMode(GL_MODELVIEW);
	//camera.apply_transformations();
	renderScene();
	glMatrixMode(GL_PROJECTION);
  	//renderScene();
	//setShaderVariables(shaderProg);
  
  	//glMatrixMode(GL_PROJECTION);
  	glPopMatrix();
  	glFlush();
  	GLint hitCount = glRenderMode(GL_RENDER);
  	listSelected(hitCount, hitBuffer);
	glMatrixMode(GL_MODELVIEW);
}

void listSelected(GLint hits, GLuint *buffer)
{
  GLint n = hits;
  int i = 0;
  //printf("Listing selected %d\n",n);
  unsigned int min = ~0;
  unsigned int toSelect = 0;
  while(n > 0)
    {
      GLuint nameCount = buffer[i];
      //printf("count: %u\nz1: %u\nz2: %u\n",buffer[i],buffer[i+1],buffer[i+2]);
      for(int j = 0; j < nameCount; j++)
	{
	if(buffer[i+j+3] != 0)
	{
		if(buffer[i+1] < min)
		{
			min = buffer[i+1];	
			toSelect = buffer[i+j+3];
		}
	}
	//printf("name: %u\n",buffer[i+j+3]);//printName(buffer[i+j+3]);
	}
      n--;
      i+=nameCount+3;
    }
	/*printf("%d hits:\n", hits);
 	int i;
 	for (i = 0; i < hits; i++)
 		printf(	"Number: %d\n"
 				"Min Z: %d\n"
 				"Max Z: %d\n"
 				"Name on stack: %d\n",
 				(GLubyte)buffer[i * 4],
 				(GLubyte)buffer[i * 4 + 1],
 				(GLubyte)buffer[i * 4 + 2],
 				(GLubyte)buffer[i * 4 + 3]
 				);*/
 	selected = toSelect;
 	//printf("%u\n",toSelect);
}

void draw_xy_grid(int w, int h, float dx, float dy)
{
	glColor3f(0.0,0.0,0.0);
	glBegin(GL_LINES);
	float xstart = w*dx/2;
	float ystart = w*dx/2;
	float xacc = -xstart;
	for(int i = 0; i < w+1; i++)
	{
		if(i != w/2)
		{
			glVertex3f(xacc,-ystart,0.0);
			glVertex3f(xacc,ystart,0.0);
		}
		xacc += dx;
	}
	float yacc = -ystart;
	for(int i = 0; i < h+1; i++)
	{
		if(i != h/2)
		{
			glVertex3f(-xstart,yacc,0.0);
			glVertex3f(xstart,yacc,0.0);
		}
		yacc += dy;
	}
	glEnd();

	glLineWidth(3.0);
	glBegin(GL_LINES);

	glColor3f(1.0,0.0,0.0);
	glVertex3f(-xstart,0.0,0.0);
	glVertex3f(xstart,0.0,0.0);
	
	glColor3f(0.0,1.0,0.0);
	glVertex3f(0.0,-ystart,0.0);
	glVertex3f(0.0,ystart,0.0);
	glEnd();
	glLineWidth(1.0);
}

void renderScene()
{
	// Set color and depth clear value
	glClearDepth(1.f);
	glClearColor(0.4f, 0.4f, 0.4f, 0.4f);
	
	// Enable Z-buffer read and write
	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
		
	// Clear color and depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
	//variables for 3D render shader
	if(GL20Support)
		glUseProgram(shaderProg);
	else
		glUseProgramObjectARB(shaderProg);
	setShaderVariables(shaderProg);
		
	//Draw everything
	/*glPushMatrix();
	setShaderVariables(shaderProg);
	drawExampleCube();
	glPopMatrix();*/
	drawShapes();

	//Draw the UI (does not use lighting)
	if(GL20Support)
		glUseProgram(noLightProg);
	else
		glUseProgramObjectARB(noLightProg);
	setShaderVariables(noLightProg);
	draw_xy_grid(20,20,1.0,1.0);

	//copy buffer to texture
	/*glBindTexture(GL_TEXTURE_2D,textureTarget);
	glCopyTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 0, 0, currentRes[0], currentRes[1], 0);
	texRender.useProgram();
		
	//variables for texture shader
	//texture unit id is handled in ShowTexture
	setShaderVariables(texRender.getProgram());
		
	//sets up a few shader variables and draws the texture on a full view quad
	texRender.render(textureTarget, currentRes, currentRes);*/
}

void handleEvents()
{
	sf::Event Event;
	const sf::Input& Input = App->GetInput();
	bool shiftDown = Input.IsKeyDown(sf::Key::LShift) || Input.IsKeyDown(sf::Key::RShift);
	while (App->GetEvent(Event))
	{
		// Close window : exit
		if (Event.Type == sf::Event::Closed)
			App->Close();
		//
		// Escape key : exit
		if (Event.Type == sf::Event::KeyPressed)
		{
			if(Event.Key.Code == sf::Key::Escape)
				App->Close();
			else if(Event.Key.Code == sf::Key::Q)
				axisM = X_AXIS;
			else if(Event.Key.Code == sf::Key::W)
				axisM = Y_AXIS;
			else if(Event.Key.Code == sf::Key::E)
				axisM = Z_AXIS;
			else if(Event.Key.Code == sf::Key::A)
				transM = TRANS_TRANSLATION;
			else if(Event.Key.Code == sf::Key::S)
				transM = TRANS_ROTATION;
			else if(Event.Key.Code == sf::Key::D)
				transM = TRANS_SCALE;
			else if(Event.Key.Code == sf::Key::F1)
				createShape(SHAPE_TETRAHEDRON);
			else if(Event.Key.Code == sf::Key::F2)
				createShape(SHAPE_CUBE);
			else if(Event.Key.Code == sf::Key::F3)
				createShape(SHAPE_OCTAHEDRON);
			else if(Event.Key.Code == sf::Key::F4)
				createShape(SHAPE_SPHERE);
			else if(Event.Key.Code == sf::Key::F5)
				createShape(SHAPE_CYLINDER);
			else if(Event.Key.Code == sf::Key::F6)
				createShape(SHAPE_PLANE);
		}

		// Resize event : adjust viewport
		if (Event.Type == sf::Event::Resized)
		{
			glViewport(0, 0, Event.Size.Width, Event.Size.Height);
			currentRes[0] = Event.Size.Width;
			currentRes[1] = Event.Size.Height;	
			update_perspective();
		}

		if (Event.Type == sf::Event::MouseButtonPressed)
		{	
			lastPos[0] = Event.MouseButton.X;
			lastPos[1] = Event.MouseButton.Y;
			if(Event.MouseButton.Button == sf::Mouse::Left)
			{
				handleSelection(Event.MouseButton.X, Event.MouseButton.Y);
			}

			if(Event.MouseButton.Button == sf::Mouse::Left && !shiftDown)
			{
				buttonDown[0] = 1;
			}
			if(Event.MouseButton.Button == sf::Mouse::Right)
				buttonDown[1] = 1;
			if(Event.MouseButton.Button == sf::Mouse::Middle)
				buttonDown[2] = 1;
			if(Event.MouseButton.Button == sf::Mouse::Left && shiftDown)
				buttonDown[2] = 1;
		}

		if (Event.Type == sf::Event::MouseButtonReleased)
		{
			if(Event.MouseButton.Button == sf::Mouse::Left && !shiftDown)
				buttonDown[0] = 0;
			if(Event.MouseButton.Button == sf::Mouse::Right)
				buttonDown[1] = 0;
			if(Event.MouseButton.Button == sf::Mouse::Middle)
				buttonDown[2] = 0;
			if(Event.MouseButton.Button == sf::Mouse::Left && shiftDown)
				buttonDown[2] = 0;
		}

		if (Event.Type == sf::Event::MouseMoved && (buttonDown[0] || buttonDown[1] || buttonDown[2]) )
		{
			int x = Event.MouseMove.X;
			int y = Event.MouseMove.Y;
			if(selected == 0)
			{
				if(buttonDown[0])
					camera.trackball_rotate(lastPos[0], lastPos[1], x, y, currentRes[0], currentRes[1]);
				if(buttonDown[1])
					camera.trackball_translate(lastPos[0], lastPos[1], x, y);
				if(buttonDown[2])
					camera.trackball_translate_z(lastPos[0], lastPos[1], x, y);
			}
			else
			{
				Shape* s = findShape(selected);
				float diff = (x-lastPos[0]);
				vec3 v;
				switch(axisM)
				{
					case X_AXIS:
						v = vec3(diff,0.0,0.0); break;
					case Y_AXIS:
						v = vec3(0.0,diff,0.0); break;
					case Z_AXIS:
						v = vec3(0.0,0.0,diff); break;
				}
				switch(transM)
				{
					case TRANS_TRANSLATION:
						s->trans.trans += v*0.01; break;
					case TRANS_ROTATION:
						s->rot.trans += v*0.1; break;
					case TRANS_SCALE:
						s->scale.trans += v*0.01; break;
				}
			}
			lastPos[0] = x;
			lastPos[1] = y;
		}
	}
}

void update_perspective()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0,RESOLUTION_X/RESOLUTION_Y, 0.5, 50.0);
	glScalef((float)RESOLUTION_X/currentRes[0], (float)RESOLUTION_Y/currentRes[1], 1.0);
	glMatrixMode(GL_MODELVIEW);
}

void init()
{
	// Create the main window
	App = new sf::Window(sf::VideoMode(RESOLUTION_X, RESOLUTION_Y, 32), "Modeling Program");
		
	// Create a clock for measuring time elapsed
	Clock = sf::Clock();
		
	__glewInit();

	currentRes[0] = RESOLUTION_X;
	currentRes[1] = RESOLUTION_Y;

	transM = TRANS_TRANSLATION;
	axisM = X_AXIS;

	selected = 0;
	idCounter = 1;
	createShape(SHAPE_TETRAHEDRON);

	//Initial light position
	lightPos[0] = 2.0f;
	lightPos[1] = 2.0f;
	lightPos[2] = 2.0f;

	//Initial camera position
	cameraPos[0] = 0.0f;
	cameraPos[1] = 0.0f;
	cameraPos[2] = 0.0f;

	//Perspective setup
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0,currentRes[0]/currentRes[1], 0.5, 50.0);
	glMatrixMode(GL_MODELVIEW);
	
	//Camera setup
	camera = Camera();
	camera.translate(0.0,0.0,20.0);
	camera.rotate(60.0, 1.0, 0.0, 0.0);
	camera.rotate(30.0, 0.0, 0.0, 1.0);
	camera.apply_transformations();
	
	//setup render target texture
	//this will eventually hald the rendered scene and be
	//rendered to a quad for post process effects
	/*int numTex = 1;
	glGenTextures(numTex, &textureTarget);
	setupTargetTexture();*/

	//setup the shader programs
	//the first set is the vertex and fragment shaders for the 3D render
	//the second set is the shaders for the texture post process effects
	ShaderManager shaders;
	shaders.GL20Support = GL20Support;
	char const * drawVert = "Shaders/Render3dModel.vert";
	char const * drawFrag = "Shaders/Render3dModel.frag";
	shaderProg = shaders.buildShaderProgram(&drawVert, &drawFrag, 1, 1);
		
	/*char const * texVert = "Shaders/RenderTexture.vert";
	char const * texFrag = "Shaders/RenderTexture.frag";
	texProg = shaders.buildShaderProgram(&texVert, &texFrag, 1, 1);*/

	char const * noLightVert = "Shaders/NoLighting.vert";
	char const * noLightFrag = "Shaders/NoLighting.frag";
	noLightProg = shaders.buildShaderProgram(&noLightVert, &noLightFrag, 1, 1);

	//this object helps draw textures that fill the viewport
	/*texRender = ShowTexture(texProg);
	texRender.GL20Support = GL20Support;*/

	// Bind custom attributes
	glBindAttribLocation(shaderProg, 1, "ambient");

	// Start render loop
	while (App->IsOpened())
	{
		// Process events
		handleEvents();
		
		// Set the active window before using OpenGL commands
		// It's useless here because active window is always the same,
		// but don't forget it if you use multiple windows or controls
		App->SetActive();
			
		//render 3D scene, then put 2D output in texture, render texture
		renderScene();
						
		// Finally, display rendered frame on screen
		App->Display();
	}
}

void setupTargetTexture()
{
	glBindTexture(GL_TEXTURE_2D,textureTarget);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,  currentRes[0], currentRes[1], 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
}

void setShaderVariables(GLuint shaderProg)
{
	GLfloat projMatrix[16];
	GLfloat viewMatrix[16];
	glGetFloatv(GL_PROJECTION_MATRIX, projMatrix);
	glGetFloatv(GL_MODELVIEW_MATRIX, viewMatrix);
		
	if(GL20Support)
	{
		//TODO: Clean up shaders
		glUniform2f(glGetUniformLocation(shaderProg, "resolution"), currentRes[0], currentRes[1]);
		glUniform3f(glGetUniformLocation(shaderProg, "lightPos"),  lightPos[0], lightPos[1], lightPos[2]);
		glUniform3f(glGetUniformLocation(shaderProg, "cameraPos"), camera.getX(), camera.getY(), camera.getZ());
		glUniform3f(glGetUniformLocation(shaderProg, "ambientLight"), 1.0, 1.0, 1.0);
		glUniform3f(glGetUniformLocation(shaderProg, "diffuseLight"), 1.0, 1.0, 1.0);
		glUniform3f(glGetUniformLocation(shaderProg, "specularLight"), 1.0, 1.0, 1.0);
		glUniform1f(glGetUniformLocation(shaderProg, "a"), 1.0);
		glUniform1f(glGetUniformLocation(shaderProg, "b"), 0.1);
		glUniform1f(glGetUniformLocation(shaderProg, "c"), 0.1);
		glUniformMatrix4fv(glGetUniformLocation(shaderProg, "projMatrix"), 1, false, projMatrix);
		glUniformMatrix4fv(glGetUniformLocation(shaderProg, "viewMatrix"), 1, false, viewMatrix);
	}
	else
	{
		//TODO: Match ARB block with block above
		glUniform2fARB(glGetUniformLocationARB(shaderProg, "resolution"), currentRes[0], currentRes[1]);
		glUniform3fARB(glGetUniformLocationARB(shaderProg, "lightPos"),  lightPos[0], lightPos[1], lightPos[2]);
		glUniformMatrix4fvARB(glGetUniformLocationARB(shaderProg, "projMatrix"), 1, false, projMatrix);
		glUniformMatrix4fvARB(glGetUniformLocationARB(shaderProg, "viewMatrix"), 1, false, viewMatrix);
	}
}

void __glewInit()
{
	GL20Support = false;
	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		/* Problem: glewInit failed, something is seriously wrong. */
		fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
		fprintf(logFile, "Error: %s\n", glewGetErrorString(err));
	}
	else
	{
		printf("GLEW init finished...\n");
		fprintf(logFile, "GLEW init finished...\n");
		if( __GLEW_VERSION_2_0 )
		{
			printf("OpenGL 2.0 is supported. Shaders should run correctly.\n");
			fprintf(logFile, "OpenGL 2.0 is supported. Shaders should run correctly.\n");
			GL20Support = true;
		}
		else
		{
			printf("OpenGL 2.0 is NOT enabled. The program may not work correctly.\n");
			fprintf(logFile, "OpenGL 2.0 is NOT enabled. The program may not work correctly.\n");
		}
		
		if( GLEW_ARB_vertex_program )
		{
			printf("ARB vertex programs supported.\n");
			fprintf(logFile, "ARB vertex programs supported.\n");
		}
		else
		{
			printf("ARB vertex programs NOT supported. The program may not work correctly.\n");
			fprintf(logFile, "ARB vertex programs NOT supported. The program may not work correctly.\n");
		}
		if( GLEW_ARB_fragment_program )
		{
			printf("ARB fragment programs supported.\n");
			fprintf(logFile, "ARB fragment programs supported.\n");
		}
		else
		{
			printf("ARB fragment programs NOT supported. The program may not work correctly.\n");
			fprintf(logFile, "ARB fragment programs NOT supported. The program may not work correctly.\n");
		}
	}
}

int main()
{
#ifdef __APPLE__
#define pathSize 5000
	char path[pathSize];
	uint32_t size = pathSize;
    _NSGetExecutablePath(path, &size);
	char *slashPos = strrchr(path, '/');
	slashPos[0] = '\0';
	chdir(path);
	chdir("../../../");
#endif
	
	GL20Support = false;
	logFile = fopen("log.txt", "w");
	init();
	fclose(logFile);
	
    return EXIT_SUCCESS;
}
