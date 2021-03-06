#include "./../../include/dynamics_rendering/SkierRenderable.hpp"
#include "./../../include/gl_helper.hpp"
#include "./../../include/log.hpp"

#include "./../../include/Utils.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GL/glew.h>

SkierRenderable::~SkierRenderable()
{
    glcheck(glDeleteBuffers(1, &m_pBuffer));
    glcheck(glDeleteBuffers(1, &m_cBuffer));
    glcheck(glDeleteBuffers(1, &m_nBuffer));
}

SkierRenderable::SkierRenderable(ShaderProgramPtr shaderProgram, ParticlePtr particle, glm::vec3 color) :
HierarchicalRenderable(shaderProgram),
m_particle(particle),
m_pBuffer(0), m_cBuffer(0), m_nBuffer(0)
{
    double bottomRadius = 0.3;
    double topRadius = 0.6;
    double height = 1.7;
    int thetaStep = 20;
    double bottom = 2.8;

    float rColor = color[0] / 255.0;
    float gColor = color[1] / 255.0;
    float bColor = color[2] / 255.0;

    glm::vec3 center(0.0, 0.0, 0.0);

    for (int i = 0; i < thetaStep; ++i) {

        double curr_theta = i*(2.0*M_PI / (double) thetaStep);
        double next_theta = (i+1) * (2.0 * M_PI / (double) thetaStep);

        glm::vec3 faceNormal(1, 0, 0);
        std::array<glm::vec3,3> vTriangles;

        vTriangles[0] = center
            + glm::vec3(bottomRadius * cos(curr_theta),
                bottomRadius * sin(curr_theta),
                bottom);
        vTriangles[1] = center
            + glm::vec3(bottomRadius * cos(next_theta),
                bottomRadius * sin(next_theta),
                bottom);
        vTriangles[2] = center
            + glm::vec3(topRadius * cos(next_theta),
                topRadius * sin(next_theta),
                bottom + height);

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

        vTriangles[0] = center
            + glm::vec3(bottomRadius * cos(curr_theta),
                bottomRadius * sin(curr_theta),
                bottom);
        vTriangles[1] = center
            + glm::vec3(topRadius * cos(next_theta),
                topRadius * sin(next_theta),
                bottom + height);
        vTriangles[2] = center
            + glm::vec3(topRadius * cos(curr_theta),
                topRadius * sin(curr_theta),
                bottom + height);

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

    }

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

void SkierRenderable::do_draw()
{
    //Update the parent and local transform matrix to position the geometric data according to the particle's data.
    const float& pRadius = m_particle->getRadius();
    const glm::vec3& pPosition = m_particle->getPosition();
    float toRotate = m_particle->getAngle();
    float toRotateBody = m_particle->getBodyAngle();
    float toRotateTorso = 3.14 / 8.0;
    glm::mat4 scale = glm::scale(glm::mat4(1.0), glm::vec3(0.3));
    glm::mat4 translate = glm::translate(glm::mat4(1.0), glm::vec3(pPosition));
    glm::mat4 rotate = glm::rotate(glm::mat4(1.0), toRotate, glm::vec3(0,0,1));
    glm::mat4 rotate2 = glm::rotate(glm::mat4(1.0), toRotateBody, glm::vec3(1,0,0));
    glm::mat4 rotate3 = glm::rotate(glm::mat4(1.0), toRotateTorso, glm::vec3(0,1,0));
    setParentTransform(translate*scale*rotate*rotate2*rotate3);

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

void SkierRenderable::do_animate(float time)
{}
