#version 430 core
layout (location = 0) in vec3 aPos;

out float xPos;

void main()
{
    gl_Position = vec4(aPos, 1.0);
    xPos = aPos.x;
    //ourColor = aColor;
}