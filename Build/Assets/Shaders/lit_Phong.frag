#version 430

#define POINT 0
#define DIRECTIONAL 1
#define SPOT 2

#define ALBEDO_TEXTURE_MASK    (1 << 0)
#define SPECULAR_TEXTURE_MASK  (1 << 1)
#define NORMAL_TEXTURE_MASK    (1 << 2)
#define EMISSIVE_TEXTURE_MASK  (1 << 3)

//in layout(location = 0) vec3 color;
in layout(location = 0) vec3 fposition;
in layout(location = 1) vec3 fnormal;
in layout(location = 2) vec2 ftexcoord;
//in layout(location = 3) vec4 fcolor;



out layout(location = 0) vec4 ocolor;



layout (binding = 0) uniform sampler2D albedoTexture; 
layout (binding = 1) uniform sampler2D specularTexture;
layout (binding = 2) uniform sampler2D normalTexture;
layout (binding = 3) uniform sampler2D emissiveTexture;

uniform struct Material {
	uint params;
	vec3 albedo;
	vec3 specular;
	float shininess;
	vec3 emissive;

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
	diffuse = (light.color * intensity);

	//specular
	specular = vec3(0);
	if (intensity > 0) {
		//phong
		//vec3 reflection = reflect(-lightDirection, normal);
		vec3 viewDirection = normalize(-position);
		//intensity = max(dot(reflection, viewDirection), 0);
		//blinn phong
		vec3 h = normalize(viewDirection + lightDirection);
		intensity = max(dot(h, normal), 0);

		intensity = pow(intensity, material.shininess);
		specular = vec3(intensity * spotIntensity);
	}
}

float attenuation(in vec3 position1, in vec3 position2, in float range)
{
	float distanceSqr = dot(position1 - position2, position1 - position2);
	float rangeSqr = pow(range, 2.0);
	float attenuation = max(0, 1 - pow((distanceSqr / rangeSqr), 2.0));
	attenuation = pow(attenuation, 2.0);
 
	return attenuation;
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
	vec4 albedoColor = bool(material.params & ALBEDO_TEXTURE_MASK) ? texture(albedoTexture, ftexcoord) : vec4(material.albedo, 1);
	vec4 specularColor = bool(material.params & SPECULAR_TEXTURE_MASK) ? texture(specularTexture, ftexcoord) : vec4(material.specular, 1);
	vec4 emissiveColor = bool(material.params & EMISSIVE_TEXTURE_MASK) ? texture(emissiveTexture, ftexcoord) : vec4(material.emissive, 1);
	
	//set ambient plus emissive color
	ocolor = (vec4(ambientLight, 1) * albedoColor) + emissiveColor;

	for (int i = 0; i < numLights; i++)
	{
		vec3 diffuse;
		vec3 specular;
 
		float attenuation = (lights[i].type == DIRECTIONAL) ? 1 : attenuation(lights[i].position, fposition, lights[i].range);
 
		phong(lights[i], fposition, fnormal, diffuse, specular);
		ocolor += ((vec4(diffuse, 1) * albedoColor) + (vec4(specular, 1) * specularColor)) * attenuation * lights[i].intensity;
	}
}
