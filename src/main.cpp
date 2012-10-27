#include "glew/glew.h"
#include <SFML/Window.hpp>
#include <math.h>

#include "Config.h"
#include "ShaderManager.h"
#include "TextureBufferObject.h"
#include "ShowTexture.h"

#ifdef __APPLE__
#include <mach-o/dyld.h>
#include <unistd.h>
#include <string.h>
#endif

GLuint textureTarget;
GLuint shaderProg;
GLuint texProg;
sf::Window *App;
sf::Clock Clock;
ShowTexture texRender;
int currentRes[2];
float lightPos[3];

void renderScene();
void handleEvents();
void init();
void setupTargetTexture();
void setShaderVariables(GLuint shaderProg);
void __glewInit();

void renderScene()
{
	// Set color and depth clear value
	glClearDepth(1.f);
	glClearColor(0.f, 0.f, 0.f, 0.f);
	
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
		
	//copy buffer to texture
	glBindTexture(GL_TEXTURE_2D,textureTarget);
	glCopyTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 0, 0, currentRes[0], currentRes[1], 0);
	texRender.useProgram();
		
	//variables for texture shader
	//texture unit id is handled in ShowTexture
	setShaderVariables(texRender.getProgram());
		
	//sets up a few shader variables and draws the texture on a full view quad
	texRender.render(textureTarget, currentRes, currentRes);
}

void handleEvents()
{
	sf::Event Event;
	while (App->GetEvent(Event))
	{
		// Close window : exit
		if (Event.Type == sf::Event::Closed)
			App->Close();
		
		// Escape key : exit
		if ((Event.Type == sf::Event::KeyPressed) && (Event.Key.Code == sf::Key::Escape))
			App->Close();

		// Resize event : adjust viewport
		if (Event.Type == sf::Event::Resized)
		{
			glViewport(0, 0, Event.Size.Width, Event.Size.Height);
			currentRes[0] = Event.Size.Width;
			currentRes[1] = Event.Size.Height;
			setupTargetTexture();
		}
	}
}

void init()
{
	// Create the main window
	App = new sf::Window(sf::VideoMode(RES, RES, 32), "Shader Box");
		
	// Create a clock for measuring time elapsed
	Clock = sf::Clock();
		
	__glewInit();

	currentRes[0] = RES;
	currentRes[1] = RES;	

	//Initial light position
	lightPos[0] = 0.0f;
	lightPos[1] = 0.0f;
	lightPos[2] = 0.0f;

	//setup render target texture
	//this will eventually hald the rendered scene and be
	//rendered to a quad for post process effects
	int numTex = 1;
	glGenTextures(numTex, &textureTarget);
	setupTargetTexture();

	//setup the shader programs
	//the first set is the vertex and fragment shaders for the 3D render
	//the second set is the shaders for the texture post process effects
	ShaderManager shaders;
	shaders.GL20Support = GL20Support;
	char const * drawVert = "Shaders/Render3dModel.vert";
	char const * drawFrag = "Shaders/Render3dModel.frag";
	shaderProg = shaders.buildShaderProgram(&drawVert, &drawFrag, 1, 1);
		
	char const * texVert = "Shaders/RenderTexture.vert";
	char const * texFrag = "Shaders/RenderTexture.frag";
	texProg = shaders.buildShaderProgram(&texVert, &texFrag, 1, 1);

	//this object helps draw textures that fill the viewport
	texRender = ShowTexture(texProg);
	texRender.GL20Support = GL20Support;

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
		glUniform2f(glGetUniformLocation(shaderProg, "resolution"), currentRes[0], currentRes[1]);
		glUniform3f(glGetUniformLocation(shaderProg, "lightPos"),  lightPos[0], lightPos[1], lightPos[2]);
		glUniformMatrix4fv(glGetUniformLocation(shaderProg, "projMatrix"), 1, false, projMatrix);
		glUniformMatrix4fv(glGetUniformLocation(shaderProg, "viewMatrix"), 1, false, viewMatrix);
	}
	else
	{
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
