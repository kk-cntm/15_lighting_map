#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexPos;

out vec3 FragPos;
out vec3 Normal;
out vec3 LightPos;
out vec2 TexPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 lightPos;

void main()
{
    FragPos = vec3(view * model * vec4(aPos, 1.0f));
    Normal = mat3(transpose(inverse(view * model))) * aNormal;
    LightPos = vec3(view * vec4(lightPos, 1.0f));
    TexPos = aTexPos;
    
    gl_Position = projection * vec4(FragPos, 1.0f);
}
