#include "Environment.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

void Environment::setup()
{
    model.loadPlane();
    texture.load("floor.jpg");
}

void Environment::draw(unsigned int modelLoc)
{
    glm::mat4 modelMat = glm::mat4(1.0f);
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &modelMat[0][0]);

    texture.bind(0);
    model.draw();
}