#version 330 core 

layout(location = 0) in vec3 aPos;

uniform mat4 transform;
uniform mat4 projection;

void main()
{
	//projection -> camera -> transform -> local point
	gl_Position = projection * transform * vec4(aPos, 1.0);
}