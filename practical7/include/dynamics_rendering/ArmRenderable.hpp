#ifndef ARM_RENDERABLE_HPP
#define ARM_RENDERABLE_HPP

#include "./../../include/dynamics_rendering/BodyCylinderRenderable.hpp"
#include "../dynamics/Particle.hpp"

#include <vector>
#include <glm/glm.hpp>

/**@brief Render a particle to the screen.
 *
 * Render a particle to the screen. Since a particle is modeled by
 * a ball, this renderable simply render the corresponding ball. If
 * you have more than one renderable, have a look to ParticleListRenderable.
 */
class ArmRenderable : public BodyCylinderRenderable
{
    public:
        /**@brief Build a particle renderable.
         *
         * Build a renderable to render a particle.
         * @param program The shader program used to render the particle.
         * @param particle The particle to render.
         */
        ArmRenderable(ShaderProgramPtr program, ParticlePtr particle, glm::vec3 color, bool isLeft);
        ~ArmRenderable();

    private:
        virtual void do_draw();
        ParticlePtr m_particle;
        bool m_isLeft;
};

typedef std::shared_ptr<ArmRenderable> ArmRenderablePtr;

#endif
