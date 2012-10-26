// All model vertices and normals are passed to this shader.
// It must transform the vertices as appropriate for the view.
// The view and projection matrices are provided, if you need
// the normal matrix, you must construct it in the shader.

uniform vec3 cameraPos;  //camera position
uniform vec3 lightPos;  //light position
uniform mat4 projMatrix;  //projection matrix
uniform mat4 viewMatrix;  //view matrix (camera)
uniform vec3 ambientLight;
uniform vec3 diffuseLight;
uniform vec3 specularLight;

varying vec3 normal;  //normal that will be interpolated for the fragment shader
varying vec4 light;
varying vec4 camera;
varying vec3 color;
varying vec4 lightDisplacement;

attribute vec3 ambient;

void main()
{	
	vec4 vertex = projMatrix * viewMatrix * gl_Vertex;  //use the uploaded matrix data
	gl_Position = vertex;  //output the transformed vertex
	color = ambient;
	normal = gl_Normal;
	lightDisplacement = vec4(lightPos,1.0)-gl_Vertex;
	camera = normalize(vec4(cameraPos,1.0)-gl_Vertex);
	light = normalize(lightDisplacement);
}
