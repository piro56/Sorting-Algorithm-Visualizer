#version 430 core
out vec4 FragColor;

in float xPos;
void main()
{
    float tmp = abs(cos(xPos)) * 0.4;
    FragColor = vec4(0.4 + tmp, 0.2, 0.6, 1.0);
}