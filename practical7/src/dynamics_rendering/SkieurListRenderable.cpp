#include "../../include/dynamics_rendering/SkieurListRenderable.hpp"
#include "../../teachers/Geometries.hpp"
#include <glm/gtc/type_ptr.hpp>

SkieurListRenderable::SkieurListRenderable(ShaderProgramPtr program, std::vector<ParticlePtr>& particles)
    : HierarchicalRenderable(program), m_positionBuffer(0)
{
    for (ParticlePtr p : particles) {
        m_particles.push_back(p);
    }

    std::vector< glm::vec3 > positions;
    std::vector< glm::vec3 > normals;
    std::vector< glm::vec4 > colors;
    teachers::getUnitSphere(positions, normals, 10, 10);

    m_numberOfVertices = positions.size();
    colors.resize(m_numberOfVertices);
    for (size_t i = 0; i < m_numberOfVertices; ++ i) {
        colors[i] = getColor(positions[i].x, -1, 1);
    }

    glcheck(glGenBuffers(1, &m_positionBuffer));
    glcheck(glGenBuffers(1, &m_colorBuffer));
    glcheck(glGenBuffers(1, &m_normalBuffer));

    glcheck(glBindBuffer(GL_ARRAY_BUFFER, m_positionBuffer));
    glcheck(glBufferData(GL_ARRAY_BUFFER, positions.size()*sizeof(glm::vec3), positions.data(), GL_STATIC_DRAW));
    glcheck(glBindBuffer(GL_ARRAY_BUFFER, m_colorBuffer));
    glcheck(glBufferData(GL_ARRAY_BUFFER, colors.size()*sizeof(glm::vec4), colors.data(), GL_STATIC_DRAW));
    glcheck(glBindBuffer(GL_ARRAY_BUFFER, m_normalBuffer));
    glcheck(glBufferData(GL_ARRAY_BUFFER, normals.size()*sizeof(glm::vec3), normals.data(), GL_STATIC_DRAW));
    glcheck(glBindBuffer(GL_ARRAY_BUFFER, 0 ));
}

void SkieurListRenderable::do_draw()
{
    int positionLocation = m_shaderProgram->getAttributeLocation("vPosition");
    int colorLocation = m_shaderProgram->getAttributeLocation("vColor");
    int normalLocation = m_shaderProgram->getAttributeLocation("vNormal");
    int modelLocation = m_shaderProgram->getUniformLocation("modelMat");

    if (modelLocation != ShaderProgram::null_location) {
        glcheck(glEnableVertexAttribArray(positionLocation));
        glcheck(glBindBuffer(GL_ARRAY_BUFFER, m_positionBuffer));
        glcheck(glVertexAttribPointer(positionLocation, 3, GL_FLOAT, GL_FALSE, 0, (void*)0));
    }

    if (colorLocation != ShaderProgram::null_location) {
        glcheck(glEnableVertexAttribArray(colorLocation));
        glcheck(glBindBuffer(GL_ARRAY_BUFFER, m_colorBuffer));
        glcheck(glVertexAttribPointer(colorLocation, 4, GL_FLOAT, GL_FALSE, 0, (void*)0));
    }

    if (normalLocation != ShaderProgram::null_location) {
        glcheck(glEnableVertexAttribArray(normalLocation));
        glcheck(glBindBuffer(GL_ARRAY_BUFFER, m_normalBuffer));
        glcheck(glVertexAttribPointer(normalLocation, 3, GL_FLOAT, GL_FALSE, 0, (void*)0));
    }

    const size_t nparticles = m_particles.size();
    glm::mat4 model = getModelMatrix();
    glm::mat4 transformation(1.0);
    for (size_t i = 0; i < nparticles; ++ i) {
        glm::vec3 position = m_particles[i]->getPosition();
        float scale = m_particles[i]->getRadius();
        transformation[0][0] = scale;
        transformation[1][1] = scale;
        transformation[2][2] = scale;
        transformation[3][0] = position.x;
        transformation[3][1] = position.y;
        transformation[3][2] = position.z;

        glcheck(glUniformMatrix4fv( modelLocation, 1, GL_FALSE, glm::value_ptr(model * transformation)));
        glcheck(glDrawArrays(GL_TRIANGLES, 0, m_numberOfVertices));
    }
}


void SkieurListRenderable::do_animate(float time)
{}

SkieurListRenderable::~SkieurListRenderable()
{
    glcheck(glDeleteBuffers(1, &m_positionBuffer));
    glcheck(glDeleteBuffers(1, &m_colorBuffer));
    glcheck(glDeleteBuffers(1, &m_normalBuffer));
}
