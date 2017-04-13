/* 
 * File:   PubCube.h
 * Author: personr
 *
 * Created on April 13, 2017, 1:56 PM
 */

#ifndef PUBCUBE_RENDERABLE_H
#define	PUBCUBE_RENDERABLE_H

#include "./MultiTexturedCubeRenderable.hpp"

class PubCubeRenderable : public MultiTexturedCubeRenderable {
public:
    ~PubCubeRenderable();
    PubCubeRenderable(ShaderProgramPtr shaderProgram,
        const std::string &textureFilename1,
        const std::string& textureFilename2);
    
private:
    virtual void do_animate(float time);
};

typedef std::shared_ptr<PubCubeRenderable> PubCubeRenderablePtr;

#endif	/* PUBCUBE_H */

