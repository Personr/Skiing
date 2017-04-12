/* 
 * File:   TreeRenderable.cpp
 * Author: personr
 * 
 * Created on April 4, 2017, 7:23 PM
 */

#include "./../include/TreeRenderable.hpp"
#include "./../include/TreeCylinderRenderable.hpp"
#include "./../include/gl_helper.hpp"
#include "./../include/log.hpp"
#include "./../include/Utils.hpp"

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GL/glew.h>
#include <iostream>

#include <cstdlib>
#include <ctime>

TreeRenderable::TreeRenderable(ShaderProgramPtr shader, float height)
    : TreeCylinderRenderable(shader, 20, false, 11, Material::Pearl()) 
{
    m_height = height;
    //Tree modeling:
    //The modeling is hierarchical (setLocalTransform, setParentTransform)
    //The animation is hierarchical too (addParentTransformKeyframe, addLocalTransformKeyframe)

    //Main branch
    this->setLocalTransform( GeometricTransformation( glm::vec3{}, glm::quat(), glm::vec3{height/80,height/80,height}).toMatrix() );

    srand(time(NULL));
    
    addCylinder(std::shared_ptr<TreeCylinderRenderable>(this), height/80, height/80, height, 0, shader);
}

TreeRenderable::~TreeRenderable() {
}

float TreeRenderable::frand_a_b(float a, float b)
{
    return ( rand()/(float)RAND_MAX ) * (b-a) + a;
}

float TreeRenderable::randSign()
{
    float r =  rand()/(float)RAND_MAX;
    if (r > 0.5) {
        return 1.0;
    } else {
        return -1.0;
    }
}

void TreeRenderable::addTransformations(std::shared_ptr<TreeCylinderRenderable>& cylinder, float z, float angleY, float angleZ, float initAngle)
{   
    float maxTime = frand_a_b(5.0,10.0);
    float t = 0.0;
    
    cylinder->addParentTransformKeyframe(t, GeometricTransformation( glm::vec3{0,0,z}, glm::quat( glm::vec3(0, initAngle, angleZ))) );
    t += frand_a_b(1.0,2.0);
    while (t < 5.0) {      
        cylinder->addParentTransformKeyframe(t, GeometricTransformation( glm::vec3{0,0,z}, glm::quat( glm::vec3(0, angleY + frand_a_b(-0.05,0.05), angleZ))) );      
        t += frand_a_b(1.0,2.0);
    }
    
    cylinder->addParentTransformKeyframe(6.0, GeometricTransformation( glm::vec3{0,0,z}, glm::quat( glm::vec3(0, initAngle, angleZ))) );
}

void TreeRenderable::addCylinder(std::shared_ptr<TreeCylinderRenderable> parent, float scaleX, float scaleY, float scaleZ, int nb, const ShaderProgramPtr& shader)
{
    if (nb <= 2) {
        int childrenNb;
        if (nb == 0) {
            childrenNb = 150;
        } else {
            childrenNb = 10*scaleZ / (m_height / 2);
        }
        for (int i=0; i<childrenNb; i++) {
            auto child = std::make_shared<TreeCylinderRenderable>(shader, false, 20, 11, Material::Leaf());

            float childScaleX = frand_a_b(0.3, 0.5) * scaleX;
            float childScaleY = frand_a_b(0.3, 0.5) * scaleY;
            float childScaleZ = frand_a_b(0.2, 0.5) * scaleZ;
            
            float angleZ = frand_a_b(0,2*float(M_PI));
            float angleY;
            float z;
            if (nb == 0) {
                angleY = float(M_PI_2);
                z = frand_a_b(scaleZ/10,99*scaleZ/100); 
                childScaleZ *= 1 - z/scaleZ;
                childScaleZ += 0.2;
                childScaleX *= 0.2 + (1 - z/scaleZ);
                childScaleY *= 0.2 + (1 - z/scaleZ);
            } else {
                angleY = frand_a_b(0,float(M_PI_4)/2);
                z = frand_a_b(scaleZ/10,3*scaleZ/4);
            }
            
            float initAngle = angleY + frand_a_b(-0.05,0.05);
            if (nb == 0) {
                child->setLocalTransform(GeometricTransformation( glm::vec3{}, glm::quat(), glm::vec3{childScaleX,childScaleY,childScaleZ}).toMatrix() );
                addTransformations(child, z, angleY, angleZ, initAngle);
            } else {           
                child->setLocalTransform(GeometricTransformation( glm::vec3{0,0,z}, glm::quat(glm::vec3(0, initAngle, angleZ)), glm::vec3{childScaleX,childScaleY,childScaleZ}).toMatrix() );
            }
                                              
            HierarchicalRenderable::addChild(parent, child);
                     
            addCylinder(child, childScaleX, childScaleY, childScaleZ, nb + 1, shader);
        }
    }
}