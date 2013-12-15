#version 330

in vec4 vVertexColor;

layout(location = 0) out vec4 vFragColor;

void main(void)
{
	vFragColor = vVertexColor;
}