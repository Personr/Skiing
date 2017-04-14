#include "./../../include/dynamics/DampingForceField.hpp"

DampingForceField::DampingForceField(const std::vector<ParticlePtr> particles, const float damping)
{
    m_particles = particles;
    m_damping = damping;
}

DampingForceField::~DampingForceField()
{
}

void DampingForceField::do_addForce()
{
    for (ParticlePtr p : m_particles) {
        /* the scalar product reprensent alignment between skier direction
        * and his speed's directional */
        glm::vec3 direction(std::cos(p->getAngle()), std::sin(p->getAngle()), 0.0);
        float scalar = fabs(dot(glm::normalize(p->getVelocity()), direction));

        if (scalar != scalar) {
            scalar = 0.0;
        }

        float coef = 5.0 * (1.0 - scalar) * m_damping;

        glm::vec3 force(-m_damping * p->getVelocity());
        glm::vec3 force2 = coef * force;//p->incrForce(-m_damping * p->getVelocity());
        p->incrForce(force2);
    }
}

const std::vector<ParticlePtr> DampingForceField::getParticles()
{
    return m_particles;
}

void DampingForceField::setParticles(const std::vector<ParticlePtr>& particles)
{
    m_particles = particles;
}

const float& DampingForceField::getDamping()
{
    return m_damping;
}

void DampingForceField::setDamping(const float& damping)
{
    m_damping = damping;
}
