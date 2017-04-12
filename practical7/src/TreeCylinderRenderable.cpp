/* 
 * File:   TreeCylinder.cpp
 * Author: personr
 * 
 * Created on April 4, 2017, 4:14 PM
 */

#include "./../include/TreeCylinderRenderable.hpp"
#include "./../include/lighting/LightedCylinderRenderable.hpp"
#include "./../include/gl_helper.hpp"
#include "./../include/log.hpp"
#include "./../include/Utils.hpp"

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GL/glew.h>
#include <iostream>

#include <cstdlib>
#include <ctime>

TreeCylinderRenderable::TreeCylinderRenderable(ShaderProgramPtr prog,
    bool normalPerVertex, unsigned int nbSlices, int spinePFace, const MaterialPtr& material):KeyframedCylinderRenderable(
        prog, material),
    m_pBuffer(0), m_cBuffer(0), m_nBuffer(0)
{
    color = glm::vec4( 0.0, 1.0, 0.0, 1.0);
    m_spinePFace = spinePFace;
    m_nbSlices = nbSlices;
    
    buildNormalPerFace(nbSlices);

    //Generate buffers
    glcheck(glGenBuffers(1, &m_pBuffer));
    glcheck(glGenBuffers(1, &m_cBuffer));
    glcheck(glGenBuffers(1, &m_nBuffer));

    //Activate buffer and send data to the graphics card
    glcheck(glBindBuffer(GL_ARRAY_BUFFER, m_pBuffer));
    glcheck(glBufferData(GL_ARRAY_BUFFER, m_positions.size()*sizeof(glm::vec3), m_positions.data(), GL_STATIC_DRAW));
    glcheck(glBindBuffer(GL_ARRAY_BUFFER, m_cBuffer));
    glcheck(glBufferData(GL_ARRAY_BUFFER, m_colors.size()*sizeof(glm::vec4), m_colors.data(), GL_STATIC_DRAW));
    glcheck(glBindBuffer(GL_ARRAY_BUFFER, m_nBuffer));
    glcheck(glBufferData(GL_ARRAY_BUFFER, m_normals.size()*sizeof(glm::vec3), m_normals.data(), GL_STATIC_DRAW));
}

TreeCylinderRenderable::~TreeCylinderRenderable() 
{
    glcheck(glDeleteBuffers(1, &m_pBuffer));
    glcheck(glDeleteBuffers(1, &m_cBuffer));
    glcheck(glDeleteBuffers(1, &m_nBuffer));
}

void TreeCylinderRenderable::do_draw()
{
    LightedCylinderRenderable::do_draw();
    //Location
    int modelLocation = m_shaderProgram->getUniformLocation("modelMat");
    int positionLocation = m_shaderProgram->getAttributeLocation("vPosition");
    int colorLocation = m_shaderProgram->getAttributeLocation("vColor");
    int normalLocation = m_shaderProgram->getAttributeLocation("vNormal");

    //Send data to GPU
    if (modelLocation != ShaderProgram::null_location) {
        glcheck(glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(getModelMatrix())));
    }

    if (positionLocation != ShaderProgram::null_location) {
        //Activate location
        glcheck(glEnableVertexAttribArray(positionLocation));
        //Bind buffer
        glcheck(glBindBuffer(GL_ARRAY_BUFFER, m_pBuffer));
        //Specify internal format
        glcheck(glVertexAttribPointer(positionLocation, 3, GL_FLOAT, GL_FALSE, 0, (void*)0));
    }
    if (colorLocation != ShaderProgram::null_location) {
        glcheck(glEnableVertexAttribArray(colorLocation));
        glcheck(glBindBuffer(GL_ARRAY_BUFFER, m_cBuffer));
        glcheck(glVertexAttribPointer(colorLocation, 4, GL_FLOAT, GL_FALSE, 0, (void*)0));
    }
    if (normalLocation != ShaderProgram::null_location) {
        glcheck(glEnableVertexAttribArray(normalLocation));
        glcheck(glBindBuffer(GL_ARRAY_BUFFER, m_nBuffer));
        glcheck(glVertexAttribPointer(normalLocation, 3, GL_FLOAT, GL_FALSE, 0, (void*)0));
    }

    
    glcheck(glDrawArrays(GL_TRIANGLES,0, m_positions.size()- 2*m_nbSlices*m_spinePFace)); 
    glcheck(glDrawArrays(GL_LINES, 0, m_positions.size()));

    if (positionLocation != ShaderProgram::null_location) {
        glcheck(glDisableVertexAttribArray(positionLocation));
    }
    if (colorLocation != ShaderProgram::null_location) {
        glcheck(glDisableVertexAttribArray(colorLocation));
    }
    if (normalLocation != ShaderProgram::null_location) {
        glcheck(glDisableVertexAttribArray(normalLocation));
    }
}

