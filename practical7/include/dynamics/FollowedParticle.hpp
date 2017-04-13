/* 
 * File:   FollowedParticle.h
 * Author: personr
 *
 * Created on April 12, 2017, 7:48 PM
 */

#ifndef FOLLOWEDPARTICLE_H
#define	FOLLOWEDPARTICLE_H

#include "../Camera.hpp"
#include "./Particle.hpp"
#include "../Viewer.hpp"

class FollowedParticle : public Particle {
public:
    FollowedParticle(const glm::vec3& position, const glm::vec3& velocity,
            const float& mass, const float& radius, Camera *camera);
    virtual ~FollowedParticle();
    void setPosition(const glm::vec3 &pos);
private:
    Camera *m_camera;

};

typedef std::shared_ptr<FollowedParticle> FollowedParticlePtr;

#endif	/* FOLLOWEDPARTICLE_H */

