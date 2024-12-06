#version 410 core

layout(location=0) in vec3 position;
layout(location=1) in vec3 vertexColors;
layout(location=2) in vec2 texCoords;
layout(location=3) in vec3 normal;

out vec3 v_vertexColors;
out vec2 v_texCoords;
out vec3 v_fragPos;
out vec3 v_normal;

uniform mat4 u_ModelMatrix;
uniform mat4 u_ViewMatrix;
uniform mat4 u_Projection;

void main() {
	v_fragPos = vec3(u_ModelMatrix * vec4(position, 1.0f));

	v_normal= mat3(transpose(inverse(u_ModelMatrix))) * normal;

	v_vertexColors = vertexColors;
	v_texCoords = texCoords;

	// MVP Matrix
	vec4 newPosition = u_Projection * u_ViewMatrix * u_ModelMatrix * vec4(position, 1.0f);

	gl_Position = newPosition;
}