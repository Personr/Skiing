#include "./../include/SlalomRenderable.hpp"
#include "./../include/gl_helper.hpp"
#include <glm/gtc/type_ptr.hpp>
#include <GL/glew.h>

#include <iostream>

/* NOTE: remember to always use the glcheck macro for gl commands.
 */

SlalomRenderable::~SlalomRenderable()
{
    glcheck(glDeleteBuffers(1, &m_pBuffer));
    glcheck(glDeleteBuffers(1, &m_cBuffer));
}

SlalomRenderable::SlalomRenderable(ShaderProgramPtr shaderProgram, int x, int y, int z, int isLeft)
  // This ":" symbol start a constructor initialization section.
  // It is a good practice to initialize data and acquire resources
  // here. It faster to perform initialization here than in the
  // constructor body.
  : Renderable(shaderProgram), // call the Renderable constructor
    m_pBuffer(0), m_cBuffer(0) // initialize the buffers to default value (useful for debugging purposes)
{
  initAttributes(x, y, z, isLeft);

    glcheck(glGenBuffers(1, &m_pBuffer)); //vertices
    glcheck(glGenBuffers(1, &m_cBuffer)); //colors

    //Activate buffer and send data to the graphics card
    glcheck(glBindBuffer(GL_ARRAY_BUFFER, m_pBuffer));
    glcheck(glBufferData(GL_ARRAY_BUFFER, m_positions.size()*sizeof(glm::vec3), m_positions.data(), GL_STATIC_DRAW));
    glcheck(glBindBuffer(GL_ARRAY_BUFFER, m_cBuffer));
    glcheck(glBufferData(GL_ARRAY_BUFFER, m_colors.size()*sizeof(glm::vec4), m_colors.data(), GL_STATIC_DRAW));
}

void SlalomRenderable::initAttributes(int x, int y, int z, int isLeft)
{

    float height = 3.0;
    float stickWidth = 0.1;
    float width = 1.5;
    glm::vec4 color;
    std::cerr << isLeft << std::endl;
    if (isLeft > 0) {
        color = glm::vec4(0,0,1,1);
    } else {
        color = glm::vec4(1,0,0,1);
    }
    float flagBottom = 1.8;
    float flagTop = 2.8;

    m_positions.push_back(glm::vec3(x, y, z));
    m_positions.push_back(glm::vec3(x, y + stickWidth, z));
    m_positions.push_back(glm::vec3(x, y + stickWidth, z + height));
    m_positions.push_back(glm::vec3(x, y, z));
    m_positions.push_back(glm::vec3(x, y + stickWidth, z + height));
    m_positions.push_back(glm::vec3(x, y, z + height));

    m_positions.push_back(glm::vec3(x, y + width, z));
    m_positions.push_back(glm::vec3(x, y + width + stickWidth, z));
    m_positions.push_back(glm::vec3(x, y + width + stickWidth, z + height));
    m_positions.push_back(glm::vec3(x, y + width, z));
    m_positions.push_back(glm::vec3(x, y + width + stickWidth, z + height));
    m_positions.push_back(glm::vec3(x, y + width, z + height));

    m_positions.push_back(glm::vec3(x, y + stickWidth, z + flagBottom));
    m_positions.push_back(glm::vec3(x, y + width + stickWidth, z + flagBottom));
    m_positions.push_back(glm::vec3(x, y + width + stickWidth, z + flagTop));
    m_positions.push_back(glm::vec3(x, y + stickWidth, z + flagBottom));
    m_positions.push_back(glm::vec3(x, y + width + stickWidth, z + flagTop));
    m_positions.push_back(glm::vec3(x, y + stickWidth, z + flagTop));


    for (int i = 0; i < 18; i++) {
        m_colors.push_back(color);
    }

}

void SlalomRenderable::do_draw()
{
    //Draw geometric data
    int positionLocation = m_shaderProgram->getAttributeLocation("vPosition");
    int colorLocation = m_shaderProgram->getAttributeLocation("vColor");
    int normalLocation = m_shaderProgram->getAttributeLocation("vNormal");
    int modelLocation = m_shaderProgram->getUniformLocation("modelMat");

    if (modelLocation != ShaderProgram::null_location) {
        glcheck(glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(getModelMatrix())));
    }

    if (positionLocation != ShaderProgram::null_location) {
        glcheck(glEnableVertexAttribArray(positionLocation));
        glcheck(glBindBuffer(GL_ARRAY_BUFFER, m_pBuffer));
        glcheck(glVertexAttribPointer(positionLocation, 3, GL_FLOAT, GL_FALSE, 0, (void*)0));
    }

    if (colorLocation != ShaderProgram::null_location) {
        glcheck(glEnableVertexAttribArray(colorLocation));
        glcheck(glBindBuffer(GL_ARRAY_BUFFER, m_cBuffer));
        glcheck(glVertexAttribPointer(colorLocation, 4, GL_FLOAT, GL_FALSE, 0, (void*)0));
    }

    //Draw triangles elements
    glcheck(glDrawArrays(GL_TRIANGLES,0, m_positions.size()));

    if (positionLocation != ShaderProgram::null_location) {
        glcheck(glDisableVertexAttribArray(positionLocation));
    }
    if (colorLocation != ShaderProgram::null_location) {
        glcheck(glDisableVertexAttribArray(colorLocation));
    }
}

void SlalomRenderable::do_animate(float time)
{}
