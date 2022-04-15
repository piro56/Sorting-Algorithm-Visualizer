#version 430 core
out vec4 FragColor;

in vec3 ourColor;
in vec3 ourPos;
void main()
{
    FragColor = vec4(ourColor.r - 0.1, ourColor.g * ourPos.x, ourColor.b, 1.0);
    //FragColor = vec4(ourColor, 1.0);
}