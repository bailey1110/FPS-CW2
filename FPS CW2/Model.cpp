// Model.cpp
#include "Model.h"
#include <cmath>

Model::Model()
{
    VAO = 0;
    VBO = 0;
}

void Model::loadSimpleCube()
{
    vertices = {
        {{-0.5f,-0.5f,-0.5f},{0.0f,0.0f}},
        {{ 0.5f,-0.5f,-0.5f},{1.0f,0.0f}},
        {{ 0.5f, 0.5f,-0.5f},{1.0f,1.0f}},

        {{ 0.5f, 0.5f,-0.5f},{1.0f,1.0f}},
        {{-0.5f, 0.5f,-0.5f},{0.0f,1.0f}},
        {{-0.5f,-0.5f,-0.5f},{0.0f,0.0f}},
    };

    setupMesh();
}

void Model::loadPlane()
{
    vertices = {
        {{-10.0f, 0.0f, -10.0f}, {0.0f, 0.0f}},
        {{ 10.0f, 0.0f, -10.0f}, {1.0f, 0.0f}},
        {{ 10.0f, 0.0f,  10.0f}, {1.0f, 1.0f}},

        {{ 10.0f, 0.0f,  10.0f}, {1.0f, 1.0f}},
        {{-10.0f, 0.0f,  10.0f}, {0.0f, 1.0f}},
        {{-10.0f, 0.0f, -10.0f}, {0.0f, 0.0f}},
    };

    setupMesh();
}

void Model::loadCylinder(int segments)
{
    vertices.clear();

    float radius = 0.1f;
    float length = 2.0f; // halved

    for (int i = 0; i < segments; i++)
    {
        float theta1 = (float)i / segments * 2.0f * 3.14159f;
        float theta2 = (float)(i + 1) / segments * 2.0f * 3.14159f;

        float x1 = cos(theta1) * radius;
        float y1 = sin(theta1) * radius;

        float x2 = cos(theta2) * radius;
        float y2 = sin(theta2) * radius;

        vertices.push_back({ {x1, y1, 0.0f},   {0.0f, 0.0f} });
        vertices.push_back({ {x2, y2, 0.0f},   {1.0f, 0.0f} });
        vertices.push_back({ {x2, y2, length}, {1.0f, 1.0f} });

        vertices.push_back({ {x2, y2, length}, {1.0f, 1.0f} });
        vertices.push_back({ {x1, y1, length}, {0.0f, 1.0f} });
        vertices.push_back({ {x1, y1, 0.0f},   {0.0f, 0.0f} });
    }

    setupMesh();
}

void Model::setupMesh()
{
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoords));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
}

void Model::draw()
{
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, vertices.size());
    glBindVertexArray(0);
}