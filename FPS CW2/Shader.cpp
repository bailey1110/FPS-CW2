#include "Shader.h"
#include <fstream>
#include <sstream>
#include <iostream>

// Load shader source code from file into a string
std::string loadShaderSource(const char* filePath)
{
    std::ifstream file(filePath); // Open file
    std::stringstream buffer;

    if (!file.is_open())
    {
        std::cout << "Failed to open shader file: " << filePath << std::endl; // Error if file fails
        return "";
    }

    buffer << file.rdbuf(); // Read entire file into buffer
    return buffer.str();    // Return as string
}

// Compile a shader (vertex or fragment)
unsigned int compileShader(unsigned int type, const char* source)
{
    unsigned int shader = glCreateShader(type); // Create shader object
    glShaderSource(shader, 1, &source, NULL);   // Attach source code
    glCompileShader(shader);                   // Compile shader

    int success;
    char infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success); // Check compile status

    if (!success)
    {
        glGetShaderInfoLog(shader, 512, NULL, infoLog); // Get error log
        std::cout << "Shader compilation error:\n" << infoLog << std::endl;
    }

    return shader; // Return compiled shader ID
}

// Create full shader program from vertex + fragment shader files
unsigned int createShaderProgram(const char* vertexPath, const char* fragmentPath)
{
    // Load shader source code
    std::string vertexCode = loadShaderSource(vertexPath);
    std::string fragmentCode = loadShaderSource(fragmentPath);

    const char* vShaderCode = vertexCode.c_str();
    const char* fShaderCode = fragmentCode.c_str();

    // Compile shaders
    unsigned int vertex = compileShader(GL_VERTEX_SHADER, vShaderCode);
    unsigned int fragment = compileShader(GL_FRAGMENT_SHADER, fShaderCode);

    // Create shader program
    unsigned int program = glCreateProgram();
    glAttachShader(program, vertex);   // Attach vertex shader
    glAttachShader(program, fragment); // Attach fragment shader
    glLinkProgram(program);            // Link program

    int success;
    char infoLog[512];
    glGetProgramiv(program, GL_LINK_STATUS, &success); // Check link status

    if (!success)
    {
        glGetProgramInfoLog(program, 512, NULL, infoLog); // Get error log
        std::cout << "Shader linking error:\n" << infoLog << std::endl;
    }

    // Delete shaders after linking (no longer needed)
    glDeleteShader(vertex);
    glDeleteShader(fragment);

    return program; // Return program ID
}