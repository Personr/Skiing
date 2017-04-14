#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/norm.hpp>

#include "./../../include/gl_helper.hpp"
#include "./../../include/dynamics/DynamicSystem.hpp"
#include "./../../include/dynamics/ParticlePlaneCollision.hpp"
#include "./../../include/dynamics/ParticleParticleCollision.hpp"


DynamicSystem::DynamicSystem() :
    m_dt(0.1), m_restitution(1.0), m_handleCollisions(true)
{
}

DynamicSystem::~DynamicSystem()
{
}

const std::vector<ParticlePtr>& DynamicSystem::getParticles() const
{
    return m_particles;
}

void DynamicSystem::setParticles(const std::vector<ParticlePtr> &particles)
{
    m_particles = particles;
}

const std::vector<ForceFieldPtr>& DynamicSystem::getForceFields() const
{
    return m_forceFields;
}

void DynamicSystem::setForceFields(const std::vector<ForceFieldPtr> &forceFields)
{
    m_forceFields = forceFields;
}


float DynamicSystem::getDt() const
{
    return m_dt;
}

void DynamicSystem::setDt(float dt)
{
    m_dt = dt;
}

void DynamicSystem::clear()
{
    m_particles.clear();
    m_forceFields.clear();
    m_planeObstacles.clear();
}

bool DynamicSystem::getCollisionDetection()
{
    return m_handleCollisions;
}

void DynamicSystem::setCollisionsDetection(bool onOff)
{
    m_handleCollisions = onOff;
}

void DynamicSystem::addParticle(ParticlePtr p)
{
    m_particles.push_back(p);
}

void DynamicSystem::addForceField(ForceFieldPtr forceField)
{
    m_forceFields.push_back(forceField);
}

void DynamicSystem::addPlaneObstacle(PlanePtr planeObstacle)
{
    m_planeObstacles.push_back(planeObstacle);
}

SolverPtr DynamicSystem::getSolver()
{
    return m_solver;
}

void DynamicSystem::setSolver(SolverPtr solver)
{
    m_solver = solver;
}

void DynamicSystem::detectCollisions()
{
 glm::vec3 px,pv;//On remet le flocon en haut quand il touche le sol debut decla
  float pm, pr;
  //  srand (static_cast <unsigned> (time(0)));
  float randx = 1.0f;
  float randy = 1.0f;
  float randz = 0.0f;
  float randr = 0.0f;
  float randv1 = 0.0f;
  float randv2 = 0.0f;
  float randv3 = 0.0f;
  //Detect particle plane collisions
    for (ParticlePtr p : m_particles) {
      for (PlanePtr o : m_planeObstacles) {
	if (testParticlePlane(p, o)) {
	  if (p->getMass() > 0.2){ // cas du skieur, on fait les collisions

	    ParticlePlaneCollisionPtr c =
	      std::make_shared<ParticlePlaneCollision>(p,o,m_restitution);
	    m_collisions.push_back(c);
	  }

	      else{ // On gère plus les collisions -> flocons
		randx = 2*(rand() / (RAND_MAX + 1.)) -1;
		randy = 2*(rand() / (RAND_MAX + 1.)) -1;
		randz = 2*(rand() / (RAND_MAX + 1.)) -1;
		randv1 = (rand() / (RAND_MAX + 1.));
		randv2 = (rand() / (RAND_MAX + 1.));
		randv3 = (rand() / (RAND_MAX + 1.));

		randr = (rand() / (RAND_MAX + 1.));

		//	px = glm::vec3(5,5, 5);   bail canon à neige
		pv = glm::vec3(0,0,-randv1);
		px = glm::vec3(60*randx-30,200*randy-100, 10*randz+5);
		pr = 0.1*randr + 0.1;
		pm = randz+1; //Fin remise en haut

		p->setPosition(px);
		p->setVelocity(pv);
		p->setRadius(pr);

		}

            }
        }
    }

    //Detect particle particle collisions
    for (size_t i = 0; i < m_particles.size(); ++i) {
        for (size_t j = i; j < m_particles.size(); ++j) {
            ParticlePtr p1 = m_particles[i];
            ParticlePtr p2 = m_particles[j];
            if (testParticleParticle(p1,p2)) {
                ParticleParticleCollisionPtr c =
                    std::make_shared<ParticleParticleCollision>(p1,p2,m_restitution);
                m_collisions.push_back(c);
            }
        }
    }
}

void DynamicSystem::solveCollisions()
{
    while (!m_collisions.empty()) {
        CollisionPtr collision = m_collisions.back();
        collision->solveCollision();
        m_collisions.pop_back();
    }
}

void DynamicSystem::computeSimulationStep()
{
    //Reset the force for each particle
    for (ParticlePtr p : m_particles) {
        p->setForce(glm::vec3(0.0, 0.0, 0.0));
    }
    //Compute forces
    for (ForceFieldPtr f : m_forceFields) {
        f->addForce();
    }

    //Integrate position and velocity of particles
    m_solver->solve(m_dt, m_particles);

    //Detect and resolve collisions
    if (m_handleCollisions) {
        detectCollisions();
        solveCollisions();
    }
}

const float DynamicSystem::getRestitution()
{
    return m_restitution;
}

void DynamicSystem::setRestitution(const float& restitution)
{
    m_restitution = std::max(0.0f,std::min(restitution,1.0f));
}

std::ostream& operator<<(std::ostream& os, const DynamicSystemPtr& system)
{
    std::vector<ParticlePtr> particles = system->getParticles();
    os << "Particle number: " << particles.size() << std::endl;
    for (ParticlePtr p : particles)
        os << p << std::endl;
    return os;
}
