#version 330 core

//in vec3 fragmentColor;
in vec2 UV;

// Ouput data
out vec3 color;

uniform sampler2D myTextureSampler;

void main()
{

	// Output color = red 
	//color = vec3(1,0,0);
	//color = fragmentColor;
	color = texture(myTextureSampler, UV).rgb;

}