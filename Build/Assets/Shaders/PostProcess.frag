#version 430
#define INVERT_MASK    (1 << 0)
#define GRAYSCALE_MASK (1 << 1)
#define COLORTINT_MASK (1 << 2)
#define GRAIN_MASK	   (1 << 3)
#define SCANLINE_MASK  (1 << 4)
#define OUTLINE_MASK  (1 << 5)

//in layout(location = 0) vec3 color;
in layout(location = 0) vec2 ftexcoord;

out layout(location = 0) vec4 ocolor;

uniform float blend = 1;
uniform uint params = 0;

uniform vec3 tint;

layout (binding = 0) uniform sampler2D screenTexture;

float random(vec2 st) 
{
	return fract(sin(dot(st.xy, vec2(12.9898, 78.233))) * 43758.5453);
}

vec4 invert(in vec4 color) {
	return vec4(vec3(1) - color.rgb, color.a);
}

vec4 grayscale(in vec4 color) {
	return vec4(vec3((color.r + color.g + color.g)/3), color.a);
}

vec4 tinting(in vec4 color) {
	return vec4(tint, 1) * color;
}

vec4 grain(in vec4 color) {
	float number = random(vec2(gl_FragCoord.x, gl_FragCoord.y));
	if (number > 0.25) return color;
	return vec4 (0,0,0,1);
}

vec4 scan(in vec4 color) {
	float modulus = mod(gl_FragCoord.y,2);
	if (modulus < 1.0) return vec4(0,0,0,1);
	return color;
}

vec4 sharpening(in vec4 color) {
	ivec2 texture_size = textureSize(screenTexture, 0);
    vec2 step_size = 1.0/texture_size;

    vec2 offsets[9] = vec2[](
        vec2(-step_size.x,  -step_size.y),
        vec2( 0.0f,         -step_size.y),
        vec2( step_size.x,  -step_size.y),
        vec2(-step_size.x,   0.0f),
        vec2( 0.0f,          0.0f),
        vec2( step_size.x,   0.0f),
        vec2(-step_size.x,  +step_size.y),
        vec2( 0.0f,         +step_size.y),
        vec2( step_size.x,   step_size.y)
    );

    float kernel[9] = float[](
        0, -1, 0,
        -1, 4, -1,
        0, -1, 0
    );

    vec4 sum = vec4(0.0);
    for(int i = 0; i < 9; i++) {
        sum += texture(screenTexture, ftexcoord + offsets[i]) * kernel[i];
    }

    sum.a = 1.0;
    return sum;
}


void main()
{
	vec4 basecolor = texture(screenTexture, ftexcoord);
	vec4 postprocess = basecolor;
	if (bool(params & OUTLINE_MASK)) postprocess = sharpening(postprocess);
	if (bool(params & INVERT_MASK)) postprocess = invert(postprocess);
	if (bool(params & GRAYSCALE_MASK)) postprocess = grayscale(postprocess);
	if (bool(params & COLORTINT_MASK)) postprocess = tinting(postprocess);
	if (bool(params & GRAIN_MASK)) postprocess = grain(postprocess);
	if (bool(params & SCANLINE_MASK)) postprocess = scan(postprocess);

	ocolor = mix(basecolor, postprocess, blend);
}
