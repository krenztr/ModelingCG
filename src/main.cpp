#include "glew/glew.h"
#include <SFML/Window.hpp>
#include <math.h>
#include <stdio.h>

#include <string.h>
#include "Config.h"
#include "ShaderManager.h"
#include "TextureBufferObject.h"
#include "ShowTexture.h"
#include "Camera.h"
#include "shape.h"
#include <list>
#include <map>
#include "button.h"
#include "draw.h"
#include "StateSave.h"

#ifdef __APPLE__
#include <mach-o/dyld.h>
#include <unistd.h>
#include <string.h>
#endif
#define RESOLUTION_X 800
#define RESOLUTION_Y 800
#define SELECT_BOX_SIZE 1
#define FPS 60

typedef enum {X_AXIS, Y_AXIS, Z_AXIS} axis_mode;
typedef enum {TRANS_TRANSLATION, TRANS_ROTATION, TRANS_SCALE} trans_mode;
typedef enum {COLOR_R, COLOR_G, COLOR_B} color_mode;

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
color_mode colorM;

vec3 buttonT = vec3(0.7,0.9,1.0);
vec3 buttonB = vec3(0.1,0.1,0.1);
vec3 textC = vec3(0.0,0.0,0.0);
vec4 padding = vec4(10.0,10.0,10.0,10.0);

Button translateButton = Button("Translate",vec2(0.0),true,false,textC,padding,buttonT, buttonB, true,true);
Button rotateButton = Button("Rotate",vec2(translateButton.width()+5,0.0),true,false,textC,padding,buttonT, buttonB, false,true);
Button scaleButton = Button("Scale",vec2(translateButton.width()+rotateButton.width()+10,0.0),true,false,textC,padding,buttonT, buttonB, false,true);
Button XButton = Button("X",vec2(0.0,translateButton.height()+5),true,false,textC,padding,buttonT, buttonB, true,true);
Button YButton = Button("Y",vec2(XButton.width()+5.0,translateButton.height()+5),true,false,textC,padding,buttonT, buttonB, false,true);
Button ZButton = Button("Z",vec2(XButton.width()+YButton.width()+10,translateButton.height()+5),true,false,textC,padding,buttonT, buttonB, false,true);
Button tetraButton = Button("Tetrahedron",vec2(0,70),true,false,textC,padding,buttonT, buttonB, false,true);
Button cubeButton = Button("Cube",vec2(0,105),true,false,textC,padding,buttonT, buttonB, false,true);
Button octButton = Button("Octahedron",vec2(0,140),true,false,textC,padding,buttonT, buttonB, false,true);
Button sphereButton = Button("Sphere",vec2(0,175),true,false,textC,padding,buttonT, buttonB, false,true);
Button cylinderButton = Button("Cylinder",vec2(0,210),true,false,textC,padding,buttonT, buttonB, false,true);
Button planeButton = Button("Plane",vec2(0,245),true,false,textC,padding,buttonT, buttonB, false,true);
Button deleteButton = Button("Delete",vec2(0,280),true,false,textC,padding,buttonT, buttonB, false,true);
Button RButton = Button("R",vec2(0,325),true,false,textC,padding,buttonT, buttonB, false,true);
Button GButton = Button("G",vec2(30,325),true,false,textC,padding,buttonT, buttonB, false,true);
Button BButton = Button("B",vec2(60,325),true,false,textC,padding,buttonT, buttonB, false,true);
Button loadButton = Button("Load",vec2(0,360),true,false,textC,padding,buttonT, buttonB, false,true);
Button saveButton = Button("Save",vec2(0,395),true,false,textC,padding,buttonT, buttonB, false,true);

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
bool handleButtons(float x, float y);
void drawButtons();

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

