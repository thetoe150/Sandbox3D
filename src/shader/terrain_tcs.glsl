#version 430 core

// NOTE: each TCS invocation is only responsible for producing a single vertex of output to the output patch.
// For each patch provided during rendering, n TCS shader invocations will be processed, 
// where n is the number of vertices in the output patch. So if a drawing command draws 20 patches, 
// and each output patch has 4 vertices, there will be a total of 80 separate TCS invocations.
layout (vertices = 4) out;

uniform mat4 uModel;
uniform mat4 uView;

// All inputs from vertex shaders to the TCS are aggregated into arrays, based on the size of the input patch. 
in vec2 vTex[];
out vec2 tcsTex[];

// in gl_PerVertex
// {
// 	vec4 gl_Position;
// 	float gl_PointSize;
// 	float gl_ClipDistance[];
// } gl_in[gl_MaxPatchVertices];

void main()
{
	// execute per vertices
	gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;
	tcsTex[gl_InvocationID] = vTex[gl_InvocationID];

	// execute per patchs
	// Invocation 0 controls tessellation levels for the entire patch
	if(gl_InvocationID == 0)
	{
		const int MIN_TESS_LEVEL = 4;
        const int MAX_TESS_LEVEL = 64;
        const float MIN_DISTANCE = 20;
        const float MAX_DISTANCE = 200;

        vec4 eyeSpacePos00 = uView * uModel * gl_in[0].gl_Position;
        vec4 eyeSpacePos01 = uView * uModel * gl_in[1].gl_Position;
        vec4 eyeSpacePos10 = uView * uModel * gl_in[2].gl_Position;
        vec4 eyeSpacePos11 = uView * uModel * gl_in[3].gl_Position;

        // "distance" from camera scaled between 0 and 1
        float distance00 = clamp((abs(eyeSpacePos00.z) - MIN_DISTANCE) / (MAX_DISTANCE - MIN_DISTANCE), 0.0, 1.0);
        float distance01 = clamp((abs(eyeSpacePos01.z) - MIN_DISTANCE) / (MAX_DISTANCE - MIN_DISTANCE), 0.0, 1.0);
        float distance10 = clamp((abs(eyeSpacePos10.z) - MIN_DISTANCE) / (MAX_DISTANCE - MIN_DISTANCE), 0.0, 1.0);
        float distance11 = clamp((abs(eyeSpacePos11.z) - MIN_DISTANCE) / (MAX_DISTANCE - MIN_DISTANCE), 0.0, 1.0);

        float tessLevel0 = mix(MAX_TESS_LEVEL, MIN_TESS_LEVEL, min(distance10, distance00));
        float tessLevel1 = mix(MAX_TESS_LEVEL, MIN_TESS_LEVEL, min(distance00, distance01));
        float tessLevel2 = mix(MAX_TESS_LEVEL, MIN_TESS_LEVEL, min(distance01, distance11));
        float tessLevel3 = mix(MAX_TESS_LEVEL, MIN_TESS_LEVEL, min(distance11, distance10));

        gl_TessLevelOuter[0] = tessLevel0;
        gl_TessLevelOuter[1] = tessLevel1;
        gl_TessLevelOuter[2] = tessLevel2;
        gl_TessLevelOuter[3] = tessLevel3;

        gl_TessLevelInner[0] = max(tessLevel1, tessLevel3);
        gl_TessLevelInner[1] = max(tessLevel0, tessLevel2);
	}
}
