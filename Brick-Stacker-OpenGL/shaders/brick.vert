#version 330 core
layout(location = 0) in vec3 position;
layout(location = 1) in vec2 texCoord;
layout(location = 2) in float texIndex;
layout(location = 3) in vec4 color;
layout(location = 4) in vec2 tiling;
layout(location = 5) in vec3 transformCol1;
layout(location = 6) in vec3 transformCol2;
layout(location = 7) in vec3 transformCol3;
layout(location = 8) in vec3 transformCol4;

uniform mat4 u_ViewMatrix;
uniform mat4 u_ProjectionMatrix;

out vec4 Color;
out vec2 TexCoord;
out float TexIndex;
out vec2 TilingFactor;

void main()
{
	mat4 transform;
	transform[0] = vec4(transformCol1, 0);
	transform[1] = vec4(transformCol2, 0);
	transform[2] = vec4(transformCol3, 0);
	transform[3] = vec4(transformCol4, 1);

	gl_Position = u_ProjectionMatrix * u_ViewMatrix * transform * vec4(position, 1.0);
	Color = vec4(color.r, color.g, color.b, color.a);
	TexCoord = vec2(texCoord.x, texCoord.y);
	TexIndex = float(texIndex);
	TilingFactor = vec2(tiling.x, tiling.y);
}