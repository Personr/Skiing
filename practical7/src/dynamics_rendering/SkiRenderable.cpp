#include "./../../include/dynamics_rendering/SkiRenderable.hpp"
#include "./../../include/gl_helper.hpp"
#include "./../../include/log.hpp"
#include "./../../include/Utils.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GL/glew.h>

SkiRenderable::~SkiRenderable()
{
    glcheck(glDeleteBuffers(1, &m_pBuffer));
    glcheck(glDeleteBuffers(1, &m_cBuffer));
    glcheck(glDeleteBuffers(1, &m_nBuffer));
}

SkiRenderable::SkiRenderable(ShaderProgramPtr shaderProgram, ParticlePtr particle, glm::vec3 color, bool isLeft) :
    HierarchicalRenderable(shaderProgram),
    m_particle(particle),
    m_isLeft(isLeft),
    m_pBuffer(0), m_cBuffer(0), m_nBuffer(0)
{
    float length = 4.0;
    float width = 0.3;
    float headLength = 0.3;

    float rColor = color[0] / 255.0;
    float gColor = color[1] / 255.0;
    float bColor = color[2] / 255.0;

    glm::vec3 center(0.0, 0.0, 0.0);

    glm::vec3 faceNormal(1, 0, 0);
    std::array<glm::vec3,3> vTriangles;
    vTriangles[0] = center + glm::vec3(0, 0, 0);
    vTriangles[1] = center + glm::vec3(length, 0, 0);
    vTriangles[2] = center + glm::vec3(0, width, 0);
    faceNormal = -glm::normalize(glm::cross(vTriangles[1]-vTriangles[0], vTriangles[2]-vTriangles[0]));

    m_positions.push_back(vTriangles[0]);
    m_positions.push_back(vTriangles[1]);
    m_positions.push_back(vTriangles[2]);
    m_normals.push_back(faceNormal);
    m_normals.push_back(faceNormal);
    m_normals.push_back(faceNormal);
    m_colors.push_back(glm::vec4(rColor,gColor,bColor,1.0));
    m_colors.push_back(glm::vec4(rColor,gColor,bColor,1.0));
    m_colors.push_back(glm::vec4(rColor,gColor,bColor,1.0));

    vTriangles[0] = center + glm::vec3(0, width, 0);
    vTriangles[1] = center + glm::vec3(length, 0, 0);
    vTriangles[2] = center + glm::vec3(length, width, 0);
    faceNormal = -glm::normalize(glm::cross( vTriangles[1]-vTriangles[0], vTriangles[2]-vTriangles[0] ));

    m_positions.push_back(vTriangles[0]);
    m_positions.push_back(vTriangles[1]);
    m_positions.push_back(vTriangles[2]);
    m_normals.push_back(faceNormal);
    m_normals.push_back(faceNormal);
    m_normals.push_back(faceNormal);
    m_colors.push_back(glm::vec4(rColor,gColor,bColor,1.0));
    m_colors.push_back(glm::vec4(rColor,gColor,bColor,1.0));
    m_colors.push_back(glm::vec4(rColor,gColor,bColor,1.0));

    vTriangles[0] = center + glm::vec3(length, 0, 0);
    vTriangles[1] = center + glm::vec3(length + 0.5, width / 2, 0.3);
    vTriangles[2] = center + glm::vec3(length, width, 0);
    faceNormal = -glm::normalize(glm::cross( vTriangles[1]-vTriangles[0], vTriangles[2]-vTriangles[0] ));

    m_positions.push_back(vTriangles[0]);
    m_positions.push_back(vTriangles[1]);
    m_positions.push_back(vTriangles[2]);
    m_normals.push_back(faceNormal);
    m_normals.push_back(faceNormal);
    m_normals.push_back(faceNormal);
    m_colors.push_back(glm::vec4(rColor,gColor,bColor,1.0));
    m_colors.push_back(glm::vec4(rColor,gColor,bColor,1.0));
    m_colors.push_back(glm::vec4(rColor,gColor,bColor,1.0));

    //Create buffers
    glGenBuffers(1, &m_pBuffer); //vertices
    glGenBuffers(1, &m_cBuffer); //colors
    glGenBuffers(1, &m_nBuffer); //normals

    //Activate buffer and send data to the graphics card
    glcheck(glBindBuffer(GL_ARRAY_BUFFER, m_pBuffer));
    glcheck(glBufferData(GL_ARRAY_BUFFER, m_positions.size()*sizeof(glm::vec3), m_positions.data(), GL_STATIC_DRAW));
    glcheck(glBindBuffer(GL_ARRAY_BUFFER, m_cBuffer));
    glcheck(glBufferData(GL_ARRAY_BUFFER, m_colors.size()*sizeof(glm::vec4), m_colors.data(), GL_STATIC_DRAW));
    glcheck(glBindBuffer(GL_ARRAY_BUFFER, m_nBuffer));
    glcheck(glBufferData(GL_ARRAY_BUFFER, m_normals.size()*sizeof(glm::vec3), m_normals.data(), GL_STATIC_DRAW));
}

void SkiRenderable::do_draw()
{

    float skiAngle =  0.85 * 3.14;

    glm::mat4 ParentTransform;
    if (m_particle->getBraking()) {
        ParentTransform = glm::translate(glm::mat4(), glm::vec3(1, -0.18, 2.3));
        ParentTransform = glm::rotate(ParentTransform, skiAngle, glm::vec3(0, 1.0, 0.0));
        if (m_isLeft) {
            ParentTransform = glm::translate(ParentTransform, glm::vec3(0.0, -1.0, 0.0));
            ParentTransform = glm::rotate(ParentTransform, 0.35f, glm::vec3(0, 0.0, 1.0));
        } else {
            ParentTransform = glm::translate(ParentTransform, glm::vec3(0.0, 1.0, 0.0));
            ParentTransform = glm::rotate(ParentTransform, -0.35f, glm::vec3(0, 0.0, 1.0));
        }
    } else {
        ParentTransform = glm::translate(glm::mat4(), glm::vec3(1, -0.18, 2.3));
        ParentTransform = glm::rotate(ParentTransform, skiAngle, glm::vec3(0, 1.0, 0.0));
    }

    setLocalTransform(getLocalTransform());
    setParentTransform(ParentTransform);

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

    if (normalLocation != ShaderProgram::null_location) {
        glcheck(glEnableVertexAttribArray(normalLocation));
        glcheck(glBindBuffer(GL_ARRAY_BUFFER, m_nBuffer));
        glcheck(glVertexAttribPointer(normalLocation, 3, GL_FLOAT, GL_FALSE, 0, (void*)0));
    }

    //Draw triangles elements
    glcheck(glDrawArrays(GL_TRIANGLES,0, m_positions.size()));

    if (positionLocation != ShaderProgram::null_location) {
        glcheck(glDisableVertexAttribArray(positionLocation));
    }
    if (colorLocation != ShaderProgram::null_location) {
        glcheck(glDisableVertexAttribArray(colorLocation));
    }
    if (normalLocation != ShaderProgram::null_location) {
        glcheck(glDisableVertexAttribArray(normalLocation));
    }
}

void SkiRenderable::do_animate(float time)
{}
