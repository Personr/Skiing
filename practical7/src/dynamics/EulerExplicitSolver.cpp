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
            //Particle::getPosition(), Particle::getVelocity(), Particle::getMass(), Particle::getForce()
            //Particle::setPosition(), Particle::setVelocity()
            glm::vec3 position = p->getPosition();
            glm::vec3 velocity = p->getVelocity();
            float mass = p->getMass();
            glm::vec3 force = p->getForce();
            glm::vec3 newVelocity = (velocity + 1 / mass * dt * force);
            p->setVelocity(newVelocity);
            p->setPosition(position + dt * newVelocity);
        }
    }
}