void deleteShape()
{
	for (std::list<Shape>::iterator it = listOfShapes.begin(); it != listOfShapes.end(); it++)
	{
		if(it->shapeId == selected)
		{
			selected = 0;
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
	Shape s = Shape(st, idCounter, TransformTranslate(vec3(0.0,0.0,0.0)), TransformRotate(vec3(0.0,0.0,0.0)), TransformScale(vec3(1.0,1.0,1.0)));
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
	gluPickMatrix(x,currentRes[1]-y,5.0,5.0,vp);
	gluPerspective(45.0,RESOLUTION_X/RESOLUTION_Y, 0.5, 50.0);
	glScalef((float)RESOLUTION_X/currentRes[0], (float)RESOLUTION_Y/currentRes[1], 1.0);
  
	glMatrixMode(GL_MODELVIEW);
	renderScene();
	glMatrixMode(GL_PROJECTION);
  
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
  unsigned int min = ~0;
  unsigned int toSelect = 0;
  while(n > 0)
    {
      GLuint nameCount = buffer[i];
      //printf("name count: %u\nz1: %u\nz2: %u\n",buffer[i],buffer[i+1],buffer[i+2]);
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

	glPushMatrix();
	glLoadIdentity();
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(0.0,currentRes[0],0.0,currentRes[1]);
	setShaderVariables(noLightProg);
	drawButtons();
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
	
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

float ffmin(float a, float b)
{
	return (a<b)?a:b;
}

float ffmax(float a, float b)
{
	return (b<a)?a:b;
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
			else if(Event.Key.Code == sf::Key::Delete)
				deleteShape();
			else if(Event.Key.Code == sf::Key::Up && selected != 0)
			{
				Shape* s = findShape(selected);
				switch(colorM)
				{
					case COLOR_R:
						s->color[0] = ffmin(1.0, s->color[0]+0.05); break;
					case COLOR_G:
						s->color[1] = ffmin(1.0, s->color[1]+0.05); break;
					case COLOR_B:
						s->color[2] = ffmin(1.0, s->color[2]+0.05); break;
				}	
			}
			else if(Event.Key.Code == sf::Key::Down && selected != 0)
			{
				Shape* s = findShape(selected);
				switch(colorM)
				{
					case COLOR_R:
						s->color[0] = ffmax(0.0, s->color[0]-0.05); break;
					case COLOR_G:
						s->color[1] = ffmax(0.0, s->color[1]-0.05); break;
					case COLOR_B:
						s->color[2] = ffmax(0.0, s->color[2]-0.05); break;
				}	
			}
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
			float x = Event.MouseButton.X;
			float y = currentRes[1]-Event.MouseButton.Y;
			if(Event.MouseButton.Button == sf::Mouse::Left)
			{
				if(!handleButtons(x,y))
				{
					handleSelection(Event.MouseButton.X, Event.MouseButton.Y);
				}
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
				float diff = (x-lastPos[0])-(y-lastPos[1]);
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

void buttonCheck()
{
	translateButton.isFlipped = false;
	rotateButton.isFlipped = false;
	scaleButton.isFlipped = false;
	XButton.isFlipped = false;
	YButton.isFlipped = false;
	ZButton.isFlipped = false;
	RButton.isFlipped = false;
	GButton.isFlipped = false;
	BButton.isFlipped = false;
	switch(transM)
	{
		case TRANS_TRANSLATION:
			translateButton.isFlipped = true; break;
		case TRANS_ROTATION:
			rotateButton.isFlipped = true; break;
		case TRANS_SCALE:
			scaleButton.isFlipped = true; break;
	}
	switch(axisM)
	{
		case X_AXIS:
			XButton.isFlipped = true; break;
		case Y_AXIS:
			YButton.isFlipped = true; break;
		case Z_AXIS:
			ZButton.isFlipped = true; break;
	}
	switch(colorM)
	{
		case COLOR_R:
			RButton.isFlipped = true; break;
		case COLOR_G:
			GButton.isFlipped = true; break;
		case COLOR_B:
			BButton.isFlipped = true; break;
	}
}

bool handleButtons(float x, float y)
{
	if(translateButton.contains(x,y))
		transM = TRANS_TRANSLATION;
	else if(rotateButton.contains(x,y))
		transM = TRANS_ROTATION;
	else if(scaleButton.contains(x,y))
		transM = TRANS_SCALE;
	else if(XButton.contains(x,y))
		axisM = X_AXIS;
	else if(YButton.contains(x,y))
		axisM = Y_AXIS;
	else if(ZButton.contains(x,y))
		axisM = Z_AXIS;
	else if(tetraButton.contains(x,y))
		createShape(SHAPE_TETRAHEDRON);
	else if(cubeButton.contains(x,y))
		createShape(SHAPE_CUBE);
	else if(octButton.contains(x,y))
		createShape(SHAPE_OCTAHEDRON);
	else if(sphereButton.contains(x,y))
		createShape(SHAPE_SPHERE);
	else if(cylinderButton.contains(x,y))
		createShape(SHAPE_CYLINDER);
	else if(planeButton.contains(x,y))
		createShape(SHAPE_PLANE);
	else if(deleteButton.contains(x,y))
		deleteShape();
	else if(RButton.contains(x,y))
		colorM = COLOR_R;
	else if(GButton.contains(x,y))
		colorM = COLOR_G;
	else if(BButton.contains(x,y))
		colorM = COLOR_B;
	else if(loadButton.contains(x,y))
	{	
		idCounter = load("default.mcg", &listOfShapes);
		if(idCounter <= 0)
		{
			idCounter = 1;
			listOfShapes.clear();
		}
	}
	else if(saveButton.contains(x,y))
		save("default", listOfShapes);
	else return false;
	buttonCheck();
	return true;
}

void drawButtons()
{
	translateButton.drawButton();
	rotateButton.drawButton();
	scaleButton.drawButton();
	XButton.drawButton();
	YButton.drawButton();
	ZButton.drawButton();
	tetraButton.drawButton();
	cubeButton.drawButton();
	octButton.drawButton();
	sphereButton.drawButton();
	cylinderButton.drawButton();
	planeButton.drawButton();
	deleteButton.drawButton();
	RButton.drawButton();
	GButton.drawButton();
	BButton.drawButton();
	loadButton.drawButton();
	saveButton.drawButton();
	
	/*vec4 viewport = getViewport();

	glBegin(GL_TRIANGLES);
	glColor3f(1.0f,0.0f,0.0f);
	glVertex3f(30.0f,viewport[3]-325.0,0.0f);
	glColor3f(0.0f,1.0f,0.0f);
	glVertex3f(0.0f,viewport[3]-375.0,0.0f);
	glColor3f(0.0f,0.0f,1.0f);
	glVertex3f(60.0f,viewport[3]-375.0,0.0f);
	glEnd();*/
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
		Clock.Reset();
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
		float diff = 1.0/FPS-Clock.GetElapsedTime();
		if(diff > 0)
			sf::Sleep(diff);
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
