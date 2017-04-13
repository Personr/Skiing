#include "../include/Viewer.hpp"
#include "../include/FrameRenderable.hpp"
#include "../include/SlalomRenderable.hpp"

#include "../include/dynamics/DynamicSystem.hpp"
#include "../include/dynamics/DampingForceField.hpp"
#include "../include/dynamics/ConstantForceField.hpp"
#include "../include/dynamics/SpringForceField.hpp"
#include "../include/dynamics/EulerExplicitSolver.hpp"
#include "../include/dynamics/FollowedParticle.hpp"

#include "../include/dynamics_rendering/DynamicSystemRenderable.hpp"
#include "../include/dynamics_rendering/ParticleRenderable.hpp"
#include "../include/dynamics_rendering/SkierRenderable.hpp"
#include "../include/dynamics_rendering/HeadRenderable.hpp"
#include "../include/dynamics_rendering/ArmRenderable.hpp"
#include "../include/dynamics_rendering/LegRenderable.hpp"
#include "../include/dynamics_rendering/BodyCylinderRenderable.hpp"
#include "../include/dynamics_rendering/StickRenderable.hpp"
#include "../include/dynamics_rendering/SkiRenderable.hpp"
#include "../include/dynamics_rendering/ParticleListRenderable.hpp"
#include "../include/dynamics_rendering/ConstantForceFieldRenderable.hpp"
#include "../include/dynamics_rendering/SpringForceFieldRenderable.hpp"
#include "../include/dynamics_rendering/SpringListRenderable.hpp"
#include "../include/dynamics_rendering/ControlledForceFieldRenderable.hpp"
#include "../include/dynamics_rendering/QuadRenderable.hpp"

#include "../include/texturing/TexturedPlaneRenderable.hpp"
#include "../include/BillboardRenderable.hpp"
#include "../include/lighting/DirectionalLightRenderable.hpp"
#include "../include/lighting/LightedMeshRenderable.hpp"
#include "../include/TreeCylinderRenderable.hpp"
#include "../include/TreeRenderable.hpp"


void practical07_particles(Viewer& viewer,
    DynamicSystemPtr& system, DynamicSystemRenderablePtr& systemRenderable);

void practical07_springs(Viewer& viewer,
    DynamicSystemPtr& system, DynamicSystemRenderablePtr& systemRenderable);

void practical07_collisions(Viewer& viewer,
    DynamicSystemPtr& system, DynamicSystemRenderablePtr& systemRenderable);

void practical07_playPool(Viewer& viewer,
    DynamicSystemPtr& system, DynamicSystemRenderablePtr& systemRenderable);

void createSkier(ShaderProgramPtr flatShader, ShaderProgramPtr phongShader,
    ParticlePtr mobile, DynamicSystemRenderablePtr& systemRenderable);

void addPannel(float x, float y, float z, Viewer& viewer, 
        ShaderProgramPtr texShader, ShaderProgramPtr multiTexShader);


void initialize_practical_07_scene(Viewer& viewer, unsigned int scene_to_load)
{
    //Set up a shader and add a 3D frame.
    ShaderProgramPtr flatShader =
        std::make_shared<ShaderProgram>("../shaders/flatVertex.glsl",
                                        "../shaders/flatFragment.glsl");
    viewer.addShaderProgram(flatShader);
    FrameRenderablePtr frame = std::make_shared<FrameRenderable>(flatShader);
    viewer.addRenderable(frame);


 ShaderProgramPtr phongShader = std::make_shared<ShaderProgram>(
        "../shaders/phongVertex.glsl", "../shaders/phongFragment.glsl");
 viewer.addShaderProgram(phongShader);

// Two texture shaders: simple and multi-texturing
    ShaderProgramPtr texShader
        = std::make_shared<ShaderProgram>("../shaders/textureVertex.glsl",
                                          "../shaders/textureFragment.glsl");
    viewer.addShaderProgram(texShader);
    //Initialize a dynamic system (Solver, Time step, Restitution coefficient)
    DynamicSystemPtr system = std::make_shared<DynamicSystem>();
    EulerExplicitSolverPtr solver = std::make_shared<EulerExplicitSolver>();
    system->setSolver(solver);
    system->setDt(0.005);

    //Temporary variables
    glm::mat4 parentTransformation(1.0), localTransformation(1.0),parentTransformation2(1.0);
    std::string filename;
    MaterialPtr pearl = Material::Pearl();

    //Define a directional light for the whole scene
    glm::vec3 d_direction = glm::normalize(glm::vec3(0.0,0.0,-1.0));
    glm::vec3 d_ambient(1,1,1), d_diffuse(1,1,1), d_specular(1,1,1);
    DirectionalLightPtr directionalLight = std::make_shared<DirectionalLight>(d_direction, d_ambient, d_diffuse, d_specular);
    //Add a renderable to display the light and control it via mouse/key event
    glm::vec3 lightPosition(0.0,0.0,5.0);
    DirectionalLightRenderablePtr directionalLightRenderable = std::make_shared<DirectionalLightRenderable>(flatShader, directionalLight, lightPosition);
    localTransformation = glm::scale(glm::mat4(1.0), glm::vec3(0.5,0.5,0.5));
    directionalLightRenderable->setLocalTransform(localTransformation);
    viewer.setDirectionalLight(directionalLight);
    viewer.addRenderable(directionalLightRenderable);


    //  filename = "../textures/neige.png";
    //filename = "../textures/checkerboard.png";
    //TexturedPlaneRenderablePtr texPlane = std::make_shared<TexturedPlaneRenderable>(texShader, filename);




    //Create a renderable associated to the dynamic system
    //This renderable is responsible for calling DynamicSystem::computeSimulationStep()in the animate() function
    //It also handles some of the key/mouse events
    DynamicSystemRenderablePtr systemRenderable = std::make_shared<DynamicSystemRenderable>(system);
    viewer.addRenderable(systemRenderable);

    //Populate the dynamic system with particles, forcefields
    //and create renderables associated to them for visualization.
    switch (scene_to_load) {
        case 71:
            practical07_particles(viewer, system, systemRenderable);
            break;
        case 72:
            practical07_springs(viewer, system, systemRenderable);
            break;
        case 73:
            practical07_collisions(viewer, system, systemRenderable);
            break;
        case 74:
            practical07_playPool(viewer, system, systemRenderable);
            break;
        default:
            break;
    }

    //Finally, run the animation
    viewer.startAnimation();
}


