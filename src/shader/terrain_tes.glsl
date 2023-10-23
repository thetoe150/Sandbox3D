#version 430 core

// NOTE: TES will be invoked at least once per tessellated vertex in the abstract patch
// each TES invocation outputs a separate vertex worth of data. 
layout (quads, fractional_odd_spacing, ccw) in;

uniform sampler2D heightMap;
uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProjection;

// The inputs for the TES are the per-vertex and per-patch outputs from the TCS or VS.
in vec2 tcsTex[];

out float Height;

// patch in float gl_TessLevelOuter[4];
// patch in float gl_TessLevelInner[2];

void main()
{
	// get patch coordinate
	// Each vertex generated and sent to the TES will be provided a normalized 2D coordinate 
	// as the gl_TessCoord input, representing the location of that vertex within
	// the abstract patch. The third component of gl_TessCoord will be 0.0. 
	//  For isolines and quads, only the XY components have valid values.
	// For triangles, all three components have valid values. 
	// All valid values are normalized floats (on the range [0, 1])
	float u = gl_TessCoord.x;
	float v = gl_TessCoord.y;

	// retrieve texture coordinate
	vec2 t00 = tcsTex[0];
	vec2 t01 = tcsTex[1];
	vec2 t10 = tcsTex[2];
	vec2 t11 = tcsTex[3];

	// bilinearly interpolate texture coordinate across patch
	vec2 t0 = (t01 - t00) * u + t00;
	vec2 t1 = (t11 - t10) * u + t10;
	vec2 texCoord = (t1 - t0) * v + t0;

	// look up texel at patch coordinate for height
	Height = texture(heightMap, texCoord).y * 64.0 - 16.0;

	// retrieve control point position coordinates
	vec4 p00 = gl_in[0].gl_Position;
	vec4 p01 = gl_in[1].gl_Position;
	vec4 p10 = gl_in[2].gl_Position;
	vec4 p11 = gl_in[3].gl_Position;

	// compute patch surface normal
	vec4 uVec = p01 - p00;
	vec4 vVec = p10 - p00;
	vec4 normal = normalize(vec4(cross(vVec.xyz, uVec.xyz), 0));

	// bilinearly interpolate position coordinate across patch
	vec4 p0 = (p01 - p00) * u + p00;
	vec4 p1 = (p11 - p10) * u + p10;
	vec4 p = (p1 - p0) * v + p0;

	// distance point along normal
	p += normal * Height;

	gl_Position = uProjection * uView * uModel * p;
}
