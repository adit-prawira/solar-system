#version 450 core

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoord;

out vec4 FragColor;

uniform vec3 light_position;
uniform vec3 view_position;
uniform vec3 light_color;
uniform vec3 object_color;

void main() {
    // Ambient
    vec3 ambient = 0.2 * light_color;

    // Diffuse
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light_position - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * light_color;

    // Specular
    float specular_strength = 0.5;
    vec3 viewDir = normalize(view_position - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specular_strength * spec * object_color;

    vec3 result = (ambient + diffuse + specular) * object_color;
    FragColor = vec4(result, 1.0);
}