void practical07_particles(Viewer& viewer, DynamicSystemPtr& system, DynamicSystemRenderablePtr &systemRenderable)
{
    //Position the camera
    viewer.getCamera().setViewMatrix(
        glm::lookAt(glm::vec3(0, -4, 0), glm::vec3(0,0,0), glm::vec3(0,0,1)) );

    //Initialize a shader for the following renderables
    ShaderProgramPtr flatShader
        = std::make_shared<ShaderProgram>("../shaders/flatVertex.glsl","../shaders/flatFragment.glsl");
    viewer.addShaderProgram(flatShader);

    //We diminish the time step to be able to see what happens before particles go too far
    system->setDt(5e-4);

    //Deactivate collision detection
    system->setCollisionsDetection(false);

    glm::vec3 px,pv;
    float pm, pr;
    //Two particles with gravity and damping
    {
        //Initialize each particle with its position, velocity, mass and radius
        //and add it to the system
        px = glm::vec3(0.0, 0.0, 1.0);
        pv = glm::vec3(3.0, 0.0, 0.0);
        pr = 0.1;
        pm = 1.0;
        ParticlePtr particle1 = std::make_shared<Particle>(px, pv, pm, pr);
        system->addParticle(particle1);

        px = glm::vec3(0.0, 0.0, 1.5);
        pv = glm::vec3(6.0, 0.0, 0.0);
        pr = 0.1;
        pm = 1.0;
        ParticlePtr particle2 = std::make_shared<Particle>(px, pv, pm, pr);
        system->addParticle(particle2);

        px = glm::vec3(0.0, 0.0, 1.5);
        pv = glm::vec3(6.5, 0.0, 0.0);
        pr = 0.1;
        pm = 1.0;
        Camera& camera = viewer.getCamera();
        std::cout << &camera;
        FollowedParticlePtr particle3 = std::make_shared<FollowedParticle>(px, pv, pm, pr, &camera);
        system->addParticle(particle3);

        //Initialize a force field that apply to all the particles of the system to simulate gravity
        //Add it to the system as a force field
        ConstantForceFieldPtr gravityForceField
            = std::make_shared<ConstantForceField>(system->getParticles(), glm::vec3{0, 0, -10} );
        system->addForceField(gravityForceField);

        //Create a particleRenderable for each particle of the system
        //DynamicSystemRenderable act as a hierarchical renderable
        //This allows easily applying a transformation on the visualization of a dynamicSystem
        ParticleRenderablePtr particleRenderable1
            = std::make_shared<ParticleRenderable>(flatShader, particle1);
        HierarchicalRenderable::addChild(systemRenderable, particleRenderable1);
        ParticleRenderablePtr particleRenderable2
            = std::make_shared<ParticleRenderable>(flatShader, particle2);
        HierarchicalRenderable::addChild(systemRenderable, particleRenderable2);
        ParticleRenderablePtr particleRenderable3
            = std::make_shared<ParticleRenderable>(flatShader, particle3);
        HierarchicalRenderable::addChild(systemRenderable, particleRenderable3);
    }
}


