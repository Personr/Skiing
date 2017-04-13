/* 
 * File:   Billboard.h
 * Author: personr
 *
 * Created on April 13, 2017, 2:12 PM
 */

#ifndef BILLBOARD_RENDERABLE_H
#define	BILLBOARD_RENDERABLE_H

#include "texturing/PubCubeRenderable.hpp"

class BillboardRenderable : public PubCubeRenderable {
public:
    BillboardRenderable(ShaderProgramPtr multiTexShader, ShaderProgramPtr texShader,
        const std::string &textureFilename1,
        const std::string& textureFilename2);
    virtual ~BillboardRenderable();
private:
    void addBoard(std::shared_ptr<PubCubeRenderable>parent, 
        ShaderProgramPtr texShader, float transX, float transZ, float scaleX, 
        float scaleY, float scaleZ);

};

typedef std::shared_ptr<BillboardRenderable> BillboardRenderablePtr;

#endif	/* BILLBOARD_H */

