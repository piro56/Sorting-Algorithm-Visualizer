#version 430 core
out vec4 FragColor;

in vec3 ourColor;

void main()
{
    FragColor = vec4(0.5, 0.9, 0.2, 1.0);
    //FragColor = vec4(ourColor, 1.0);
}