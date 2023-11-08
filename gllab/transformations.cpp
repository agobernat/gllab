#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <stdlib.h>
#include <time.h>
#include <bitset>


#define TINYGLTF_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#define STBI_MSC_SECURE_CRT

#include <tiny_gltf.h>

#undef TINYGLTF_IMPLEMENTATION
#undef STB_IMAGE_IMPLEMENTATION
#undef STB_IMAGE_WRITE_IMPLEMENTATION
#undef STBI_MSC_SECURE_CRT

#include "bullet/btBulletDynamicsCommon.h"
#include "bullet/BulletDynamics/ConstraintSolver/btNNCGConstraintSolver.h"


#include "model.hpp"
//#include <learnopengl/filesystem.h>
#include <shader_t.h>
#include "shadermanager.hpp"
#include <iostream>
#include "resourcemanager.hpp"
#include "sprite.hpp"
#include "terrain.hpp"
#include "terraingen.hpp"
#include "camera.hpp"
#include "levelloader.hpp"
#include "static.hpp"
#include "globals.hpp"

#include "debugdrawer.hpp"

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
void countfps();
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
//float* genheightmap(int size);
void mouse_callback_init(GLFWwindow* window, double xposIn, double yposIn);

Camera cam;
// settings
bool triggered = false;

struct rCallBack : public btCollisionWorld::ContactResultCallback
{
    inline virtual btScalar addSingleResult(btManifoldPoint& cp, const btCollisionObjectWrapper* colObj0Wrap,
        int partId0, int index0, const btCollisionObjectWrapper* colObj1Wrap, int partId1, int index1)
    {
        
        auto obj0 = static_cast<btCollisionObject*>(colObj0Wrap->getCollisionObject()->getUserPointer());
        auto obj1 = static_cast<btCollisionObject*>(colObj1Wrap->getCollisionObject()->getUserPointer());
        
        auto userindex0 = obj0->getUserIndex();
        auto userindex1 = obj1->getUserIndex();
        if (userindex0 == 5 && userindex1 == 6 || userindex0 == 6 && userindex1 == 5)
        {
            triggered = true;
        }

        if (!obj0->isStaticOrKinematicObject() && obj1->isStaticOrKinematicObject())
        {
            auto& wt = obj0->getWorldTransform();
            auto origin = wt.getOrigin();
            if (cp.getDistance() < 0.0001f)
            {
                wt.setOrigin(origin + abs(cp.getDistance()) * cp.m_normalWorldOnB * 1.0001);
            }
        }
        if (!obj1->isStaticOrKinematicObject() && obj0->isStaticOrKinematicObject())
        {
            auto& wt = obj1->getWorldTransform();
            auto origin = wt.getOrigin(); 
            if (cp.getDistance() < 0.0001f)
            {
                wt.setOrigin(origin + abs(cp.getDistance()) * -cp.m_normalWorldOnB * 1.0001);
            }
        }
        return 0;
    }
};

struct pbCallback : public btCollisionWorld::ContactResultCallback
{
    inline virtual btScalar addSingleResult(btManifoldPoint& cp, const btCollisionObjectWrapper* colObj0Wrap,
        int partId0, int index0, const btCollisionObjectWrapper* colObj1Wrap, int partId1, int index1)
    {
        auto obj0 = static_cast<btRigidBody*>(colObj0Wrap->getCollisionObject()->getUserPointer());
        auto obj1 = static_cast<btCollisionObject*>(colObj1Wrap->getCollisionObject()->getUserPointer());

        auto userindex0 = obj0->getUserIndex();
        auto userindex1 = obj1->getUserIndex();
        if (userindex0 != 6)
        {
            return 0;
        }

        if (obj1->isStaticOrKinematicObject())
        {
            auto& wt = obj0->getWorldTransform();
            auto origin = wt.getOrigin();
            auto mvvec = obj0->getWorldTransform().getOrigin() - obj1->getWorldTransform().getOrigin();
            mvvec.normalize();
            const auto dist = cp.getDistance();
            std::cout << dist << '\n';
            if (dist < 0.001f)
            {
                obj0->applyCentralImpulse(0.002 * cp.m_normalWorldOnB);
                //wt.setOrigin(origin + abs(cp.getDistance()) * mvvec * 1.0001);
            }
            else
            {
                obj0->applyCentralImpulse(abs(dist) * cp.m_normalWorldOnB * 0.025);
            }
            
        }
        return 0;
    }
};


