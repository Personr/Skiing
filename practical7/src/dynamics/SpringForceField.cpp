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
    
    //Nb:   Compute force ONLY IF the displacement length is above std::numeric_limits<float>::epsilon()
    //      Otherwise the computation is useless
  const ParticlePtr p1=SpringForceField::getParticle1();
  const ParticlePtr p2=SpringForceField::getParticle2();
  if (length(glm::normalize(p1->getPosition()-p2->getPosition()-m_equilibriumLength)) > std::numeric_limits<float>::epsilon()){


  glm::vec3 force1 = (-m_stiffness*    (glm::normalize(p1->getPosition()-p2->getPosition()) -m_equilibriumLength)* ((p1->getPosition()-p2->getPosition())/(glm::normalize(p1->getPosition()-p2->getPosition()))));



  glm::vec3 force1bis = -m_damping*   ((p1->getVelocity()-p2->getVelocity())*    ((p1->getPosition()-p2->getPosition())/(glm::normalize(p1->getPosition()-p2->getPosition()))*    ((p1->getPosition()-p2->getPosition())/(glm::normalize(p1->getPosition()-p2->getPosition())))));


  glm::vec3  force2 =  m_stiffness*    (glm::normalize(p1->getPosition()-p2->getPosition()) -m_equilibriumLength)* ((p1->getPosition()-p2->getPosition())/(glm::normalize(p1->getPosition()-p2->getPosition())));

  glm::vec3 force2bis = m_damping*    ((p1->getVelocity()-p2->getVelocity())*    ((p1->getPosition()-p2->getPosition())/(glm::normalize(p1->getPosition()-p2->getPosition()))*    ((p1->getPosition()-p2->getPosition())/(glm::normalize(p1->getPosition()-p2->getPosition())))));
      
  m_p1->setForce(force1+force1bis);
  m_p2->setForce(force2+force2bis);
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
