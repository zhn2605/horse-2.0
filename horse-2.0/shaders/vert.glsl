#version 410 core

layout(location=0) in vec3 position;
layout(location=1) in vec3 vertexColors;
layout(location=2) in vec2 texCoords;

out vec3 v_vertexColors;
out vec2 v_texCoords;

uniform mat4 u_ModelMatrix;
uniform mat4 u_ViewMatrix;
uniform mat4 u_Projection;

void main() {
	v_vertexColors = vertexColors;
	v_texCoords = texCoords;

	// MVP Matrix
	vec4 newPosition = u_Projection * u_ViewMatrix * u_ModelMatrix * vec4(position, 1.0f);

	gl_Position = newPosition;
}