void TreeCylinderRenderable::buildNormalPerFace(unsigned int nbSlices)
{
    // basic cylinder: radius 1, along z axis, bases on z == 0 and z == 1
    // The normal on a given coord around z is the coord itself!
    //
    // NORMAL PER FACE: in this case, each slice of the cylinder has its
    // own normal (1 for the 2 triangles of a slice)

    glm::vec3 bottomCenter(0.0, 0.0, 0.0);
    glm::vec3 topCenter(0.0, 0.0, 1.0);

    double dTheta = 2.0 * M_PI / (double) nbSlices;
    double currentTheta = 0.0;
    double currentCos = 1.0;
    double currentSin = 0.0;

    for (unsigned int i = 0; i < nbSlices; ++i) {
        double nextCos = cos(currentTheta + dTheta);
        double nextSin = sin(currentTheta + dTheta);
        // normal: computed in the middle of the slice
        glm::vec3 normal(cos(currentTheta + dTheta / 2.0),
                         sin(currentTheta + dTheta / 2.0), 0.0);
        // let's color this cube with normal values
        // (since we do not have illumination yet, it will create a shaded effect)

        // first triangle along z
        m_positions.push_back(glm::vec3(currentCos, currentSin, 0.0));
        m_positions.push_back(glm::vec3(nextCos, nextSin, 1.0));
        m_positions.push_back(glm::vec3(nextCos, nextSin, 0.0));
        for (unsigned int i = 0; i < 3; i++) {
            m_colors.push_back(color);
            m_normals.push_back(normal);
        }

        // second triangle along z
        m_positions.push_back(glm::vec3(currentCos, currentSin, 0.0));
        m_positions.push_back(glm::vec3(nextCos, nextSin, 1.0));
        m_positions.push_back(glm::vec3(currentCos, currentSin, 1.0));
        for (unsigned int i = 0; i < 3; i++) {
            m_colors.push_back(color);
            m_normals.push_back(normal);
        }

        // bottom and top bases
        m_positions.push_back(bottomCenter);
        m_positions.push_back(glm::vec3(currentCos, currentSin, 0.0));
        m_positions.push_back(glm::vec3(nextCos, nextSin, 0.0));

        // glm::vec4 color((i%2==0) ? 0.0 : 1.0, 0.0, (i%2==0) ? 1.0 : 0.0, 1.0);
        for (unsigned int i = 0; i < 3; i++) {
            m_normals.push_back(glm::vec3(0.0, 0.0, -1.0));
            m_colors.push_back(color);
        }

        m_positions.push_back(topCenter);
        m_positions.push_back(glm::vec3(currentCos, currentSin, 1.0));
        m_positions.push_back(glm::vec3(nextCos, nextSin, 1.0));
        for (unsigned int i = 0; i < 3; i++) {
            m_normals.push_back(glm::vec3(0.0, 0.0, +1.0));
            m_colors.push_back(color);
        }

        currentTheta = currentTheta + dTheta;
        currentCos = nextCos;
        currentSin = nextSin;
    }
    
    currentTheta = 0.0;
    currentCos = 1.0;
    currentSin = 0.0;
    
    for (unsigned int i = 0; i < nbSlices; ++i) {
        double nextCos = cos(currentTheta + dTheta);
        double nextSin = sin(currentTheta + dTheta);
        // normal: computed in the middle of the slice
        glm::vec3 normal(cos(currentTheta + dTheta / 2.0),
                         sin(currentTheta + dTheta / 2.0), 0.0);
   
        for (unsigned int i = 0; i < m_spinePFace; i++) {
            float z = rand()/(float)RAND_MAX;
            m_positions.push_back(glm::vec3(currentCos, currentSin, z));
            m_positions.push_back(glm::vec3(4*currentCos, 4*currentSin, z));
            m_colors.push_back(color);
            m_colors.push_back(color);
        }

        currentTheta = currentTheta + dTheta;
        currentCos = nextCos;
        currentSin = nextSin;
    }
}