void practical07_springs(Viewer& viewer, DynamicSystemPtr& system, DynamicSystemRenderablePtr &systemRenderable)
{
    //Position the camera
    viewer.getCamera().setViewMatrix(
        glm::lookAt(glm::vec3(-10, 0, 2), glm::vec3(0,0,0), glm::vec3(0,0,1)) );

    //Initialize a shader for the following renderables
    ShaderProgramPtr flatShader
        = std::make_shared<ShaderProgram>("../shaders/flatVertex.glsl","../shaders/flatFragment.glsl");
    viewer.addShaderProgram(flatShader);

    //Initialize particle attributes (radius, mass)
    float pr = 0.1, pm = 10.0;
    glm::vec3 px(0.0,0.0,0.0), pv(0.0,0.0,0.0);

    //Create particles on a squared uniform grid starting at origin
    std::vector<ParticlePtr> particles;
    glm::vec3 origin(0,-2.0,1.0), displacement(0.0,0.0,0.0);
    int particlePerLine = 11;
    float gridWidth=4.0, gridHeight=4.0;
    float ystep = gridWidth / (float)(particlePerLine-1);
    float zstep = gridHeight / (float)(particlePerLine-1);
    particles.resize(particlePerLine*particlePerLine);
    for (size_t i = 0; i < particlePerLine; ++ i) {
        for (size_t j = 0; j < particlePerLine; ++ j) {
            displacement = glm::vec3(0, i*ystep, j*zstep);
            px = origin + displacement;
            particles[i*particlePerLine+j] = std::make_shared<Particle>( px, pv, pm, pr );
            system->addParticle( particles[i*particlePerLine+j] );
        }
    }

    //Fix particles on the y-borders
    for (size_t j = 0; j < particlePerLine; ++j) {
        particles[0 * particlePerLine + j]->setFixed(true);
        particles[(particlePerLine - 1) * particlePerLine + j]->setFixed(true);
    }

    //Initialize springs attributes (stiffness, rest length, damping)
    float stiffness = 4e3;
    float l0 = gridWidth / (particlePerLine-1);
    float damping = 0.0;

    //Create springs between particles of the grid, horizontally and vertically
    //Store them in a list
    std::list<SpringForceFieldPtr> springForceFields;
    //Interior
    for (size_t i = 1; i < particlePerLine; ++ i) {
        for (size_t j = 1; j < particlePerLine; ++ j) {
            SpringForceFieldPtr spring1 = std::make_shared<SpringForceField>( particles[(i-1)*particlePerLine+j], particles[i*particlePerLine+j], stiffness, l0, damping );
            springForceFields.push_back(spring1);
            system->addForceField(spring1);
            SpringForceFieldPtr spring2 = std::make_shared<SpringForceField>( particles[i*particlePerLine+(j-1)], particles[i*particlePerLine+j], stiffness, l0, damping );
            springForceFields.push_back(spring2);
            system->addForceField(spring2);
        }
    }
    //Border case 1
    for (size_t j = 1; j < particlePerLine; ++ j) {
        SpringForceFieldPtr spring = std::make_shared<SpringForceField>( particles[0*particlePerLine+j], particles[0*particlePerLine+(j-1)], stiffness, l0, damping );
        springForceFields.push_back(spring);
        system->addForceField(spring);
    }
    //Border case 2
    for (size_t i = 1; i < particlePerLine; ++ i) {
        SpringForceFieldPtr spring = std::make_shared<SpringForceField>( particles[(i-1)*particlePerLine+0], particles[i*particlePerLine+0], stiffness, l0, damping );
        springForceFields.push_back(spring);
        system->addForceField(spring);
    }

    //Initialize a force field that apply to all the particles of the system to simulate gravity
    //Add it to the system as a force field
    ConstantForceFieldPtr gravityForceField = std::make_shared<ConstantForceField>(system->getParticles(), glm::vec3{0, 0, -10});
    system->addForceField(gravityForceField);

    //Initialize a force field that apply to all the particles of the system to simulate vicosity (air friction)
    float dampingCoefficient = 5.0;
    DampingForceFieldPtr dampingForceField = std::make_shared<DampingForceField>(system->getParticles(), dampingCoefficient);
    system->addForceField(dampingForceField);


    //Create a particleListRenderable to efficiently visualize the particles of the system
    ParticleListRenderablePtr particleListRenderable = std::make_shared<ParticleListRenderable>( flatShader, particles);
    HierarchicalRenderable::addChild(systemRenderable, particleListRenderable);

    //Create a springListRenderable to efficiently visualize the springs of the system
    SpringListRenderablePtr springsRenderable = std::make_shared<SpringListRenderable>(flatShader, springForceFields);
    HierarchicalRenderable::addChild(systemRenderable, springsRenderable);

    //Display gravity
    ConstantForceFieldRenderablePtr gravityRenderable = std::make_shared<ConstantForceFieldRenderable>(flatShader, gravityForceField);
    HierarchicalRenderable::addChild(systemRenderable, gravityRenderable);
}


/*
void createPlane(ShaderProgramPtr flatShader, DynamicSystemRenderablePtr& systemRenderable ){

  glm::mat4 parentTransformation(1.0), localTransformation(1.0),parentTransformation2(1.0);
  std::string filename;
  MaterialPtr pearl = Material::Pearl();
  filename = "../textures/neige.jpg";
  //filename = "../textures/checkerboard.png";
  TexturedPlaneRenderablePtr texPlane = std::make_shared<TexturedPlaneRenderable>(texShader, filename);

  parentTransformation = glm::scale(glm::mat4(1.0), glm::vec3(60.0,200.0,50.0));
  texPlane->setParentTransform(parentTransformation);
  texPlane->setMaterial(pearl);
  viewer.addRenderable(texPlane);
  float anglecour=0.1;
  float xcour = 30;
  float zcour = 0;
  for (int l=0; l<6; l++){
    TexturedPlaneRenderablePtr texPlane2 = std::make_shared<TexturedPlaneRenderable>(texShader, filename);
    parentTransformation2 = glm::scale(glm::mat4(1.0), glm::vec3(10.0,200.0,50.0));
    parentTransformation2 =glm::rotate(glm::mat4(1.0), -anglecour, glm::vec3(0,1,0))* parentTransformation2;
    parentTransformation2 = glm::translate(glm::mat4(1.0), glm::vec3((xcour+5*cos(anglecour)),0.0,(zcour+5*sin(anglecour))))*parentTransformation2;
    xcour+=10*cos(anglecour);
    zcour+=10*sin(anglecour);
    anglecour+=(0.1);
    texPlane2->setParentTransform(parentTransformation2);
    texPlane2->setMaterial(pearl);
    viewer.addRenderable(texPlane2);
  }
  TexturedPlaneRenderablePtr texPlane2 = std::make_shared<TexturedPlaneRenderable>(texShader, filename);
  parentTransformation2 = glm::scale(glm::mat4(1.0), glm::vec3(300.0,200.0,50.0));
  parentTransformation2 =glm::rotate(glm::mat4(1.0), -anglecour, glm::vec3(0,1,0))* parentTransformation2;
  parentTransformation2 = glm::translate(glm::mat4(1.0), glm::vec3((xcour+150*cos(anglecour)),0.0,(zcour+150*sin(anglecour))))*parentTransformation2;
  texPlane2->setParentTransform(parentTransformation2);
  texPlane2->setMaterial(pearl);
  viewer.addRenderable(texPlane2);

  //Initialize a plane from 3 points and add it to the system as an obstacle
  //Plan 1
  glm::vec3 p1, p2, p3;
  PlanePtr plane;
  xcour =0;
  float xsuiv = 30;
  zcour =0;
  float zsuiv = 0;
  // float alphacour=3.14159/8;
  float alphacour=0.1;

  for (float i = 0; i<8; i++){
    p1= glm::vec3 (xcour, 0.0f, zcour);
    p2=glm::vec3(xsuiv, 0.0f, zsuiv);
    p3=glm::vec3(xcour, 1.0f, zcour);               //Plan incliné
    xcour = xsuiv;
    zcour = zsuiv;
    xsuiv = xsuiv+10*cos(alphacour);
    zsuiv = zsuiv+10*sin(alphacour);
    // alphacour=alphacour+3.14159/8;
    alphacour=alphacour+0.1;
    plane = std::make_shared<Plane>(p1, p2, p3);
    system->addPlaneObstacle(plane);
  }


}


*/


