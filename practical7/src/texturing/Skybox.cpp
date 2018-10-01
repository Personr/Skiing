#include "../../include/texturing/Skybox.hpp"
#include "../../include/gl_helper.hpp"
#include "../../teachers/Geometries.hpp"

#include <glm/gtc/type_ptr.hpp>
#include <GL/glew.h>
#include <SFML/Graphics/Image.hpp>
#include <iostream>

//#define GL_TEXTURE_CUBE_MAP_ARB             0x8513
//#define GL_TEXTURE_BINDING_CUBE_MAP         0x8514
//#define GL_TEXTURE_CUBE_MAP_POSITIVE_X      0x8515
//#define GL_TEXTURE_CUBE_MAP_NEGATIVE_X      0x8516
//#define GL_TEXTURE_CUBE_MAP_POSITIVE_Y      0x8517
//#define GL_TEXTURE_CUBE_MAP_NEGATIVE_Y      0x8518
//#define GL_TEXTURE_CUBE_MAP_POSITIVE_Z      0x8519
//#define GL_TEXTURE_CUBE_MAP_NEGATIVE_Z      0x851A

#define CUBE_MAP_SIZE 256

Skybox::Skybox(
        ShaderProgramPtr shaderProgram, const std::string& textureFilename)
: HierarchicalRenderable(shaderProgram),
m_pBuffer(0), m_nBuffer(0), m_tBuffer(0), m_texId(0) {
//    //Initialize geometry
//    teachers::getUnitCube(m_positions, m_normals, m_texCoords);
//
//    // === PART 1: Vertex attributes, except texture coordinates
//    //Create buffers
//    glcheck(glGenBuffers(1, &m_pBuffer)); //vertices
//    glcheck(glGenBuffers(1, &m_nBuffer)); //normals
//
//    //Activate buffer and send data to the graphics card
//    glcheck(glBindBuffer(GL_ARRAY_BUFFER, m_pBuffer));
//    glcheck(glBufferData(GL_ARRAY_BUFFER, m_positions.size() * sizeof (glm::vec3), m_positions.data(), GL_STATIC_DRAW));
//    glcheck(glBindBuffer(GL_ARRAY_BUFFER, m_nBuffer));
//    glcheck(glBufferData(GL_ARRAY_BUFFER, m_normals.size() * sizeof (glm::vec3), m_normals.data(), GL_STATIC_DRAW));
//
//    // === PART 2: Texture
//    glGenTextures(1, &m_texId);
//    glBindTexture(GL_TEXTURE_CUBE_MAP, m_texId);
//
//    // Liste des faces successives pour la création des textures de CubeMap
//
//    std::vector<GLenum> cube_map_target;
//    cube_map_target.push_back(GL_TEXTURE_CUBE_MAP_NEGATIVE_X);
//    cube_map_target.push_back(GL_TEXTURE_CUBE_MAP_POSITIVE_X);
//    cube_map_target.push_back(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y);
//    cube_map_target.push_back(GL_TEXTURE_CUBE_MAP_POSITIVE_Y);
//    cube_map_target.push_back(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z);
//    cube_map_target.push_back(GL_TEXTURE_CUBE_MAP_POSITIVE_Z);
//
//    // now handle the "texture image" itself
//    // load the image (here using the sfml library)
//    sf::Image image;
//    image.loadFromFile(textureFilename);
//    // sfml inverts the v axis...
//    // Hence, flip it to put the image in OpenGL convention: lower left corner is (0,0)
//    image.flipVertically();
//
//    //**********chargement des textures
//
//    std::vector<sf::Image> textures;
//    textures[0] = image;
//    textures[1] = image;
//    textures[2] = image;
//    textures[3] = image;
//    textures[4] = image;
//    textures[5] = image;
//
//    for (int i = 0; i < 6; i++) {
//
//        sf::Image im = textures[i];
//        glTexImage2D(cube_map_target[i], 0, 3, im.getSize().x, im.getSize().y, 0, GL_RGBA, GL_UNSIGNED_BYTE, im.getPixelsPtr());
//    }
//
//    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
//    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
//    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);  

}

Skybox::~Skybox() {

    glcheck(glDeleteBuffers(1, &m_pBuffer));
    glcheck(glDeleteBuffers(1, &m_tBuffer));
    glcheck(glDeleteBuffers(1, &m_nBuffer));

    glcheck(glDeleteTextures(1, &m_texId));
}

