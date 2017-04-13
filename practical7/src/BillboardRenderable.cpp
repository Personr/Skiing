/* 
 * File:   Billboard.cpp
 * Author: personr
 * 
 * Created on April 13, 2017, 2:12 PM
 */

#include "../include/BillboardRenderable.hpp"
#include "../include/texturing/TexturedCubeRenderable.hpp"
#include "./../include/keyframes/GeometricTransformation.hpp"
#include "./../include/gl_helper.hpp"
#include "./../include/log.hpp"
#include "./../include/Utils.hpp"

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GL/glew.h>
#include <iostream>

BillboardRenderable::BillboardRenderable(ShaderProgramPtr multiTexShader, ShaderProgramPtr texShader,
        const std::string &textureFilename1,
        const std::string& textureFilename2) : PubCubeRenderable(
        multiTexShader, textureFilename1, textureFilename2) 
{
    float width = 1/15.0;
    glm::mat4 parentTransformation(1.0);
    parentTransformation = glm::translate( glm::mat4(1.0), glm::vec3(0,0,2));
    parentTransformation = glm::scale( parentTransformation, glm::vec3(3,width,2));
    this->setLocalTransform(parentTransformation);       
    
    addBoard(std::shared_ptr<PubCubeRenderable>(this), texShader, -1.5F-(1.5*width), 2.0F, 3*width, 3*width, 2.0F);
    addBoard(std::shared_ptr<PubCubeRenderable>(this), texShader, 1.5F+(1.5*width), 2.0F, 3*width, 3*width, 2.0F);
    addBoard(std::shared_ptr<PubCubeRenderable>(this), texShader, 0.0F, 3.0F+width, 3.0F+(6*width), 3*width, 3*width);
    addBoard(std::shared_ptr<PubCubeRenderable>(this), texShader, 0.0F, 1.0F-width, 3.0F+(6*width), 3*width, 3*width);
    
    addBoard(std::shared_ptr<PubCubeRenderable>(this), texShader, 1.0F, 0.25F, 3*width, 3*width, 1.5F);
    addBoard(std::shared_ptr<PubCubeRenderable>(this), texShader, -1.0F, 0.25F, 3*width, 3*width, 1.5F);
}

BillboardRenderable::~BillboardRenderable() {
}

void BillboardRenderable::addBoard(std::shared_ptr<PubCubeRenderable>parent, 
        ShaderProgramPtr texShader, float transX, float transZ, float scaleX, 
        float scaleY, float scaleZ) {
    //Textured cube
    std::string filename = "../textures/crate.jpg";
    TexturedCubeRenderablePtr texCube = std::make_shared<TexturedCubeRenderable>(texShader, filename);
    glm::mat4 parentTransformation(1.0);
    parentTransformation = glm::translate( glm::mat4(1.0), glm::vec3(transX,0,transZ));
    parentTransformation = glm::scale( parentTransformation, glm::vec3(scaleX,scaleY,scaleZ));
    texCube->setParentTransform(parentTransformation);
    texCube->setMaterial(Material::Pearl());
    
    HierarchicalRenderable::addChild(parent, texCube);
}

