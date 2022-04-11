#pragma once
#include <glad/glad.h>
#include <iostream>
#include "ShaderProgram.h"
#include "VertexArray.h"
#include "BufferObject.h"
class Triangle {
private:

VertexArray vao;
BufferObject vbo = BufferObject(GL_ARRAY_BUFFER);   // vertices, colors
float buffer[18] = {0.0f};   // xyz rgb 6 * 3
float x = 0.5f;     // position x
float y = 0.5f;     // position y
float scale = 1.0f;
float width;
float height;
float rotation = 0.0f;

public:
Triangle(float width, float height);
void setColor(float r, float g, float b);
void setScale(float scale);
void setRotation(float rot);
void setPosition(float x, float y);
void draw();

};

