#include "./../../include/dynamics/ParticlePlaneCollision.hpp"

ParticlePlaneCollision::ParticlePlaneCollision(ParticlePtr particle, PlanePtr plane, float restitution) :
    Collision(restitution)
{
    m_particle = particle;
    m_plane = plane;
}

ParticlePlaneCollision::~ParticlePlaneCollision()
{}

void ParticlePlaneCollision::do_solveCollision()
{
    //Don't process fixed particles (ground plane is assumed fixed)
    if (m_particle->isFixed())
        return;

    //TODO: Solve ParticlePlane collisions,
    // update particle position and velocity after collision
    //
    //Functions to use:
    //glm::dot(v1, v2): Return the dot product of two vector.
    //Plane::distanceToOrigin(): Return the distance to origin from the plane
    //Plane::normal(): Return the normal of the plane
    //Particle::getRadius(), Particle::getPosition(), Particle::getVelocity(), Particle::setPosition(), Particle::setVelocity()
    float r = m_particle->getRadius();
    glm::vec3 pos2 = m_plane->projectOnPlane(m_particle->getPosition());
    //Compute particle-plane vector

    float particlePlaneDist = glm::distance(m_particle->getPosition(),pos2);
    float interpenetrationDist = m_particle->getRadius() - particlePlaneDist;
    glm::vec3 k = m_plane->normal();

     m_particle->setPosition(m_particle->getPosition() +2*interpenetrationDist*k);  

    //Compute post-collision velocity
    glm::vec3 prev_v1 = m_particle->getVelocity();
    float proj_v = (1.0f + m_restitution)
      * glm::dot(k, prev_v1)
      / (1.0 / m_particle->getMass());//masse plan est infinie
    glm::vec3 new_v1 = prev_v1 - proj_v/m_particle->getMass()*k;
    m_particle->setVelocity(new_v1);
    
    /*    glm::vec3 final_v1 = m_particle->getVelocity()*0.00001f;
	  m_particle->setVelocity(final_v1);*/
    
}



bool testParticlePlane(const ParticlePtr &particle, const PlanePtr &plane)
{
    /* Equation of a plane passing through A and normal to n:
   * dot( p - A, n ) = dot( p, n ) - dot( A, n ) = 0
   * dot( A, n ) is stored in the "distanceToOrigin" of the plane.
   *
   * Equation of a particle of radius r centered in c:
   * dot( p - c, p - c ) = r²
   *
   * distance( plane, particle )
   *   = min( distance( plane, c ) - r, 0 )              //definition
   *   = min( abs( dot( c - A, n ) ) - r, 0 )
   *   = min( abs( dot( c, n ) - dot( A, n ) ) - r, 0 )
   *
   * So, there is intersection if distance( plane, particle ) = 0
   * <=> abs( dot( c, n ) - dot( A, n ) ) - r <= 0
   * <=> abs( dot( c, n ) - dot( A, n ) ) <= r
   */

    //TODO: Test collision between particle and plane
    //Functions to use:
    //glm::dot(v1, v2): Return the dot product of two vector.
    //Plane::distanceToOrigin(): Return the distance to origin from the plane
    //Plane::normal(): Return the normal of the plane
    //Particle::getRadius(), Particle::getPosition()

  float r = particle->getRadius();
  glm::vec3 pos2 = plane->projectOnPlane(particle->getPosition());
  float particlePlaneDist = glm::distance(particle->getPosition(),pos2)-r;
  return (particlePlaneDist < 0.0f) ? true : false;
}
