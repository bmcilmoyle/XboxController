#version 400

uniform sampler2D texture_0;

in vec4 ex_Color;
in vec2 ex_UV;

out vec4 out_Color;

void main(void)
{
	vec4 textColour_0 = texture( texture_0, ex_UV.xy );	 // sampler2D	

	if ((textColour_0.r <= 0.2f) && (textColour_0.g <= 0.2f) && (textColour_0.b <= 0.2f))
			discard; 

	out_Color = textColour_0;
}