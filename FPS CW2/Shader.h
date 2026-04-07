#pragma once
#include <string>
#include <glad/glad.h>

// Creates and links a shader program from vertex and fragment shader files
// vertexPath = file path to vertex shader source
// fragmentPath = file path to fragment shader source
unsigned int createShaderProgram(const char* vertexPath, const char* fragmentPath);