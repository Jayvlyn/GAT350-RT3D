#version 430

#define INVERT_MASK	    (1 << 0)
#define GRAYSCALE_MASK  (1 << 1)
#define COLORTINT_MASK  (1 << 2)
#define GRAIN_MASK		(1 << 3)
#define SCANLINE_MASK	(1 << 4)
#define KERNEL_MASK		(1 << 5)

in layout(location = 0) vec2 ftexcoord;
out layout(location = 0) vec4 ocolor;

uniform float blend = 1;
uniform uint params = 0;
uniform vec3 colorTint = {1, 1, 1};
uniform float time = 0; // for grain 
uniform float scanlineSpacing;
uniform float scanlineIntensity;
uniform float kernelOffset;

layout(binding = 0) uniform sampler2D screenTexture;

float random(vec2 st)
{
	return fract(sin(dot(st.xy, vec2(12.9898, 78.233))) * 43758.5453);
}

vec4 invert(in vec4 color)
{
	return vec4(vec3(1) - color.rgb, color.a);
}

vec4 grayscale(in vec4 color)
{
	return vec4(vec3(0.2126 * color.r + 0.7152 * color.g + 0.0722 * color.b), color.a);
}

vec4 colortint(in vec4 color){
	return vec4(color.rgb * colorTint, color.a);
}

vec4 grain(in vec4 color)
{
	return color * random(gl_FragCoord.xy + time);
}

vec4 scanline(in vec4 color) {
	float yPos = ftexcoord.y * textureSize(screenTexture, 0).y;

	float scanline = mod(yPos, scanlineSpacing) < (scanlineSpacing / 2.0) ? scanlineIntensity : 0.0;

	return vec4(mix(color.rgb, vec3(0.0), scanline), color.a);
}

vec4 kernel(in vec4 color) {
	float offset = 1.0 / kernelOffset;

	vec2 offsets[9] = vec2[](
		vec2(-offset, offset), // top left
		vec2(0.0f, offset), // top center
		vec2(offset, offset), // top right
		vec2(-offset, 0.0f), // center left
		vec2(0.0f, 0.0f), // center
		vec2(offset, 0.0f), // center right
		vec2(-offset, -offset), // bottom left
		vec2(0.0f, -offset), // bottom center
		vec2(offset, -offset) // bottom right
	);

	float kernel[9] = float[](
		-1, -1, -1,
		-1,  9, -1,
		-1, -1, -1
	);

	vec3 sampleTex[9];
	for(int i = 0; i < 9; i++)
	{
		sampleTex[i] = vec3(texture(screenTexture, ftexcoord.st + offsets[i])) * color.rgb;
	}
	vec3 col = vec3(0.0);
	for(int i = 0; i < 9; i++) col += sampleTex[i] * kernel[i];

	return vec4(col, 1.0);
}


void main()
{
	vec4 basecolor = texture(screenTexture, ftexcoord);
	vec4 postprocess = basecolor;

	if (bool(params & KERNEL_MASK)) postprocess = kernel(postprocess);
	if (bool(params & INVERT_MASK))	   postprocess = invert(basecolor);
	if (bool(params & GRAYSCALE_MASK)) postprocess = grayscale(postprocess);
	if (bool(params & COLORTINT_MASK)) postprocess = colortint(postprocess);
	if (bool(params & GRAIN_MASK)) postprocess = grain(postprocess);
	if (bool(params & SCANLINE_MASK)) postprocess = scanline(postprocess);

	ocolor = mix(basecolor, postprocess, blend);
}
