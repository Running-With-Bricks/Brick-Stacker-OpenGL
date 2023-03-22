#version 330 core
in float a;

out vec2 TexCoord;

vec2 pos[] = vec2[](
	vec2(0, 0), vec2(1, 0),
	vec2(0, 1), vec2(1, 1)
);

void main()
{
	gl_Position = vec4(pos[gl_VertexID], 0, 1);
	TexCoord = pos[gl_VertexID];
}