void practical07_collisions(Viewer& viewer, DynamicSystemPtr& system, DynamicSystemRenderablePtr &systemRenderable)
{

  //Position the camera
  viewer.getCamera().setViewMatrix(
				   glm::lookAt(glm::vec3(1, 0, 15), glm::vec3(0,0,0), glm::vec3(0,0,(intptr_t)time)) );//Pos, origine, et sens caméra

  //Initialize a shader for the following renderables
  ShaderProgramPtr flatShader
    = std::make_shared<ShaderProgram>("../shaders/flatVertex.glsl","../shaders/flatFragment.glsl");
  viewer.addShaderProgram(flatShader);

 ShaderProgramPtr phongShader = std::make_shared<ShaderProgram>(
        "../shaders/phongVertex.glsl", "../shaders/phongFragment.glsl");
 viewer.addShaderProgram(phongShader);


//Multi-textured cube
    ShaderProgramPtr multiTexShader
        = std::make_shared<ShaderProgram>("../shaders/multiTextureVertex.glsl",
                                          "../shaders/multiTextureFragment.glsl");
    viewer.addShaderProgram(multiTexShader);

    // Two texture shaders: simple and multi-texturing
    ShaderProgramPtr texShader
        = std::make_shared<ShaderProgram>("../shaders/textureVertex.glsl",
                                          "../shaders/textureFragment.glsl");
    viewer.addShaderProgram(texShader);
    
    
    
    
    
    addPannel(5.0F, 5.0F, 0.0F, viewer, texShader, multiTexShader);
  auto tree = std::make_shared<TreeRenderable>(phongShader, 4.0);
  viewer.addRenderable(tree);
  //Activate collision detection
  system->setCollisionsDetection(true);

  //Initialize the restitution coefficient for collision
  //1.0 = full elastic response
  //0.0 = full absorption
  system->setRestitution(.10f);

  //createPlane(texShader, systemRenderable);



  glm::mat4 parentTransformation(1.0), localTransformation(1.0),parentTransformation2(1.0);
    std::string filename;
    filename = "../textures/neige.jpg";
    //filename = "../textures/checkerboard.png";
    TexturedPlaneRenderablePtr texPlane = std::make_shared<TexturedPlaneRenderable>(texShader, filename);

    parentTransformation = glm::scale(glm::mat4(1.0), glm::vec3(60.0,200.0,50.0));
    texPlane->setParentTransform(parentTransformation);
    MaterialPtr pearl = Material::Pearl();
    texPlane->setMaterial(pearl);
    viewer.addRenderable(texPlane);
    float anglecour=0.02;
    float xcour = 30;
    float zcour = 0;
    for (int l=0; l<20; l++){
      TexturedPlaneRenderablePtr texPlane2 = std::make_shared<TexturedPlaneRenderable>(texShader, filename);
      parentTransformation2 = glm::scale(glm::mat4(1.0), glm::vec3(20.0,200.0,50.0));
      parentTransformation2 =glm::rotate(glm::mat4(1.0), -anglecour, glm::vec3(0,1,0))* parentTransformation2;
      parentTransformation2 = glm::translate(glm::mat4(1.0), glm::vec3((xcour+10*cos(anglecour)),0.0,(zcour+10*sin(anglecour))))*parentTransformation2;
      xcour+=20*cos(anglecour);
      zcour+=20*sin(anglecour);
      anglecour+=(0.02);
      texPlane2->setParentTransform(parentTransformation2);
      texPlane2->setMaterial(pearl);
      viewer.addRenderable(texPlane2);
    }
    TexturedPlaneRenderablePtr texPlane2 = std::make_shared<TexturedPlaneRenderable>(texShader, filename);
    parentTransformation2 = glm::scale(glm::mat4(1.0), glm::vec3(300.0,200.0,50.0));
    parentTransformation2 =glm::rotate(glm::mat4(1.0), -anglecour, glm::vec3(0,1,0))* parentTransformation2;
    parentTransformation2 = glm::translate(glm::mat4(1.0), glm::vec3((xcour+150*cos(anglecour)),0.0,(zcour+150*sin(anglecour))))*parentTransformation2;
    texPlane2->setParentTransform(parentTransformation2);
    texPlane2->setMaterial(pearl);
    viewer.addRenderable(texPlane2);

  //Initialize a plane from 3 points and add it to the system as an obstacle
  //Plan 1
  glm::vec3 p1, p2, p3;
  PlanePtr plane;
  xcour =0;
  float xsuiv = 30;
  zcour =0;
  float zsuiv = 0;
  // float alphacour=3.14159/8;
  float alphacour=0.02;
  float k = -1;
  for (float i = 0; i<22; i++){
    k = k*-1;
    p1= glm::vec3 (xcour, 0.0f, zcour);
    p2=glm::vec3(xsuiv, 0.0f, zsuiv);
    p3=glm::vec3(xcour, 1.0f, zcour);               //Plan incliné
    xcour = xsuiv;
    zcour = zsuiv;
    xsuiv = xsuiv+20*cos(alphacour);
    zsuiv = zsuiv+20*sin(alphacour);
    // alphacour=alphacour+3.14159/8;
    alphacour=alphacour+0.02;
    plane = std::make_shared<Plane>(p1, p2, p3);
    system->addPlaneObstacle(plane);

    SlalomRenderablePtr slalom = std::make_shared<SlalomRenderable>(flatShader, xcour, 0, zcour);
    viewer.addRenderable(slalom);
    slalom = std::make_shared<SlalomRenderable>(flatShader, xcour, 10*k, zcour);
    viewer.addRenderable(slalom);
  }
  //coté opposé
  /*
    xcour =0;
    xsuiv = 30;
    zcour =0;
    zsuiv = 0;
    alphacour=3.14159/8;
    for (float i = 0; i<6; i++){
    p1= glm::vec3 (-xcour, 0.0f, zcour);
    p2=glm::vec3(-xsuiv, 0.0f, zsuiv);
    p3=glm::vec3(-xcour, 1.0f, zcour);               //Plan incliné
    xcour = xsuiv;
    zcour = zsuiv;
    xsuiv = xsuiv+10*cos(alphacour);
    zsuiv = zsuiv+10*sin(alphacour);
    alphacour=alphacour+3.14159/8;
    plane = std::make_shared<Plane>(p1, p2, p3);
    system->addPlaneObstacle(plane);
    }*/
  glm::vec3 px,pv;
  float pm, pr;
  //Particle vs Plane collision

  {
    srand (static_cast <unsigned> (time(0)));
    float randx = 1.0f;
    float randy = 1.0f;
    float randz = 0.0f;
    float randr = 0.0f;
    int i =0;

    time_t t0 = time(NULL);
    unsigned tmax = 2;
    int j =0;

      for (i=0; i <= 100; i = i + 1) {
      //Initialize a particle with position, velocity, mass and radius and add it to the system

      randx = 2*(rand() / (RAND_MAX + 1.)) -1;
      randy = 2*(rand() / (RAND_MAX + 1.)) -1;
      randz = 2*(rand() / (RAND_MAX + 1.)) -1;
      randr = 2*(rand() / (RAND_MAX + 1.)) -1;

      px = glm::vec3(30*randx,30*randy, 5*randz + 5);
      pv = glm::vec3(0.0, 0.0, 0.0);
      pr = 0.05*randr + 0.1;
      pm = pr;
      ParticlePtr particle = std::make_shared<Particle>(px, pv, pm, pr);
      system->addParticle(particle);

      //Create a particleRenderable for each particle of the system
      //DynamicSystemRenderable act as a hierarchical renderable
      //This which allows to easily apply transformation on the visualiazation of a dynamicSystem
      ParticleRenderablePtr particleRenderable = std::make_shared<ParticleRenderable>(flatShader, particle);
      HierarchicalRenderable::addChild(systemRenderable, particleRenderable);                 //laaaaaaaaaaaaaaaaaaaaaaaaa

      }

       //Initialize two particles with position, velocity, mass and radius and add it to the system
    glm::vec3 px(0.0, 0.0, 0.0);//A remplacer par le skieur
    glm::vec3 pv(0.0, 0.0, 0.0);
    float pm = 1.0, pr = 1.0;
    xcour = xcour+(xsuiv-xcour)*10;
    zcour = zcour+(zsuiv-zcour)*10+5;
    px = glm::vec3(xcour,0.0,zcour);

    Camera& camera = viewer.getCamera();
    FollowedParticlePtr mobile = std::make_shared<FollowedParticle>( px, pv, pm, pr, &camera);
    system->addParticle( mobile );
    px = glm::vec3(0.0,5.0,1.0);
    ParticlePtr other = std::make_shared<Particle>( px, pv, pm, pr);
    system->addParticle( other );

    //Create a particleRenderable for each particle of the system
    //Add them to the system renderable
    //ParticleRenderablePtr mobileRenderable = std::make_shared<ParticleRenderable>(flatShader, mobile);
    //HierarchicalRenderable::addChild(systemRenderable, mobileRenderable);
    createSkier(flatShader, phongShader, mobile, systemRenderable);
    ParticleRenderablePtr otherRenderable = std::make_shared<ParticleRenderable>(flatShader, other);
    HierarchicalRenderable::addChild(systemRenderable, otherRenderable);
    glm::vec3 nullForce(0.0, 0.0, 0.0);
    std::vector<ParticlePtr> vParticle;
    vParticle.push_back(mobile);
    ConstantForceFieldPtr force = std::make_shared<ConstantForceField>(vParticle, nullForce);
    system->addForceField(force);

    //Initialize a renderable for the force field applied on the mobile particle.
    //This renderable allows to modify the attribute of the force by key/mouse events
    //Add this renderable to the systemRenderable.
    ControlledForceFieldRenderablePtr forceRenderable = std::make_shared<ControlledForceFieldRenderable>(flatShader, force);
    HierarchicalRenderable::addChild(systemRenderable, forceRenderable);

    //Add a damping force field to the mobile.
    DampingForceFieldPtr dampingForceField = std::make_shared<DampingForceField>(vParticle, 0.9);
    system->addForceField(dampingForceField);

  }
  //Initialize a force field that apply to all the particles of the system to simulate gravity
  //Add it to the system as a force field
  ConstantForceFieldPtr gravityForceField = std::make_shared<ConstantForceField>(system->getParticles(), glm::vec3{0,0,-10} );
  system->addForceField(gravityForceField);



  // ConstantForceFieldPtr windForceField = std::make_shared<ConstantForceField>(system->getParticles(), glm::vec3{0,1,0} );
  //  system->addForceField(windForceField);

}

