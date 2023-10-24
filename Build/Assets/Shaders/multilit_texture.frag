#version 430

#define POINT 0
#define DIRECTIONAL 1
#define SPOT 2

//in layout(location = 0) vec3 color;
in layout(location = 0) vec3 fposition;
in layout(location = 1) vec3 fnormal;
in layout(location = 2) vec2 ftexcoord;
//in layout(location = 3) vec4 fcolor;



out layout(location = 0) vec4 ocolor;



layout (binding = 0) uniform sampler2D tex;

uniform struct Material {
	vec3 diffuse;
	vec3 specular;
	float shininess;

	vec2 offset;
	vec2 tiling;
} material;

uniform struct Light {
	int type;
	vec3 position;
	vec3 direction;
	vec3 color;
	float intensity;
	float range;
	float innerAngle;
	float outerAngle;
} lights[3];

uniform vec3 ambientLight;
uniform int numLights = 3;

void phong(in Light light, in vec3 position, in vec3 normal, out vec3 diffuse, out vec3 specular) {
	//diffuse
	vec3 lightDirection = (light.type == DIRECTIONAL) ? normalize(-light.direction) : normalize(light.position - position);
	
	float spotIntensity = 1;
	if (light.type == SPOT) {
		float angle = acos(dot(light.direction, -lightDirection));
		spotIntensity = smoothstep(light.outerAngle + 0.001, light.innerAngle, angle);
	}
	
	float intensity = max(dot(lightDirection, normal), 0) * spotIntensity;
	diffuse = material.diffuse * (light.color * intensity);

	//specular
	specular = vec3(0);
	if (intensity > 0) {
		vec3 reflection = reflect(-lightDirection, normal);
		vec3 viewDirection = normalize(-position);
		intensity = max(dot(reflection, viewDirection), 0);
		intensity = pow(intensity, material.shininess);
		specular = material.specular * intensity * spotIntensity;
	}
}

//vec3 ads(in vec3 position, in vec3 normal) {
//	//ambient
//	vec3 ambient = ambientLight;
//
//	//attenuation
//	float attenuation = 1;
//	if (light.type != DIRECTIONAL) {
//		float dSqr = dot(light.position - position, light.position - position);
//		float rangeSqr = light.range * light.range;
//		attenuation = pow(max(0, 1-pow((dSqr/rangeSqr), 2.0)), 2.0);
//	}
//
//
//	
//
//	return ambient + (diffuse + specular) * light.intensity * attenuation;
//}

void main()
{
	vec4 texcolor = texture(tex, ftexcoord);
	ocolor = vec4(ambientLight, 1);

	for (int i = 0; i < numLights; i++) {
		vec3 diffuse;
		vec3 specular;
		phong(lights[i], fposition, fnormal, diffuse, specular);
		ocolor += vec4(diffuse, 1) * texcolor + vec4(specular, 1);
	}
}
