#version 330

uniform vec3 vColor;
layout(location = 0) out vec4 vFragColor;

void main(void)
{
	vFragColor = vec4(vColor, 0);
}