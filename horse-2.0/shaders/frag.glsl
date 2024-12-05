#version 410 core
out vec4 color;

in vec3 v_vertexColors;
in vec2 v_texCoords;

uniform sampler2D textureSampler;

void main() {
   // color = vec4(v_vertexColors.r, v_vertexColors.g, v_vertexColors.b, 1.0f);
   color = texture(textureSampler, v_texCoords);
}