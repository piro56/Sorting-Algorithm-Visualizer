#version 430 core
out vec4 FragColor;

in vec3 ourColor;
in vec3 ourPos;
void main()
{
    float tmp = abs(cos(ourPos.x)) * 0.4;
    FragColor = vec4(0.4 + tmp, 0.2, 0.6, 1.0) * vec4(ourColor, 0.0);
    //FragColor = vec4(ourColor.r - 0.1, ourColor.g * ourPos.x, ourColor.b, 1.0);
    //FragColor = vec4(ourColor, 1.0);
}