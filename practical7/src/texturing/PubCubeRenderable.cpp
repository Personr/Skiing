/* 
 * File:   PubCube.cpp
 * Author: personr
 * 
 * Created on April 13, 2017, 1:56 PM
 */

#include "../../include/texturing/PubCubeRenderable.hpp"

PubCubeRenderable::PubCubeRenderable(ShaderProgramPtr shaderProgram,
        const std::string &textureFilename1,
        const std::string& textureFilename2) : MultiTexturedCubeRenderable(shaderProgram,
textureFilename1, textureFilename2) {
}

PubCubeRenderable::~PubCubeRenderable() {
}

void PubCubeRenderable::do_animate(float time) {
    float sinTime = std::sin(time);
    if (sinTime > 0.9) {
        m_blendingCoefficient = 1;
    } else if (sinTime < -0.9) {
        m_blendingCoefficient = -1;
    }
}