bool keys[1024];

double dts;
double prevt;
double start;

double currt;
bool flipped;
bool debugdraw;
bool xpressed;
bool zpressed;

GameObject* kidsprite;
btDiscreteDynamicsWorld* dynamicsWorld;

int main()
{

    debugdraw = false;
    flipped = false;

    xpressed = false;
    zpressed = false;
   
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//glfwWindowHint(GLFW_DOUBLEBUFFER, GL_TRUE);

    srand(time(NULL));

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(Static::SCR_WIDTH, Static::SCR_HEIGHT, "Engine v0.1", NULL, NULL);
	//GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Engine v0.1", glfwGetPrimaryMonitor(), NULL);
	if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetKeyCallback(window, key_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, mouse_callback_init);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    Static::init();

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


    // build and compile our shader zprogram
    // ------------------------------------
    auto& shaderManager = Globals::shaderManager;
    shaderManager.loadShader("box", "3d1st.vert", "5.1.transform.frag");
    shaderManager.loadShader("terrain", "terrain.vert", "terrain.frag", nullptr, "terrain.tesc", "terrain.tese");
    shaderManager.loadShader("defaultmodel", "tree1.vert", "tree1.frag");
    shaderManager.loadShader("background", "tree1.vert", "transparent.frag");
    shaderManager.loadShader("trigger", "tree1.vert", "colorshader.frag");
    shaderManager.setDefaultShader("defaultmodel");
    shaderManager.setMaterial("defaultmodel", "Material");
    shaderManager.setMaterial("background", "Material2");
    shaderManager.setMaterial("trigger", "Material3");



    // load and create a texture 
    // -------------------------
    unsigned int texture1, texture2, texture3, texture4, texture5, texture6;
    // texture 1
    // ---------
	//unsigned char *data;
	texture1 = ResourceManager::LoadTexture("resources\\textures\\container.jpg", GL_RGB, GL_RGB, GL_NEAREST, GL_NEAREST);
    texture2 = ResourceManager::LoadTexture("resources\\textures\\kid.png", GL_RGBA, GL_RGBA, GL_NEAREST, GL_NEAREST);
    texture3 = ResourceManager::LoadTexture("resources\\textures\\bricks2.jpg", GL_RGB, GL_RGB, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR_MIPMAP_LINEAR);
    texture4 = ResourceManager::LoadTexture("resources\\textures\\unknown67.png", GL_RGB, GL_RGBA, GL_LINEAR, GL_LINEAR_MIPMAP_LINEAR);
    texture5 = ResourceManager::LoadTexture("resources\\textures\\metal.png", GL_RGB, GL_RGB, GL_LINEAR, GL_LINEAR_MIPMAP_LINEAR);
    texture6 = ResourceManager::LoadTexture("resources\\textures\\blank.png", GL_RGB, GL_RGB, GL_LINEAR, GL_LINEAR_MIPMAP_LINEAR);
    unsigned int texture8 = ResourceManager::LoadTexture("resources\\textures\\pbr\\grass\\albedo.png", GL_RGB, GL_RGB, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR_MIPMAP_LINEAR);
    unsigned int texture7 = ResourceManager::LoadTexture("resources\\textures\\iceland_heightmap.png", GL_RGB, GL_RGB, GL_LINEAR, GL_LINEAR_MIPMAP_LINEAR);


    //std::string mdlpath("resources\\models\\Tree_01.gltf");
    //std::string mdlpath("resources\\models\\rivercorner.gltf");
    //std::string mdlpath("resources\\models\\Cube.gltf");
    //std::string mdlpath("resources\\models\\tent.gltf");
    //std::string mdlpath("resources\\models\\platform.gltf");
    std::string mdlpath4("resources\\models\\pushblock.gltf");
    std::string mdlpath5("resources\\models\\trigger.gltf");
    std::string mdlpath3("resources\\models\\needle.gltf");
    std::string mdlpath2("resources\\models\\guyatt2fix.gltf");
    std::string mdlpath("resources\\models\\block.gltf");
    std::string defaultblockpath("resources\\models\\defaultblock.gltf");
    LevelLoader loader;
    auto level = loader.loadFromFile("level01.csv");


    btDefaultCollisionConfiguration* collisionConfiguration = new
        btDefaultCollisionConfiguration();
    btCollisionDispatcher* dispatcher = new btCollisionDispatcher(collisionConfiguration);
    btBroadphaseInterface* overlappingPairCache = new btDbvtBroadphase();
    
    
   
    btSequentialImpulseConstraintSolver* solver = new btSequentialImpulseConstraintSolver;
    //btSequentialImpulseConstraintSolver* solver = new btNNCGConstraintSolver;
    
    dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher,
        overlappingPairCache, solver, collisionConfiguration);

    dynamicsWorld->setGravity(btVector3(0., -10.0, 0.));
    

    //btCollisionShape* meshshape = new btBvhTriangleMeshShape();

    //btCollisionShape* groundShape = new btBoxShape(btVector3(btScalar(1.), btScalar(1.), btScalar(1.)));
    //btTransform groundTransform;
    //groundTransform.setIdentity();
    //groundTransform.setOrigin(btVector3(0, -1, 0));
    //
    //btScalar mass(0.);
    //btVector3 localInertia(0, 0, 0);
    //btDefaultMotionState* myMotionState = new btDefaultMotionState(groundTransform);
    //btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, groundShape, localInertia);
    //btRigidBody* body = new btRigidBody(rbInfo);
    //dynamicsWorld->addRigidBody(body);

    

    DebugDrawer debugDrawer;
    dynamicsWorld->setDebugDrawer(&debugDrawer);
    dynamicsWorld->getDebugDrawer()->setDebugMode(btIDebugDraw::DBG_DrawWireframe);


    GameModel box;
    box.loadFromFile(mdlpath);
    box.bind();
    box.setColliderFromMesh();
    GameModel kidmodel;
    kidmodel.loadFromFile(mdlpath2);
    kidmodel.bind();
    kidmodel.setCustomCollider(std::make_unique<btCapsuleShape>(0.25, 0.5));
    GameModel spikemodel;
    spikemodel.loadFromFile(mdlpath3);
    spikemodel.bind();
    spikemodel.setCustomCollider(std::make_unique<btBoxShape>(btVector3{ 0.5, 0.5, 0.5 }));

    GameModel trigger;
    trigger.loadFromFile(mdlpath5);
    trigger.bind();
    trigger.setCustomCollider(std::make_unique<btBoxShape>(btVector3{ 0.45, 0.45, 0.45 }));
    
    

    GameModel defaultblockmodel;
    
    defaultblockmodel.loadFromFile(defaultblockpath);
    defaultblockmodel.bind();
    defaultblockmodel.setCustomCollider(btVector3(12.5, 9.5, 0.5));
    

    GameModel pushblockmodel;
    pushblockmodel.loadFromFile(mdlpath4);
    pushblockmodel.bind();
    pushblockmodel.setCustomCollider(std::make_unique<btBoxShape>(btVector3{ 0.475, 0.475, 0.475 }));
    

    auto boxes = std::vector<GameObject*>();

    GameObject* background = new GameObject(defaultblockmodel);
    boxes.push_back(background);
    
    background->normalizeSize();
    background->getTransform().scale(glm::vec3(25., 19., 1.));
    background->getTransform().translate(glm::vec3(12., 21., -1.0));
    background->setCustomCollider(Transform::glmTobtVec3(background->getTransform().getTranslate()), btScalar(0.0), btScalar(0.0));
    background->addColliderToDynamicsWorld(dynamicsWorld);
    
    boxes.reserve(20);
    kidsprite = nullptr;

    std::vector<GameObject*> pushblocks;
    std::vector<GameObject*> triggers;


    for (size_t i = 0; i < level.size(); i++)
    {
        GameObject* object = nullptr;
        switch (level[i].second)
        {
        case(0):
            object = new GameObject(box);
            
            break;
        case(1):
            object = new GameObject(spikemodel);
            break; 
        case(2):
            object = new GameObject(kidmodel);
            kidsprite = object;
            break;
        case(4):
            object = new GameObject(pushblockmodel);
            pushblocks.push_back(object);
            
            break;
        case(5):
            object = new GameObject(trigger);
            object->triggercolor = { 0, 0.7, 0.7, 0.6 };
            //object->getTransform().scale({ 1.1, 1.1, 1.1 });
            break;
        default:
            break;
        }
        object->setTransform(level[i].first);
        if (level[i].second == 2)
        {
            
            object->setCustomCollider(Transform::glmTobtVec3(object->getTransform().getTranslate()), btScalar(0.1), btScalar(0.0));
        }
        else if (level[i].second == 4)
        {
            object->setCustomCollider(Transform::glmTobtVec3(object->getTransform().getTranslate()), btScalar(0.1), btScalar(2.0));
            
        }
        else if (level[i].second == 5) {
            object->setCustomTriggerCollider(Transform::glmTobtVec3(object->getTransform().getTranslate()));
        }
        else
        {
            object->setBoxColliderFromMesh();
        }
        //if (level[i].second == 1)
        //{
        //    object->getTransform().scale({ 5., 5., 5. });
        //    object->getTransform().translate({ 0., -0.3, 0. });
        //}
        
        object->normalizeSize();
        object->addColliderToDynamicsWorld(dynamicsWorld);
        if (level[i].second == 4) {
            object->setUserIndex(6);
        }
        if (level[i].second == 5)
        {
            object->getTransform().scale({ 1.2, 1.2, 1.2 });
        }
        if (level[i].second != 5)
        {
            boxes.push_back(object);
        }
        else
        {
            triggers.push_back(object);
        }
        
        
    }
    //auto trig1 = new GameObject(trigger);
    //trig1->setCustomTriggerCollider(Transform::glmTobtVec3(boxes[5]->getTransform().getTranslate()));
    //triggers.push_back(trig1);
    //trig1->triggercolor = { 0, 0.7, 0.7, 0.6 };
    //trig1->addColliderToDynamicsWorld(dynamicsWorld);

    cam = Camera::getDefault();

    int hmapsize = 16;
    Terrain terrain(shaderManager.getShader("terrain"), hmapsize, 16, texture8, 6969);
    
    
    

    //terrain.loadChunk(0, 0);
    terrain.loadChunk(-1, 0);
    //terrain.loadChunk(0, -1);
    terrain.loadChunk(-1, -1);
    //terrain.loadChunk(1, 0);
    //terrain.loadChunk(-2, 0);
    //terrain.unloadChunk(0, 0);

    std::vector<std::pair<int, int>> loaded;
    //loaded.push_back(std::pair(0, 0));
    loaded.push_back(std::pair(-1, 0));
    //loaded.push_back(std::pair(0, -1));
    //loaded.push_back(std::pair(-1, -1));
    loaded.push_back(std::pair(1, 0));
    //loaded.push_back(std::pair(-2, 0));
    terrain.setPos(glm::dvec3( 0., -20., -3.));



	prevt = (float)glfwGetTime();
    float delaytime;
    float dt = 0.;
    delaytime = prevt;
	start = prevt;
    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        currt = (float)glfwGetTime();
        
        //delaytime = (float)glfwGetTime();
        //std::cout << currt << " " << delaytime << "\n";
        if (currt - delaytime > 0.2f) {
            delaytime = currt;
            
            std::cout << "\n";
            //std::cout << dynamicsWorld->getNumCollisionObjects() << std::endl;
           
        }


        

        processInput(window);
        

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        
        terrain.draw(currt, cam);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        terrain.move(glm::vec3(10 * dts, 0., 0.));
        

        for (const auto& obj : boxes) {
            obj->draw(cam);
        }
        for (const auto& obj: triggers)
        {
            obj->draw(cam);
        }
        cam.tick(dts);
        //kid.draw(cam);

        
        
        glClear(GL_DEPTH_BUFFER_BIT);
        debugDrawer.setMVP(glm::mat4(1.0), cam.view(), cam.projection());
        
        
        rCallBack rcallback;
        pbCallback pbcallback;
        dynamicsWorld->contactTest(kidsprite->getCollisionBody(), rcallback);
        
        for (auto& trigger : triggers) {
            dynamicsWorld->contactTest(trigger->getCollisionBody(), rcallback);
            if (triggered)
            {
                trigger->blocktimer = 0.1;
                triggered = false;
            }
        }
        for (auto& pb : pushblocks)
        {
            dynamicsWorld->contactTest(pb->getCollisionBody(), pbcallback);
            btVector3 vectest{ 1., 0., 0. };
            vectest = vectest.rotate({ 0., 0., 1. }, (rand() % 100) * 3.1415 * 2 / 100);
            std::cout << vectest.x() << " " << vectest.y() << " " << vectest.z() << "\n";
            //static_cast<btRigidBody*>(pb->getCollisionBody())->applyCentralImpulse(vectest * 0.006);
        }
        dynamicsWorld->stepSimulation(dts, 1, 1.0 / 50);
        for (auto& trigger : triggers) 
        {
            if (trigger->blocktimer > 0)
            {
                trigger->triggercolor = { 0.1, 0.9, 0.2, 0.6 };
                trigger->blocktimer -= dts;
            }
            else
            {
                trigger->triggercolor = { 0.6, 0.6, 0.1, 0.6 };
                trigger->blocktimer = 0;
            }

        }
       
        
        //dynamicsWorld->
        //dynamicsWorld->applyGravity();
        
        for (const auto& box : boxes) {
            if (!box->isKinematic())
            {
                box->updateGraphicsTransformFromPhysics();
            }
        }
        if (debugdraw)
        {
            dynamicsWorld->debugDrawWorld();
        }
        for (auto& pushblock : pushblocks) {
            const auto grav = dynamicsWorld->getGravity().normalized().absolute();
            const auto vel = pushblock->getVelocity();
            pushblock->setVelocity(glm::vec3(vel.x * grav.x(), vel.y * grav.y(), vel.z * grav.z()));
        }
        
        

        glfwSwapBuffers(window);

        glfwPollEvents();
    }


    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------

