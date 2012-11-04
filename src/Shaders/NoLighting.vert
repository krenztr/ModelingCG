uniform mat4 projMatrix;  //projection matrix
uniform mat4 viewMatrix;  //view matrix (camera)
varying vec4 color;

void main()
{	
	vec4 vertex = projMatrix * viewMatrix * gl_Vertex;  //use the uploaded matrix data
	gl_Position = vertex;  //output the transformed vertex
	color = gl_Color;
}
