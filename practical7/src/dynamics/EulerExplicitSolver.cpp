#include "./../../include/dynamics/EulerExplicitSolver.hpp"

EulerExplicitSolver::EulerExplicitSolver()
{

}

EulerExplicitSolver::~EulerExplicitSolver()
{

}

void EulerExplicitSolver::do_solve(const float& dt, std::vector<ParticlePtr>& particles)
{
    for (ParticlePtr p : particles) {
        if (!p->isFixed()) {
            //TODO: Implement explicit euler solver
            //Functions to use:
	  glm::vec3 position = p->getPosition();
	  glm::vec3 vitesse = p->getVelocity();
	  glm::vec3 force = p->getForce();
	  float poids = p->getMass();

	  //, Particle::getMass(), Particle::getForce()
          p->setPosition(position+vitesse*dt);
	  p->setVelocity(vitesse+(1/poids)*force*dt);
        }
    }
}
