#include "./../../include/dynamics/SpringForceField.hpp"

SpringForceField::SpringForceField(const ParticlePtr p1, const ParticlePtr p2,
                                   float stiffness, float equilibriumLength,
                                   float damping) :
    m_p1(p1),
    m_p2(p2),
    m_stiffness(stiffness),
    m_equilibriumLength(equilibriumLength),
    m_damping(damping)
{}

  void SpringForceField::do_addForce()
  {
    //TODO: Implement a damped spring
    //Functions to use:
    //glm::length( vec3 ): Return the length of a vector
    //glm::normalize( vec3 ): Return the normalization of a vector
    //Particle::getVelocity(), Particle::getForce(), Particle::setForce()
    //
    //Nb:   Compute force ONLY IF the displacement length is above std::numeric_limits<float>::epsilon()
    //      Otherwise the computation is useless

    ParticlePtr p1 = getParticle1();
    ParticlePtr p2 = getParticle2();
    float dist = glm::length(p1->getPosition() - p2->getPosition());

    if (dist > std::numeric_limits<float>::epsilon()) {
      glm::vec3 norm = glm::normalize(p1->getPosition() - p2->getPosition());
      glm::vec3 idealForce = (- m_stiffness)
      * (dist - m_equilibriumLength)
      * norm;

      glm::vec3 damperForce = (- m_damping)
      * dot((p1->getVelocity() - p2->getVelocity()), norm)
      * norm;

      p1->setForce(p1->getForce() + idealForce + damperForce);
      p2->setForce(p2->getForce() - idealForce - damperForce);

    }
    
  }

ParticlePtr SpringForceField::getParticle1() const
{
    return m_p1;
}

ParticlePtr SpringForceField::getParticle2() const
{
    return m_p2;
}
