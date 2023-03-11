#version 330 core

in vec4 Color;
in vec2 TexCoord;
flat in float TexIndex;

uniform sampler2D TopTexture;
uniform sampler2D BottomTexture;

out vec4 FragColor;

void main()
{
	if( Color.a <= 0.0 )
		discard;

	vec4 texColor = Color;

	switch(int(TexIndex))
	{
		case 1: texColor *= texture(TopTexture, TexCoord); break;
		case 2: texColor *= texture(BottomTexture, TexCoord); break;
	}

	FragColor = texColor;
}