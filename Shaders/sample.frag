#version 330 core 

out vec4 FragColor; //pixel color
uniform sampler2D tex0;
in vec2 texCoord;

void main()
{//					  r		g	   b	a
	//FragColor = vec4(1.0f, 0.4f, 0.8f, 1.0f);
	FragColor = texture(tex0, texCoord);
}