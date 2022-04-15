#version 430 core
layout (location = 0) in vec3 aPos;

out vec3 ourColor;

uniform mat4 transform;
uniform vec3 aColor;
void main()
{
    gl_Position = transform * vec4(aPos, 1.0);
    
    ourColor = aColor;
}