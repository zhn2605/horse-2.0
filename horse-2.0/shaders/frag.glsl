#version 410 core
out vec4 color;

in vec3 v_vertexColors;
in vec2 v_texCoords;
in vec3 v_fragPos;
in vec3 v_normal;

uniform sampler2D textureSampler;
uniform vec3 u_lightPos;
uniform vec3 u_lightColor;
uniform vec3 u_objectColor;
uniform vec3 u_viewPos;

uniform bool u_useTexture;

void main() {
    // Ambient
    float ambientStrength = 0.1f;
    vec3 ambient = ambientStrength * u_lightColor;
    
    // Diffuse
    vec3 norm = normalize(v_normal);
    vec3 lightDir = normalize(u_lightPos - v_fragPos);
    float diff = max(dot(norm, lightDir), 0.2f);
    vec3 diffuse = diff * u_lightColor;
    
    // Specular
    float specularStrength = 1.0f;
    vec3 viewDir = normalize(u_viewPos - v_fragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0f), 32);
    vec3 specular = specularStrength * spec * u_lightColor;
    
    // Combine
    vec3 result = (ambient + diffuse + specular) * u_objectColor;
    
    
    // Texture blending
    if (u_useTexture) {
        vec4 texColor = texture(textureSampler, v_texCoords);
        color = vec4(result * texColor.rgb, 1.0);
    } else {
        color = vec4(result, 1.0);
    }

    //color = texture(textureSampler, v_texCoords);
}