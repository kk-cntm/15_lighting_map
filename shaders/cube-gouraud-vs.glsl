#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

out vec3 LightColor;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 lightPos;
uniform vec3 lightColor;

void main()
{
    vec3 fragPos = vec3(view * model * vec4(aPos, 1.0f));
    vec3 normal = mat3(transpose(inverse(view * model))) * aNormal;
    vec3 lightPosView = vec3(view * vec4(lightPos, 1.0f));
    
    // ambient
    float ambientStrength = 0.2;
    vec3 ambient = ambientStrength * lightColor;
      
    // diffuse
    vec3 norm = normalize(normal);
    vec3 lightDir = normalize(lightPosView - fragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;
    
    float specularStrength = 1;
    vec3 viewDir = normalize(-fragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * lightColor;
    
    LightColor = ambient + diffuse + specular;
    
    gl_Position = projection * vec4(fragPos, 1.0f);
}
