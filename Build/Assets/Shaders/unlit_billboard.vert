#version 430

in layout(location = 0) vec3 vposition;
in layout(location = 1) vec2 texcoord;

out layout(location = 0) vec2 otexcoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform struct Material {
	vec3 albedo;

	vec2 offset;
	vec2 tiling;
} material;

void main()
{
	otexcoord = (texcoord * material.tiling) + material.offset;

	vec3 position = model[3].xyz;

	vec2 size = vec2(length(model[0]), length(model[1]));

	vec3 right = vec3(view[0][0], view[1][0], view[2][0]);

	vec3 up = { 0, 1, 0};
	
	position = position + (vposition.x * right * size.x) + (vposition.y * up * size.y);

	mat4 vp = projection * view;
	gl_Position = vp * vec4(position, 1.0);
}
