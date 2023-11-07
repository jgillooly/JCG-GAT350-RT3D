#version 430

//in layout(location = 0) vec3 color;
in layout(location = 0) vec2 texcoord;

out layout(location = 0) vec4 ocolor;

uniform struct Material {
	vec3 albedo;

	vec2 offset;
	vec2 tiling;
} material;

layout (binding = 0) uniform sampler2D tex;


void main()
{
	vec4 texcolor = texture(tex, texcoord);
	if (texcolor.a < 0.8) discard;
	ocolor = texcolor * vec4(material.albedo, 1);
}
