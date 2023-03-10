#version 330

in vec4 Color;
in vec2 TexCoord;
in float TexIndex;
in vec2 TilingFactor;

uniform sampler2D TopTexture;
uniform sampler2D BottomTexture;

out vec4 FragColor;

void main()
{
	if( Color.a <= 0 )
		discard;

	FragColor = Color;

	switch(int(TexIndex))
	{
		case 1: FragColor *= texture(TopTexture, TexCoord*TilingFactor); break;
		case 2: FragColor *= texture(BottomTexture, TexCoord*TilingFactor); break;
	}
}