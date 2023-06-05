#version 330 core 

uniform vec3 objectColor;
out vec4 FragColor; //pixel color

void main()
{
	FragColor.rgb = objectColor;
	FragColor.a = 1.0f;
}