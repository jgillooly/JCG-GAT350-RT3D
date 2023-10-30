#version 430

in layout(location = 0) vec3 vposition;
in layout(location = 1) vec2 vtexcoord;
in layout(location = 2) vec3 vnormal;

out layout(location = 0) vec3 oposition;
out layout(location = 1) vec3 onormal;
out layout(location = 2) vec2 otexcoord;
//out layout(location = 3) vec4 ocolor;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform struct Material {
	uint params;
	vec3 albedo;
	vec3 specular;
	float shininess;
	vec3 emissive;

	vec2 offset;
	vec2 tiling;
} material;


uniform vec3 ambientLight;

vec3 diffuseLight = vec3(1, 1, 1);
vec3 lightPosition = vec3(0, 8, 0);



void main()
{
	mat4 modelView = view * model;

	// convert position and normal to world-view space
	oposition = vec3(modelView * vec4(vposition, 1));
	onormal = normalize(mat3(modelView) * vnormal);
	otexcoord = (vtexcoord * material.tiling) + material.offset;


	//ocolor = vec4(ads(oposition, onormal), 1);

	mat4 mvp = projection * view * model;
	gl_Position = mvp * vec4(vposition, 1.0);
}
