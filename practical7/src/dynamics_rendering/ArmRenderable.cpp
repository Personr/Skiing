#include "./../../include/dynamics_rendering/ArmRenderable.hpp"
#include "./../../include/dynamics_rendering/BodyCylinderRenderable.hpp"
#include "./../../include/gl_helper.hpp"
#include "./../../include/log.hpp"
#include "./../../include/Utils.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GL/glew.h>

ArmRenderable::~ArmRenderable()
{
}

ArmRenderable::ArmRenderable(ShaderProgramPtr shaderProgram, ParticlePtr particle, glm::vec3 color, bool isLeft) :
    BodyCylinderRenderable(shaderProgram, color),
    m_particle(particle),
    m_isLeft(isLeft)
{

}


void ArmRenderable::do_draw()
{
    //Update the parent and local transform matrix to position the geometric data according to the particle's data.
    float armAngle = 2.5 * 3.14 / 4.0;
    float toRotateBody = 1.5 * m_particle->getBodyAngle();

    glm::mat4 ParentTransform;
    if (m_isLeft) {
        ParentTransform = glm::translate(glm::mat4(), glm::vec3(-0.5, -0.8, 4.5));
        ParentTransform = glm::rotate(ParentTransform, armAngle, glm::vec3(0.3, 1.0, 0.0));
        glm::mat4 rotate = glm::rotate(glm::mat4(1.0), toRotateBody, glm::vec3(0,1,0));
        ParentTransform = ParentTransform * rotate;
    } else {
        ParentTransform = glm::translate(glm::mat4(), glm::vec3(-0.5, 0.8, 4.5));
        ParentTransform = glm::rotate(ParentTransform, armAngle, glm::vec3(-0.3, 1.0, 0.0));
        glm::mat4 rotate = glm::rotate(glm::mat4(1.0), -toRotateBody, glm::vec3(0,1,0));
        ParentTransform = ParentTransform * rotate;
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