void processInput(GLFWwindow *window)
{
	
	dts = currt - prevt;
	prevt = currt;
    

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

	if (keys[GLFW_KEY_ESCAPE])
		glfwSetWindowShouldClose(window, true);

	if (keys[GLFW_KEY_LEFT])
	{
        auto vel = kidsprite->getVelocity();
        vel.x = -4.5;
        kidsprite->setVelocity(vel);
        if (!flipped)
        {
            kidsprite->getTransform().setRotation(3.1415, {0., 1., 0. });
        }
        else
        {
            kidsprite->getTransform().setRotation(-3.1415 / 2, { -1., 0., 0. });
            kidsprite->getTransform().rotate(3.1415, { 0., 1., 0. });
        }
        
	}
	else if (keys[GLFW_KEY_RIGHT])
	{
        auto vel = kidsprite->getVelocity();
        vel.x = 4.5;
        kidsprite->setVelocity(vel);

        if (!flipped)
        {
            kidsprite->getTransform().setRotation(0, { 0., 1., 0. });
        }
        else
        {
            kidsprite->getTransform().setRotation(-3.1415 / 2, { -1., 0., 0. });
        }
	}
    else
    {
        auto vel = kidsprite->getVelocity();
        vel.x = 0.0;
        kidsprite->setVelocity(vel);
    }
    
    if (keys[GLFW_KEY_UP])
    {
        if (flipped)
        {
            auto vel = kidsprite->getVelocity();
            vel.y = 4.5;
            kidsprite->setVelocity(vel);
            kidsprite->getTransform().setRotation(-3.1415 / 2, { -1., 0., 0. });
            kidsprite->getTransform().rotate(3.1415 / 2, { 0., 1., 0. });
            
        }
        
    }
    else if (keys[GLFW_KEY_DOWN])
    {
        if (flipped)
        {
            auto vel = kidsprite->getVelocity();
            vel.y = -4.5;
            kidsprite->setVelocity(vel);
            kidsprite->getTransform().setRotation(-3.1415 / 2, { -1., 0., 0. });
            kidsprite->getTransform().rotate(-3.1415 / 2, { 0., 1., 0. });
        }

    }
    else
    {
        if (flipped)
        {
            auto vel = kidsprite->getVelocity();
            vel.y = 0.0;
            kidsprite->setVelocity(vel);
        }

    }


    if (keys[GLFW_KEY_C])
    {
        if (!flipped)
        {
            auto vel = kidsprite->getVelocity();
            vel.y = 8.0;
            kidsprite->setVelocity(vel);
        }
       
        
    }
    else if (!keys[GLFW_KEY_C])
    {

    }

    if (keys[GLFW_KEY_X])
    {
        if (!xpressed)
        {
            debugdraw = !debugdraw;
        }
        xpressed = true;
    }
    else 
    {
        xpressed = false;
    }

    if (keys[GLFW_KEY_Z])
    {
        if (!zpressed)
        {
            if (flipped)
            {
                cam.linearMove(glm::vec3(12.0f, 20.0f, 32.0f), glm::vec3(0.0f, 0.0f, -1.0f), 1.);
                dynamicsWorld->setGravity(btVector3(0., -10., 0.));
                kidsprite->getTransform().setRotation(glm::dmat4(1.));
            }
            else
            {
                cam.linearMove(glm::vec3(12.0f, 8.0f, 15.0f), glm::vec3(0.0f, 0.7f, -1.0f), 1.);
                dynamicsWorld->setGravity(btVector3(0., 0., -10.));
                kidsprite->getTransform().setRotation(-3.1415 / 2, { -1., 0., 0. });
            }
            flipped = !flipped;
        }
        zpressed = true;
        
    }
    else
    {
        zpressed = false;
    }

    if (keys[GLFW_KEY_R])
    {
        cam = Camera::getDefault();
    }

    float cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
        cameraSpeed = 12.5f * dts;
    else
    {
        cameraSpeed = 2.50f * dts;
    }// adjust accordingly
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cam.pos += cameraSpeed * cam.front;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cam.pos -= cameraSpeed * cam.front;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cam.pos -= glm::normalize(glm::cross(cam.front, cam.up)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cam.pos += glm::normalize(glm::cross(cam.front, cam.up)) * cameraSpeed;
        
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        cam.pos += cameraSpeed * cam.up;
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        cam.pos -= cameraSpeed * cam.up;
    
	countfps();

}


void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	// when a user presses the escape key, we set the WindowShouldClose property to true, closing the application
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
			keys[key] = true;
		else if (action == GLFW_RELEASE)
		{
			keys[key] = false;
		}
	}
}

int fpscounter = 0;
void countfps() 
{
	if (currt - start > 1.0f)
	{
        //std::cout << "position:" << position.x << std::endl;
		//std::cout << fpscounter + 1 << " fps" << std::endl;
        //std::cout << "x:" << cam.lastX << ", y:" << cam.lastY << std::endl;
        //std::cout << glfwGetTime();

        
		fpscounter = 0;
		start = currt;
	}
	else
	{
		fpscounter += 1;
	}
	
}

void mouse_callback_init(GLFWwindow* window, double xposIn, double yposIn) {
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);
    cam.lastX = xpos;
    cam.lastY = ypos;


    glfwSetCursorPosCallback(window, mouse_callback);
}

void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);
    
    cam.move(xpos, ypos);
    
}


// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}


/*
for (size_t i = 0; i < model.buffers[0].data.size(); i += 4)
{
    if (i % 12 == 0)
    {
        std::cout << std::endl;
    }
    std::cout << *reinterpret_cast<float*>(&model.buffers[0].data.at(i)) << ", ";

}*/


