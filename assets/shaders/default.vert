#version 330 core

layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec3 aNormal;

uniform mat4 transform;
uniform mat4 model;

out vec3 FragPosition;
out vec3 Normal;

void main() {
    gl_Position = transform * vec4(aPosition, 1.0);

    FragPosition = vec3(model * vec4(aPosition, 1.0));

    mat3 normalMatrix = mat3(transpose(inverse(model)));
    Normal = normalize(normalMatrix * aNormal);
}