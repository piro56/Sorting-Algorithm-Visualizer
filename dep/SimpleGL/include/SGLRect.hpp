#pragma once
#include <glad/glad.h>
#include <iostream>

#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
#include "gtc/type_ptr.hpp"

#include "ShaderProgram.hpp"
#include "ShaderManager.hpp"
#include "VertexArray.hpp"
#include "BufferObject.hpp"



/* RECTANGLE
 * TODO COMMENTS
 * 
 * With a shader program, the vertex shader sets "transform" mat4 from glm
 * Be sure to have a uniform mat4 in your vertex shader
 * 
 * Without a shader, all this will do is have 1 vertex attrib ptrs
 * Layout 0: position
 * 
 * With a valid shaderpointer, transform can scale, rotate, and move the triangle.
 */

class SGLRect {
private:
VertexArray vao;
BufferObject ebo = BufferObject(GL_ELEMENT_ARRAY_BUFFER);
BufferObject vbo = BufferObject(GL_ARRAY_BUFFER);   // vertices, colors
float buffer[12] = {};   // xyz
unsigned int indices[6] = {};
float x = 0.0f;     // position x
float y = 0.0f;     // position y 
float scale = 1.0f;
float width;
float height;
float rotation = 0.0f;
glm::mat4 transform;
bool transUpdated = false;
ShaderProgram* shader = NULL;


public:
SGLRect(float width, float height);
SGLRect(float width, float height, ShaderProgram* sp);

void setColor(float r, float g, float b);
void setScale(float scale);
void setRotation(float rot);
void setPosition(float x, float y);
void draw();

};