void addPannel(float x, float y, float z, Viewer& viewer, ShaderProgramPtr texShader, ShaderProgramPtr multiTexShader) {
    MaterialPtr pearl = Material::Pearl();
    std::string filename1 = "../textures/freestyle.jpg", filename2 = "../textures/plan.jpg";
    BillboardRenderablePtr multitexCube = std::make_shared<BillboardRenderable>(multiTexShader, texShader, filename1, filename2, x, y, z, &viewer);
    //    parentTransformation = glm::translate(glm::mat4(1.0), glm::vec3(5,0.0,0.5));
    //    multitexCube->setParentTransform(parentTransformation);
    multitexCube->setMaterial(pearl);
    viewer.addRenderable(multitexCube);
    //Define a spot light
    glm::vec3 s_position(x, y+1.0, z + 2), s_spotDirection = glm::normalize(glm::vec3(0.0, -1.0, 1.5));
    glm::vec3 s_ambient(1.0, 1.0, 1.0), s_diffuse(1.0, 1.0, 1.0), s_specular(1.0, 1.0, 1.0);
    float s_constant = 0.0, s_linear = 1.0, s_quadratic = 0.0;
    float s_cosInnerCutOff = std::cos(glm::radians(40.0f));
    float s_cosOuterCutOff = std::cos(glm::radians(80.0f));
    SpotLightPtr spotLight = std::make_shared<SpotLight>(s_position, s_spotDirection,
            s_ambient, s_diffuse, s_specular,
            s_constant, s_linear, s_quadratic,
            s_cosInnerCutOff, s_cosOuterCutOff);
    s_position = glm::vec3(x, y-1.0, z + 2); 
    s_spotDirection = glm::normalize(glm::vec3(0.0, 1.0, 1.5));
    SpotLightPtr spotLight2 = std::make_shared<SpotLight>(s_position, s_spotDirection,
            s_ambient, s_diffuse, s_specular,
            s_constant, s_linear, s_quadratic,
            s_cosInnerCutOff, s_cosOuterCutOff);
    viewer.addSpotLight(spotLight);
    viewer.addSpotLight(spotLight2);
}