void Skybox::do_draw() {
//    //Locations
//    int modelLocation = m_shaderProgram->getUniformLocation("modelMat");
//    int nitLocation = m_shaderProgram->getUniformLocation("NIT");
//    int texSamplerLocation = m_shaderProgram->getUniformLocation("texSampler");
//
//    int positionLocation = m_shaderProgram->getAttributeLocation("vPosition");
//    int normalLocation = m_shaderProgram->getAttributeLocation("vNormal");
//    int texCoordLocation = m_shaderProgram->getAttributeLocation("vTexCoord");
//
//    //Send material as uniform to GPU
//    Material::sendToGPU(m_shaderProgram, m_material);
//
//    //Send uniforms to GPU
//    if (modelLocation != ShaderProgram::null_location) {
//        glcheck(glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(getModelMatrix())));
//    }
//    if (nitLocation != ShaderProgram::null_location) {
//        glcheck(glUniformMatrix3fv(nitLocation, 1, GL_FALSE,
//                glm::value_ptr(glm::transpose(glm::inverse(glm::mat3(getModelMatrix()))))));
//    }
//
//    //Send vertex attributes to GPU
//    if (positionLocation != ShaderProgram::null_location) {
//        glcheck(glEnableVertexAttribArray(positionLocation));
//        glcheck(glBindBuffer(GL_ARRAY_BUFFER, m_pBuffer));
//        glcheck(glVertexAttribPointer(positionLocation, 3, GL_FLOAT, GL_FALSE, 0, (void*) 0));
//    }
//    if (normalLocation != ShaderProgram::null_location) {
//        glcheck(glEnableVertexAttribArray(normalLocation));
//        glcheck(glBindBuffer(GL_ARRAY_BUFFER, m_nBuffer));
//        glcheck(glVertexAttribPointer(normalLocation, 3, GL_FLOAT, GL_FALSE, 0, (void*) 0));
//    }
//
//    // Texture and texture coordinates
//    if (texCoordLocation != ShaderProgram::null_location) {
//        // Bind texture on texture unit 0
//        glcheck(glActiveTexture(GL_TEXTURE0));
//        glcheck(glBindTexture(GL_TEXTURE_2D, m_texId));
//
//        // Tells the sampler to use the texture unit 0
//        glcheck(glUniform1i(texSamplerLocation, 0));
//
//        // Send texture coordinates attributes
//        glcheck(glEnableVertexAttribArray(texCoordLocation));
//        glcheck(glBindBuffer(GL_ARRAY_BUFFER, m_tBuffer));
//        glcheck(glVertexAttribPointer(texCoordLocation, 2, GL_FLOAT, GL_FALSE, 0, (void*) 0));
//    }
//
//    //Draw triangles elements
//    glcheck(glDrawArrays(GL_TRIANGLES, 0, m_positions.size()));
//
//
//    
//    
//
//
//    glEnable(GL_TEXTURE_CUBE_MAP);
//    glBindTexture(GL_TEXTURE_CUBE_MAP, cube_map_texture_ID);
//
//    //(les VBO ont été préalablement alloués)
//
//    // Utilisation des données des buffers
//    glBindBuffer(GL_ARRAY_BUFFER, VBObuffersId[0]);
//    glVertexPointer(3, GL_FLOAT, 0, BUFFER_OFFSET(0));
//
//    //stride de 3 car cube map
//    glTexCoordPointer(3, GL_FLOAT, 0, BUFFER_OFFSET(VBOverticesSize * sizeof (float)));
//
//
//    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VBObuffersId[1]);
//
//    glDrawElements(GL_QUADS, 6*4, GL_UNSIGNED_INT, 0);
//
//
//
//
//
//    //Release everything
//    if (positionLocation != ShaderProgram::null_location) {
//        glcheck(glDisableVertexAttribArray(positionLocation));
//    }
//    if (normalLocation != ShaderProgram::null_location) {
//        glcheck(glDisableVertexAttribArray(normalLocation));
//    }
//
//    if (texCoordLocation != ShaderProgram::null_location) {
//        glcheck(glDisableVertexAttribArray(texCoordLocation));
//        glcheck(glBindTexture(GL_TEXTURE_2D, 0)); // unbind the texture!
//    }
}

void Skybox::do_animate(float time) {

}

void Skybox::setMaterial(const MaterialPtr& material) {
    m_material = material;
}
