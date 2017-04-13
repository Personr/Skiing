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
    
    glm::vec3 velocityDir = glm::normalize(getVelocity());
    glm::vec3 posDif = glm::vec3(8*velocityDir[0], 8*velocityDir[1], 8*velocityDir[2]-5);
    glm::vec3 pointDif = glm::vec3(8*velocityDir[0], 8*velocityDir[1], 8*velocityDir[2]);
    //m_camera->setPosition(pos - newDir);
    m_camera->setViewMatrix(
        glm::lookAt(pos - posDif, pos + pointDif, glm::vec3(0,0,1)) );
}