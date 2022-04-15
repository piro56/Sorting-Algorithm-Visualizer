#version 430 core
layout (location = 0) in vec3 aPos;

out vec3 ourColor;
out vec3 ourPos;
uniform mat4 transform;
uniform vec3 aColor;
void main()
{
    gl_Position = transform * vec4(aPos, 1.0);
    ourPos = gl_Position.xyz;
    ourColor = aColor;
}