void createSkier(ShaderProgramPtr flatShader, ShaderProgramPtr phongShader, ParticlePtr mobile, DynamicSystemRenderablePtr& systemRenderable) {
    glm::vec3 legColor(25.0, 25.0, 255.0);
    glm::vec3 torsoColor(0.0, 204.0, 0.0);
    glm::vec3 skiColor(204.0, 0.0, 0.0);
    glm::vec3 stickColor = skiColor;

    //Create a particleRenderable for each particle of the system
    //Add them to the system renderable
    SkierRenderablePtr mobileRenderable = std::make_shared<SkierRenderable>(flatShader, mobile, torsoColor);
    SkiRenderablePtr lSkiRenderable = std::make_shared<SkiRenderable>(flatShader, mobile, skiColor, true);
    SkiRenderablePtr rSkiRenderable = std::make_shared<SkiRenderable>(flatShader, mobile, skiColor, false);
    ArmRenderablePtr lArmRenderable = std::make_shared<ArmRenderable>(flatShader, mobile, torsoColor, true);
    ArmRenderablePtr rArmRenderable = std::make_shared<ArmRenderable>(flatShader, mobile, torsoColor, false);
    BodyCylinderRenderablePtr lForearmRenderable = std::make_shared<BodyCylinderRenderable>(flatShader, torsoColor);
    BodyCylinderRenderablePtr rForearmRenderable = std::make_shared<BodyCylinderRenderable>(flatShader, torsoColor);
    LegRenderablePtr lThighRenderable = std::make_shared<LegRenderable>(flatShader, mobile, legColor, true);
    LegRenderablePtr rThighRenderable = std::make_shared<LegRenderable>(flatShader, mobile, legColor, false);
    BodyCylinderRenderablePtr lTibiaRenderable = std::make_shared<BodyCylinderRenderable>(flatShader, legColor);
    BodyCylinderRenderablePtr rTibiaRenderable = std::make_shared<BodyCylinderRenderable>(flatShader, legColor);
    StickRenderablePtr rStickRenderable = std::make_shared<StickRenderable>(flatShader, stickColor);
    StickRenderablePtr lStickRenderable = std::make_shared<StickRenderable>(flatShader, stickColor);


    float headZAngle = 1.9 * 3.14 / 4;
    float headXAngle = 3.14 / 2;
    LightedMeshRenderablePtr suzanneBoyle = std::make_shared<LightedMeshRenderable>(phongShader, "./../meshes/magnetto2.obj", Material::Chrome());
    glm::mat4 parentTransform = glm::translate(glm::mat4(1.0), glm::vec3(0,0.0,4));
    parentTransform = glm::rotate(parentTransform, headXAngle, glm::vec3(1.0, 0.0, 0.0));
    parentTransform = glm::rotate(parentTransform, headZAngle, glm::vec3(0, 1.0, 0.0));
    parentTransform = glm::scale(parentTransform, glm::vec3(0.3, 0.3, 0.3));
    suzanneBoyle->setParentTransform(parentTransform);
    suzanneBoyle->setMaterial(Material::Chrome());

    float skiAngle =  0.85 * 3.14;
    glm::mat4 lSkiParentTransform = glm::translate(glm::mat4(), glm::vec3(1, -0.18, 2.3));
    glm::mat4 rSkiParentTransform = glm::translate(glm::mat4(), glm::vec3(1, -0.18, 2.3));
    lSkiParentTransform = glm::rotate(lSkiParentTransform, skiAngle, glm::vec3(0, 1.0, 0.0));
    rSkiParentTransform = glm::rotate(rSkiParentTransform, skiAngle, glm::vec3(0, 1.0, 0.0));
    lSkiRenderable->setParentTransform(lSkiParentTransform);
    rSkiRenderable->setParentTransform(rSkiParentTransform);

    float armAngle = 2.5 * 3.14 / 4.0;
    glm::mat4 lArmParentTransform = glm::translate(glm::mat4(), glm::vec3(-0.5, -0.8, 4.5));
    glm::mat4 rArmParentTransform = glm::translate(glm::mat4(), glm::vec3(-0.5, 0.8, 4.5));
    lArmParentTransform = glm::rotate(lArmParentTransform, armAngle, glm::vec3(0.3, 1.0, 0.0));
    rArmParentTransform = glm::rotate(rArmParentTransform, armAngle, glm::vec3(-0.3, 1.0, 0.0));
    lArmRenderable->setParentTransform(lArmParentTransform);
    rArmRenderable->setParentTransform(rArmParentTransform);

    float foreArmAngle = 3.14/10.0;
    glm::mat4 lForearmParentTransform = glm::rotate(glm::mat4(), -foreArmAngle, glm::vec3(1, 0, 0.0));
    glm::mat4 rForearmParentTransform = glm::rotate(glm::mat4(), foreArmAngle, glm::vec3(1, 0, 0.0));
    lForearmParentTransform = glm::translate(lForearmParentTransform, glm::vec3(0, -0.5, 1.2));
    rForearmParentTransform = glm::translate(rForearmParentTransform, glm::vec3(0, 0.5, 1.2));
    rForearmParentTransform = glm::scale(rForearmParentTransform, glm::vec3(0.8, 0.8, 0.6));
    lForearmParentTransform = glm::scale(lForearmParentTransform, glm::vec3(0.8, 0.8, 0.6));
    lForearmRenderable->setParentTransform(lForearmParentTransform);
    rForearmRenderable->setParentTransform(rForearmParentTransform);


    float tibiaAngle = 3.14/3.0;
    glm::mat4 lTibiaParentTransform = glm::scale(glm::mat4(), glm::vec3(0.8, 0.8, 1));
    glm::mat4 rTibiaParentTransform = glm::scale(glm::mat4(), glm::vec3(0.8, 0.8, 1));
    lTibiaParentTransform = glm::translate(lTibiaParentTransform, glm::vec3(-0.4, 0, 1.3));
    rTibiaParentTransform = glm::translate(rTibiaParentTransform, glm::vec3(-0.4, 0, 1.3));
    lTibiaParentTransform = glm::rotate(lTibiaParentTransform, tibiaAngle, glm::vec3(0, 1, 0.0));
    rTibiaParentTransform = glm::rotate(rTibiaParentTransform, tibiaAngle, glm::vec3(0, 1, 0.0));
    lTibiaRenderable->setParentTransform(lTibiaParentTransform);
    rTibiaRenderable->setParentTransform(rTibiaParentTransform);

    float StickAngle = 3.14/3.0;
    glm::mat4 lStickParentTransform = glm::scale(glm::mat4(), glm::vec3(0.8, 0.8, 1));
    glm::mat4 rStickParentTransform = glm::scale(glm::mat4(), glm::vec3(0.8, 0.8, 1));
    lStickParentTransform = glm::translate(lStickParentTransform, glm::vec3(0, 0, 1.3));
    rStickParentTransform = glm::translate(rStickParentTransform, glm::vec3(0, 0, 1.3));
    lStickParentTransform = glm::rotate(lStickParentTransform, StickAngle, glm::vec3(0.5, 1, 0.0));
    rStickParentTransform = glm::rotate(rStickParentTransform, StickAngle, glm::vec3(-0.5, 1, 0.0));
    lStickRenderable->setParentTransform(lStickParentTransform);
    rStickRenderable->setParentTransform(rStickParentTransform);


    HierarchicalRenderable::addChild(systemRenderable, mobileRenderable);
    HierarchicalRenderable::addChild(mobileRenderable, lArmRenderable);
    HierarchicalRenderable::addChild(mobileRenderable, rArmRenderable);
    HierarchicalRenderable::addChild(lArmRenderable, lForearmRenderable);
    HierarchicalRenderable::addChild(rArmRenderable, rForearmRenderable);
    HierarchicalRenderable::addChild(mobileRenderable, lThighRenderable);
    HierarchicalRenderable::addChild(mobileRenderable, rThighRenderable);
    HierarchicalRenderable::addChild(lThighRenderable, lTibiaRenderable);
    HierarchicalRenderable::addChild(rThighRenderable, rTibiaRenderable);
    HierarchicalRenderable::addChild(lTibiaRenderable, lSkiRenderable);
    HierarchicalRenderable::addChild(rTibiaRenderable, rSkiRenderable);
    HierarchicalRenderable::addChild(mobileRenderable, suzanneBoyle);
    HierarchicalRenderable::addChild(rForearmRenderable, rStickRenderable);
    HierarchicalRenderable::addChild(lForearmRenderable, lStickRenderable);
}


