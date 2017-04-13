#ifndef BODY_CYLINDER_RENDERABLE_HPP
#define BODY_CYLINDER_RENDERABLE_HPP

#include "../HierarchicalRenderable.hpp"

#include <vector>
#include <glm/glm.hpp>

/**@brief Render a particle to the screen.
 *
 * Render a particle to the screen. Since a particle is modeled by
 * a ball, this renderable simply render the corresponding ball. If
 * you have more than one renderable, have a look to ParticleListRenderable.
 */
class BodyCylinderRenderable : public HierarchicalRenderable
{
    public:
        /**@brief Build a particle renderable.
         *
         * Build a renderable to render a particle.
         * @param program The shader program used to render the particle.
         * @param particle The particle to render.
         */
        BodyCylinderRenderable(ShaderProgramPtr program, glm::vec3 color);

        ~BodyCylinderRenderable();

    protected:
        virtual void do_draw();
        void do_animate(float time);

        size_t m_numberOfVertices;
        std::vector<glm::vec3> m_positions;
        std::vector<glm::vec4> m_colors;
        std::vector<glm::vec3> m_normals;

        unsigned int m_pBuffer;
        unsigned int m_cBuffer;
        unsigned int m_nBuffer;
};

typedef std::shared_ptr<BodyCylinderRenderable> BodyCylinderRenderablePtr;

#endif
