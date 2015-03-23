#version 330

uniform mat4 mProjection, mModelView;

layout(location = 0) in vec4 vVertex;

void main()
{
	gl_Position = mProjection * mModelView * vVertex;
}