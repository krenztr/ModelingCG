// Computes fragment colors for the model triangles.
uniform vec3 ambientLight;
uniform vec3 diffuseLight;
uniform vec3 specularLight;
uniform float a;
uniform float b;
uniform float c;

varying vec3 normal;
varying vec4 light;
varying vec4 camera;
varying vec3 color;
varying vec4 lightDisplacement;

void main()
{
	vec3 ambientL = ambientLight;	
	vec3 ambientK = color;
	vec3 diffuseL = diffuseLight;
	vec3 diffuseK = color;
	vec3 specularL = specularLight;
	vec3 specularK = color;
	vec3 r = 2.0*dot(vec3(camera),normal)*normal-vec3(camera);
	float specularExp = 1.0;
	float d = length(lightDisplacement);
	float attenuation = 1.0/(a+b*d+c*pow(d,2.0));
	
	vec3 ambientTerm = ambientK*ambientL;
	vec3 diffuseTerm = diffuseK*dot(vec3(light),normal)*diffuseL;
	vec3 specularTerm = specularK*specularL*pow(dot(r,vec3(camera)),specularExp);

	if(diffuseTerm.x<0.0) diffuseTerm.x = 0.0;
	if(diffuseTerm.y<0.0) diffuseTerm.y = 0.0;
	if(diffuseTerm.z<0.0) diffuseTerm.z = 0.0;

	if(specularTerm.x<0.0) specularTerm.x = 0.0;
	if(specularTerm.y<0.0) specularTerm.y = 0.0;
	if(specularTerm.z<0.0) specularTerm.z = 0.0;

	vec3 sum = (ambientTerm+diffuseTerm+specularTerm)*attenuation;
	if(sum.x>1.0) sum.x = 1.0;
	if(sum.y>1.0) sum.y = 1.0;
	if(sum.z>1.0) sum.z = 1.0;
	
	gl_FragColor = vec4(sum,0.0);
}
