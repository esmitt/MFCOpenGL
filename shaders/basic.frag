#version 330

in vec4 vVertexColor;
in vec2 vVertexUV;
uniform sampler2D sTexture;
uniform int bHasTexture;
layout(location = 0) out vec4 vFragColor;

vec4 getColor(in int bHasTexture)
{
	if(bHasTexture == 0)
		return vVertexColor;
	else
		return texture(sTexture, vVertexUV);
}

void main(void)
{
	//vFragColor = vVertexColor;
	//vFragColor = texture(sTexture, vVertexUV);
	vFragColor = getColor(bHasTexture);
	//vFragColor = vec4(1,0,0,0);
	//vFragColor.a = .5 - vVertexUV.x;
}