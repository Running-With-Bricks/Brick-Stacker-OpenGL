#version 330 core

in vec3 Color;
in vec2 TexCoord;
flat in float TexIndex;

uniform sampler2D TopTexture;

out vec4 FragColor;

void main()
{
	vec4 texColor = vec4(Color, 1);

	switch(int(TexIndex))
	{
		case 1: texColor *= texture(TopTexture, TexCoord); break;
	}

	FragColor = texColor;
}