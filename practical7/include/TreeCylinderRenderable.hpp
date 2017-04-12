#ifndef TREE_CYLINDER_RENDERABLE_HPP
#define TREE_CYLINDER_RENDERABLE_HPP

#include "./keyframes/KeyframedCylinderRenderable.hpp"
#include "./keyframes/KeyframeCollection.hpp"

#include <glm/glm.hpp>

class TreeCylinderRenderable : public KeyframedCylinderRenderable
{
public:
    TreeCylinderRenderable(ShaderProgramPtr program,
    bool normalPerVertex = false, unsigned int nbSlices = 20, int spinePFace = 11, const MaterialPtr& material = Material::Emerald());
    ~TreeCylinderRenderable();

protected:
    void do_draw();
    

private:
    void buildNormalPerFace(unsigned int nbSlices);
    
    std::vector< glm::vec3 > m_positions;
    std::vector< glm::vec4 > m_colors;
    std::vector< glm::vec3 > m_normals;

    unsigned int m_pBuffer;
    unsigned int m_cBuffer;
    unsigned int m_nBuffer;
    
    glm::vec4 color;
    int m_spinePFace;
    int m_nbSlices;
};

typedef std::shared_ptr<TreeCylinderRenderable> TreeCylinderRenderablePtr;

#endif

