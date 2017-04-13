/* 
 * File:   FollowedParticle.cpp
 * Author: personr
 * 
 * Created on April 12, 2017, 7:48 PM
 */

#include "../../include/dynamics/FollowedParticle.hpp"

FollowedParticle::FollowedParticle(const glm::vec3& position, const glm::vec3& velocity,
            const float& mass, const float& radius, Camera *camera) : Particle(position, velocity,
            mass, radius) {
    m_camera = camera;
    std::cout << m_camera; 
}

FollowedParticle::~FollowedParticle() {
}

void FollowedParticle::setPosition(const glm::vec3 &pos)
{
    Particle::setPosition(pos);
    m_camera->setPosition(pos + glm::vec3(1,1,1));
    glm::vec3 velocityDir = glm::normalize(getVelocity());
    glm::vec3 newDir = glm::vec3(2*velocityDir[0], 2*velocityDir[1], 2*velocityDir[2]);
    m_camera->setViewMatrix(
        glm::lookAt(pos - newDir, pos, glm::vec3(0,0,1)) );
}