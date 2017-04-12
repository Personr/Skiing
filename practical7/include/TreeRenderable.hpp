/* 
 * File:   TreeRenderable.h
 * Author: personr
 *
 * Created on April 4, 2017, 7:23 PM
 */

#ifndef TREERENDERABLE_H
#define	TREERENDERABLE_H

#include "./TreeCylinderRenderable.hpp"
#include <glm/glm.hpp>

class TreeRenderable : public TreeCylinderRenderable
{
public:
    TreeRenderable(ShaderProgramPtr program, float height);
    virtual ~TreeRenderable();
    
protected:
    
private:
    float frand_a_b(float a, float b);
    float randSign();
    void addTransformations(std::shared_ptr<TreeCylinderRenderable>& cylinder, float z, float angleY, float angleZ, float initAngle);
    void addCylinder(std::shared_ptr<TreeCylinderRenderable> parent, float scaleX, float scaleY, float scaleZ, int nb, const ShaderProgramPtr& shader);

    float m_height;
};

typedef std::shared_ptr<TreeRenderable> TreeRenderablePtr;

#endif	/* TREERENDERABLE_H */

