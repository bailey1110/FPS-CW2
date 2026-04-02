// Model.cpp

#include "Model.h"
#include <cmath>
#include <fstream>
#include <sstream>
#include <vector>

Model::Model()
{
    VAO = 0;
    VBO = 0;
}

void Model::loadSimpleCube()
{
    vertices = {
        {{-0.5f,-0.5f,-0.5f},{0.0f,0.0f},{ 0.0f, 0.0f,-1.0f}},
        {{ 0.5f,-0.5f,-0.5f},{1.0f,0.0f},{ 0.0f, 0.0f,-1.0f}},
        {{ 0.5f, 0.5f,-0.5f},{1.0f,1.0f},{ 0.0f, 0.0f,-1.0f}},

        {{ 0.5f, 0.5f,-0.5f},{1.0f,1.0f},{ 0.0f, 0.0f,-1.0f}},
        {{-0.5f, 0.5f,-0.5f},{0.0f,1.0f},{ 0.0f, 0.0f,-1.0f}},
        {{-0.5f,-0.5f,-0.5f},{0.0f,0.0f},{ 0.0f, 0.0f,-1.0f}},
    };

    setupMesh();
}

void Model::loadPlane()
{
    vertices = {
        {{-10.0f, 0.0f, -10.0f}, {0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}},
        {{ 10.0f, 0.0f, -10.0f}, {1.0f, 0.0f}, {0.0f, 1.0f, 0.0f}},
        {{ 10.0f, 0.0f,  10.0f}, {1.0f, 1.0f}, {0.0f, 1.0f, 0.0f}},

        {{ 10.0f, 0.0f,  10.0f}, {1.0f, 1.0f}, {0.0f, 1.0f, 0.0f}},
        {{-10.0f, 0.0f,  10.0f}, {0.0f, 1.0f}, {0.0f, 1.0f, 0.0f}},
        {{-10.0f, 0.0f, -10.0f}, {0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}},
    };

    setupMesh();
}

void Model::loadCylinder(int segments)
{
    vertices.clear();

    float radius = 0.1f;
    float length = 2.0f;

    for (int i = 0; i < segments; i++)
    {
        float theta1 = (float)i / segments * 2.0f * 3.14159f;
        float theta2 = (float)(i + 1) / segments * 2.0f * 3.14159f;

        float x1 = cos(theta1) * radius;
        float y1 = sin(theta1) * radius;

        float x2 = cos(theta2) * radius;
        float y2 = sin(theta2) * radius;

        glm::vec3 n1 = glm::normalize(glm::vec3(x1, y1, 0.0f));
        glm::vec3 n2 = glm::normalize(glm::vec3(x2, y2, 0.0f));

        vertices.push_back({ {x1, y1, 0.0f},   {0.0f, 0.0f}, n1 });
        vertices.push_back({ {x2, y2, 0.0f},   {1.0f, 0.0f}, n2 });
        vertices.push_back({ {x2, y2, length}, {1.0f, 1.0f}, n2 });

        vertices.push_back({ {x2, y2, length}, {1.0f, 1.0f}, n2 });
        vertices.push_back({ {x1, y1, length}, {0.0f, 1.0f}, n1 });
        vertices.push_back({ {x1, y1, 0.0f},   {0.0f, 0.0f}, n1 });
    }

    setupMesh();
}

static void parseFaceVertex(const std::string& token, int& positionIndex, int& texCoordIndex, int& normalIndex)
{
    positionIndex = 0;
    texCoordIndex = 0;
    normalIndex = 0;

    size_t firstSlash = token.find('/');
    if (firstSlash == std::string::npos)
    {
        positionIndex = std::stoi(token);
        return;
    }

    std::string posPart = token.substr(0, firstSlash);
    if (!posPart.empty())
        positionIndex = std::stoi(posPart);

    size_t secondSlash = token.find('/', firstSlash + 1);
    if (secondSlash == std::string::npos)
    {
        std::string texPart = token.substr(firstSlash + 1);
        if (!texPart.empty())
            texCoordIndex = std::stoi(texPart);
    }
    else
    {
        std::string texPart = token.substr(firstSlash + 1, secondSlash - firstSlash - 1);
        std::string normPart = token.substr(secondSlash + 1);

        if (!texPart.empty())
            texCoordIndex = std::stoi(texPart);

        if (!normPart.empty())
            normalIndex = std::stoi(normPart);
    }
}

