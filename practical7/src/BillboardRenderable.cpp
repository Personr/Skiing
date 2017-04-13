/* 
 * File:   Billboard.cpp
 * Author: personr
 * 
 * Created on April 13, 2017, 2:12 PM
 */

#include "../include/BillboardRenderable.hpp"
#include "../include/texturing/TexturedCubeRenderable.hpp"
#include "./../include/keyframes/GeometricTransformation.hpp"
#include "./../include/lighting/SpotLightRenderable.hpp"
#include "./../include/gl_helper.hpp"
#include "./../include/log.hpp"
#include "./../include/Utils.hpp"

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GL/glew.h>
#include <iostream>

BillboardRenderable::BillboardRenderable(ShaderProgramPtr multiTexShader, ShaderProgramPtr texShader,
        const std::string &textureFilename1,
        const std::string& textureFilename2, float x, float y, float z, Viewer *viewer) : PubCubeRenderable(
        multiTexShader, textureFilename1, textureFilename2) 
{
    float width = 1/15.0;
    glm::mat4 parentTransformation(1.0), localTransformation(1.0);
    parentTransformation = glm::translate( glm::mat4(1.0), glm::vec3(x,y,z+2));
    parentTransformation = glm::scale( parentTransformation, glm::vec3(3,width,2));
    //this->setParentTransform(parentTransformation); 
    this->setLocalTransform(parentTransformation);  
    
    addBoard(std::shared_ptr<PubCubeRenderable>(this), texShader, x-1.5F-(1.5*width), y, z+2.0F, 3*width, 3*width, 2.0F);
    addBoard(std::shared_ptr<PubCubeRenderable>(this), texShader, x+1.5F+(1.5*width), y, z+2.0F, 3*width, 3*width, 2.0F);
    addBoard(std::shared_ptr<PubCubeRenderable>(this), texShader, x, y, z+3.0F+width, 3.0F+(6*width), 3*width, 3*width);
    addBoard(std::shared_ptr<PubCubeRenderable>(this), texShader, x, y, z+1.0F-width, 3.0F+(6*width), 3*width, 3*width);
    
    addBoard(std::shared_ptr<PubCubeRenderable>(this), texShader, x+1.0F, y, z+0.25F, 3*width, 3*width, 1.5F);
    addBoard(std::shared_ptr<PubCubeRenderable>(this), texShader, x-1.0F, y, z+0.25F, 3*width, 3*width, 1.5F);
    
    
    
    
}

BillboardRenderable::~BillboardRenderable() {
}

void BillboardRenderable::addBoard(std::shared_ptr<PubCubeRenderable>parent, 
        ShaderProgramPtr texShader, float transX, float transY, float transZ, float scaleX, 
        float scaleY, float scaleZ) {
    //Textured cube
    std::string filename = "../textures/crate.jpg";
    TexturedCubeRenderablePtr texCube = std::make_shared<TexturedCubeRenderable>(texShader, filename);
    glm::mat4 parentTransformation(1.0);
    parentTransformation = glm::translate( glm::mat4(1.0), glm::vec3(transX,transY,transZ));
    parentTransformation = glm::scale( parentTransformation, glm::vec3(scaleX,scaleY,scaleZ));
    texCube->setParentTransform(parentTransformation);
    texCube->setMaterial(Material::Pearl());
    
    HierarchicalRenderable::addChild(parent, texCube);
}