void practical07_playPool(Viewer& viewer, DynamicSystemPtr& system, DynamicSystemRenderablePtr& systemRenderable)
{
    //Initialize a shader for the following renderables
    ShaderProgramPtr flatShader
    = std::make_shared<ShaderProgram>("../shaders/flatVertex.glsl","../shaders/flatFragment.glsl");
    ShaderProgramPtr phongShader
    = std::make_shared<ShaderProgram>("../shaders/phongVertex.glsl", "../shaders/phongFragment.glsl");
    viewer.addShaderProgram(phongShader);
    viewer.addShaderProgram(flatShader);

    //Position the camera
    viewer.getCamera().setViewMatrix(
        glm::lookAt(glm::vec3(0, -15, 15), glm::vec3(0,0,0), glm::vec3(0,0,1)) );

    //Initialize two particles with position, velocity, mass and radius and add it to the system
    glm::vec3 px(0.0, 0.0, 0.0);
    glm::vec3 pv(0.0, 0.0, 0.0);
    float pm = 1.0, pr = 1.0;
    px = glm::vec3(0.0,0.0,1.0);
    ParticlePtr mobile = std::make_shared<Particle>( px, pv, pm, pr);
    system->addParticle( mobile );
    px = glm::vec3(0.0,5.0,1.0);
    ParticlePtr other = std::make_shared<Particle>( px, pv, pm, pr);
    system->addParticle( other );

    createSkier(flatShader, phongShader, mobile, systemRenderable);

    ParticleRenderablePtr otherRenderable = std::make_shared<ParticleRenderable>(flatShader, other);
    HierarchicalRenderable::addChild(systemRenderable, otherRenderable);

    //Initialize four planes to create walls arround the particles
    glm::vec3 planeNormal, planePoint;
    planeNormal = glm::vec3(-1, 0, 0);
    planePoint = glm::vec3(10, 0, 0);
    PlanePtr p0 = std::make_shared<Plane>(planeNormal, planePoint);
    system->addPlaneObstacle(p0);

    planeNormal = glm::vec3(1, 0, 0);
    planePoint = glm::vec3(-10, 0, 0);
    PlanePtr p1 = std::make_shared<Plane>(planeNormal, planePoint);
    system->addPlaneObstacle(p1);

    planeNormal = glm::vec3(0, -1, 0);
    planePoint = glm::vec3(0, 10, 0);
    PlanePtr p2 = std::make_shared<Plane>(planeNormal, planePoint);
    system->addPlaneObstacle(p2);

    planeNormal = glm::vec3(0, 1, 0);
    planePoint = glm::vec3(0, -10, 0);
    PlanePtr p3 = std::make_shared<Plane>(planeNormal, planePoint);
    system->addPlaneObstacle(p3);

    planeNormal = glm::vec3(0, 0, 1);
    planePoint = glm::vec3(0, 0, 0);
    PlanePtr floor = std::make_shared<Plane>( planeNormal, planePoint);
    system->addPlaneObstacle(floor);

    //Create  plane renderables to display each obstacle
    //Add them to the system renderable
    glm::vec3 x1, x2, x3, x4;
    glm::vec4 color;
    x1 = glm::vec3(10,  10, 5);
    x2 = glm::vec3(10,  10, 0);
    x3 = glm::vec3(10, -10, 0);
    x4 = glm::vec3(10, -10, 5);
    color = glm::vec4(0.4, 0.2, 0.2, 1.0);
    PlaneRenderablePtr p1Renderable = std::make_shared<QuadRenderable>(flatShader, x1, x2, x3, x4, color);
    HierarchicalRenderable::addChild(systemRenderable, p1Renderable);

    x1 = glm::vec3(-10, 10, 5);
    x2 = glm::vec3(-10, 10, 0);
    x3 = glm::vec3( 10, 10, 0);
    x4 = glm::vec3( 10, 10, 5);
    color = glm::vec4(0.4, 0.2, 0.2, 1.0);
    PlaneRenderablePtr p2Renderable = std::make_shared<QuadRenderable>(flatShader, x1, x2, x3, x4, color);
    HierarchicalRenderable::addChild(systemRenderable, p2Renderable);

    x1 = glm::vec3(-10, -10, 5);
    x2 = glm::vec3(-10, -10, 0);
    x3 = glm::vec3(-10,  10, 0);
    x4 = glm::vec3(-10,  10, 5);
    color = glm::vec4(0.2, 0.4, 0.4, 1.0);
    PlaneRenderablePtr p3Renderable = std::make_shared<QuadRenderable>(flatShader, x1, x2, x3, x4, color);
    HierarchicalRenderable::addChild(systemRenderable, p3Renderable);

    x1 = glm::vec3( 10, -10, 5);
    x2 = glm::vec3( 10, -10, 0);
    x3 = glm::vec3(-10, -10, 0);
    x4 = glm::vec3(-10, -10, 5);
    color = glm::vec4(0.2, 0.4, 0.4, 1.0);
    PlaneRenderablePtr p4Renderable = std::make_shared<QuadRenderable>(flatShader, x1, x2, x3, x4, color);
    HierarchicalRenderable::addChild(systemRenderable, p4Renderable);

    //Initialize a force field that apply only to the mobile particle
    glm::vec3 nullForce(0.0, 0.0, 0.0);
    std::vector<ParticlePtr> vParticle;
    vParticle.push_back(mobile);
    //vParticle.push_back(other); //added
    ConstantForceFieldPtr force = std::make_shared<ConstantForceField>(vParticle, nullForce);
    system->addForceField(force);

    //Initialize a renderable for the force field applied on the mobile particle.
    //This renderable allows to modify the attribute of the force by key/mouse events
    //Add this renderable to the systemRenderable.
    ControlledForceFieldRenderablePtr forceRenderable = std::make_shared<ControlledForceFieldRenderable>(flatShader, force);
    HierarchicalRenderable::addChild(systemRenderable, forceRenderable);

    //Add a damping force field to the mobile.
    DampingForceFieldPtr dampingForceField = std::make_shared<DampingForceField>(vParticle, 0.9);
    system->addForceField(dampingForceField);

    //Activate collision and set the restitution coefficient to 1.0
    system->setCollisionsDetection(true);
    system->setRestitution(1.0f);
}
