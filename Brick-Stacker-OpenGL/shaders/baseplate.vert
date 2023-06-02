#version 330 core
layout(location = 0) in vec3 position;
layout(location = 1) in vec2 texCoord;
layout(location = 2) in float texIndex;
layout(location = 3) in vec3 color;
layout(location = 4) in float scale;

uniform mat4 u_ViewProjectionMatrix;

uniform sampler2D TopTexture;

out vec3 Color;
out vec2 TexCoord;
flat out float TexIndex;

void main()
{
	gl_Position = u_ViewProjectionMatrix * vec4(position*scale, 1.0);
	Color = color;
	TexCoord = texCoord * scale;
	TexIndex = texIndex;
}