bool Model::loadOBJ(const std::string& path)
{
    std::ifstream file(path);
    if (!file.is_open())
        return false;

    std::vector<glm::vec3> positions;
    std::vector<glm::vec2> texCoords;
    std::vector<glm::vec3> normals;
    vertices.clear();

    std::string line;
    while (std::getline(file, line))
    {
        if (line.empty() || line[0] == '#')
            continue;

        std::stringstream ss(line);
        std::string prefix;
        ss >> prefix;

        if (prefix == "v")
        {
            glm::vec3 pos;
            ss >> pos.x >> pos.y >> pos.z;
            positions.push_back(pos);
        }
        else if (prefix == "vt")
        {
            glm::vec2 uv;
            ss >> uv.x >> uv.y;
            uv.y = 1.0f - uv.y;
            texCoords.push_back(uv);
        }
        else if (prefix == "vn")
        {
            glm::vec3 n;
            ss >> n.x >> n.y >> n.z;
            normals.push_back(glm::normalize(n));
        }
        else if (prefix == "f")
        {
            std::vector<std::string> faceTokens;
            std::string token;

            while (ss >> token)
                faceTokens.push_back(token);

            if (faceTokens.size() < 3)
                continue;

            for (size_t i = 1; i + 1 < faceTokens.size(); ++i)
            {
                std::string tri[3] = { faceTokens[0], faceTokens[i], faceTokens[i + 1] };

                Vertex triVerts[3];
                bool hasNormals = true;

                for (int j = 0; j < 3; ++j)
                {
                    int p = 0, t = 0, n = 0;
                    parseFaceVertex(tri[j], p, t, n);

                    triVerts[j].position = glm::vec3(0.0f);
                    triVerts[j].texCoords = glm::vec2(0.0f);
                    triVerts[j].normal = glm::vec3(0.0f, 1.0f, 0.0f);

                    if (p > 0 && p <= (int)positions.size())
                        triVerts[j].position = positions[p - 1];

                    if (t > 0 && t <= (int)texCoords.size())
                        triVerts[j].texCoords = texCoords[t - 1];

                    if (n > 0 && n <= (int)normals.size())
                        triVerts[j].normal = normals[n - 1];
                    else
                        hasNormals = false;
                }

                if (!hasNormals)
                {
                    glm::vec3 edge1 = triVerts[1].position - triVerts[0].position;
                    glm::vec3 edge2 = triVerts[2].position - triVerts[0].position;
                    glm::vec3 faceNormal = glm::normalize(glm::cross(edge1, edge2));

                    if (glm::length(faceNormal) < 0.0001f)
                        faceNormal = glm::vec3(0.0f, 1.0f, 0.0f);

                    triVerts[0].normal = faceNormal;
                    triVerts[1].normal = faceNormal;
                    triVerts[2].normal = faceNormal;
                }

                vertices.push_back(triVerts[0]);
                vertices.push_back(triVerts[1]);
                vertices.push_back(triVerts[2]);
            }
        }
    }

    if (vertices.empty())
        return false;

    setupMesh();
    return true;
}

void Model::setupMesh()
{
    if (VAO != 0)
        glDeleteVertexArrays(1, &VAO);

    if (VBO != 0)
        glDeleteBuffers(1, &VBO);

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoords));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);
}

void Model::draw()
{
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, (GLsizei)vertices.size());
    glBindVertexArray(0);
}