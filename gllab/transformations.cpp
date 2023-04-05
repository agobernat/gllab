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
#include "collider.h"

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
void countfps();
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
//float* genheightmap(int size);
void mouse_callback_init(GLFWwindow* window, double xposIn, double yposIn);

Camera cam;
// settings


struct PlayerData {
    double xpos;
    double ypos;
    double vspeed;
    bool jumpheld = false;
    bool movingleft = false;
    bool movingright = false;
    int jumpamount = 0;
    glm::dvec2 playerstart;

    double gravity = 0.45;
    void tick(double dt) {
        ypos += vspeed;
        if (vspeed > 0 && jumpheld)
        {
            vspeed -= gravity * dt * 0.5;
        }
        else if (vspeed > -0.2) {
            vspeed -= gravity * dt;
        }

    }
    void jump() {
        if (jumpamount > 0)
        {
            vspeed = 0.12;
            jumpamount -= 1;
        }
        
    }
};

PlayerData playerdata;




bool keys[1024];

float dts;
float prevt;
float start;

float currt;

int main()
{
    playerdata.vspeed = 0;
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


    // build and compile our shader zprogram
    // ------------------------------------
    auto& shaderManager = Globals::shaderManager;
    shaderManager.loadShader("box", "3d1st.vert", "5.1.transform.frag");
    shaderManager.loadShader("terrain", "terrain.vert", "terrain.frag", nullptr, "terrain.tesc", "terrain.tese");
    shaderManager.loadShader("defaultmodel", "tree1.vert", "tree1.frag");
    shaderManager.setDefaultShader("defaultmodel");
    shaderManager.setMaterial("defaultmodel", "Material");

    playerdata.xpos = 0;
    playerdata.ypos = 0;


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
    std::string mdlpath3("resources\\models\\needle.gltf");
    std::string mdlpath2("resources\\models\\guyatt2fix.gltf");
    std::string mdlpath("resources\\models\\block.gltf");
    LevelLoader loader;
    auto level = loader.loadFromFile("level1.csv");


    btDefaultCollisionConfiguration* collisionConfiguration = new
        btDefaultCollisionConfiguration();
    btCollisionDispatcher* dispatcher = new btCollisionDispatcher(collisionConfiguration);
    btBroadphaseInterface* overlappingPairCache = new btDbvtBroadphase();
    btSequentialImpulseConstraintSolver* solver = new btSequentialImpulseConstraintSolver;
    btDiscreteDynamicsWorld * dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher,
        overlappingPairCache, solver, collisionConfiguration);

    //btCollisionShape* groundShape = new btBoxShape(btVector3(btScalar(50.), btScalar(50.), btScalar(50.)));
    //btTransform groundTransform;
    //groundTransform.setIdentity();
    //groundTransform.setOrigin(btVector3(0, -50, 0));
    //btScalar mass(0.);
    //btVector3 localInertia(0, 0, 0);
    //btDefaultMotionState* myMotionState = new btDefaultMotionState(groundTransform);
    //btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, groundShape, localInertia);
    //btRigidBody* body = new btRigidBody(rbInfo);
    //dynamicsWorld->addRigidBody(body);




    GameModel box;
    box.loadFromFile(mdlpath);
    box.bind();
    GameModel kidmodel;
    kidmodel.loadFromFile(mdlpath2);
    kidmodel.bind();
    GameModel spikemodel;
    spikemodel.loadFromFile(mdlpath3);
    spikemodel.bind();
    //GameObject kid(kidmodel);

    //box2.scale(glm::dvec3(0.5, 0.5, 0.5));
    //box2.translate(glm::dvec3(2.0, 0.0, 0.0));
    auto boxes = std::vector<GameObject*>();
    auto colliders = std::vector<Collider*>();
    auto blocks = std::vector<Collider*>();
   
    //Collider* kidcollider = nullptr;
    
    //auto colliders = std::vector<>();


    
    boxes.reserve(20);
    GameObject* kidspritetest = nullptr;





    for (size_t i = 0; i < level.size(); i++)
    {
        for (size_t j = 0; j < level[i].size(); j++)
        {
            auto tile = level[i][j];

            

            std::bitset<32> btst = tile;
            std::cout << btst;
            std::cout << std::endl;
            /*
            if (tile == 1)
            {
                GameObject* kid = new GameObject(kidmodel);
                playerdata.playerstart = glm::dvec2(static_cast<float>(j), -static_cast<float>(i));
                kid->translate(glm::dvec3(static_cast<float>(j), -static_cast<float>(i), 0.0f));
                playerdata.xpos = static_cast<double>(j);
                playerdata.ypos = -static_cast<double>(i);
                kid->scale(glm::dvec3(0.5, 0.5, 0.5));
                //kid->objecttype = "kid";
                boxes.push_back(kid);
                kidspritetest = kid;

                auto coll = new Collider();
                coll->bottomleft = glm::vec2(0.2, 0.0);
                coll->topright = glm::vec2(0.9, 1.0);
                coll->moveTo(glm::dvec2(static_cast<float>(j), -static_cast<float>(i)));
                coll->type = Collider::Kid;
                
                kid->collider = coll;
                

            }
            else if (tile == 2) {
                GameObject* block = new GameObject(box);
                block->translate(glm::dvec3(static_cast<float>(j), -static_cast<float>(i), 0.0f));
                block->scale(glm::dvec3(0.5, 0.5, 0.5));
                //block->objecttype = "block";
                boxes.push_back(block);
                
                auto coll = Collider::unitBox();
                coll->moveTo(glm::dvec2(static_cast<float>(j), -static_cast<float>(i)));
                coll->type = Collider::Block;
                colliders.push_back(coll);
                block->collider = colliders[colliders.size() - 1];
                blocks.push_back(block->collider);

            }
            else if (tile >= 3) {

                GameObject* spike = new GameObject(spikemodel);
                spike->translate(glm::dvec3(static_cast<float>(j), -static_cast<float>(i), 0.0f));
                spike->scale(glm::dvec3(0.5, 0.5, 0.5));
                

                if (tile == 4) {
                    spike->rotate(1.0 / 2.0 * 3.1415f, glm::dvec3(0.0, 0.0, 1.0));
                    
                }
                else if (tile == 5)
                {
                    spike->rotate(3.1415f, glm::dvec3(0.0, 0.0, 1.0));
                    
                }
                else if (tile == 6)
                {
                    spike->rotate(3.0 / 2.0 * 3.1415f, glm::dvec3(0.0, 0.0, 1.0));
                    
                }
                spike->objecttype = "spike";
                boxes.push_back(spike);

                auto coll = Collider::unitBox();
                coll->moveTo(glm::dvec2(static_cast<float>(j), -static_cast<float>(i)));
                coll->type = Collider::Spike;
                colliders.push_back(coll);
                spike->collider = colliders[colliders.size() - 1];
            }*/
            
            
        }
    }

    

    /*for (size_t i = 0; i < 20; i++)
    {
        boxes.push_back(GameObject(box));
        boxes[i].scale(glm::dvec3(0.5, 0.5, 0.5));
        boxes[i].translate(glm::dvec3(1.0 * i, 0.0, 0.0));
    }*/


    cam = Camera(glm::vec3(0.0f, 4.0f, 12.0f),
                 glm::vec3(0.0f, 0.0f, - 1.0f),
                 glm::vec3(0.0f, 1.0f, 0.0f));

    int hmapsize = 16;
    Terrain terrain(shaderManager.getShader("terrain"), hmapsize, 16, texture8, 6969);
    

    

    terrain.loadChunk(0, 0);
    terrain.loadChunk(-1, 0);
    terrain.loadChunk(0, -1);
    terrain.loadChunk(-1, -1);
    terrain.loadChunk(1, 0);
    terrain.loadChunk(-2, 0);
    //terrain.unloadChunk(0, 0);

    std::vector<std::pair<int, int>> loaded;
    //loaded.push_back(std::pair(0, 0));
    loaded.push_back(std::pair(-1, 0));
    loaded.push_back(std::pair(0, -1));
    loaded.push_back(std::pair(-1, -1));
    loaded.push_back(std::pair(1, 0));
    loaded.push_back(std::pair(-2, 0));
    





	prevt = (float)glfwGetTime();
    float delaytime;
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
            
            kidspritetest->collider->printbounds();
            
            std::cout << "\n";
           
        }

        //dynamicsWorld->stepSimulation(delaytime, 10);

        for (const auto& item : colliders)
        {
            if (kidspritetest->collider->collWithOther(*item))
            {
                if (item->type == Collider::Spike)
                {
                    playerdata.xpos = playerdata.playerstart.x;
                    playerdata.ypos = playerdata.playerstart.y;
                    playerdata.vspeed = 0;
                    std::cout << "death";
                }
                else if (playerdata.vspeed > 0)
                {
                    playerdata.ypos = item->bottomleft.y + item->transform.y - 1;
                }
                else if (playerdata.vspeed < 0)
                {
                    playerdata.jumpamount = 2;
                    playerdata.ypos = item->topright.y + item->transform.y;
                }

                playerdata.vspeed = 0;
            }

        }

        if (playerdata.movingright) {
            kidspritetest->setRotation(0, glm::dvec3(0.0, 1.0, 0.0));
        }
        if (playerdata.movingleft) {
            kidspritetest->setRotation(3.1415, glm::dvec3(0.0, 1.0, 0.0));
        }

        processInput(window);
        cam.pos = glm::vec3(playerdata.xpos, playerdata.ypos, cam.pos.z);
        auto translatetest = kidspritetest->getTransformVec();
        kidspritetest->setTranslate(glm::dvec3(playerdata.xpos, playerdata.ypos, translatetest.z));
        kidspritetest->collider->moveTo(glm::dvec2(playerdata.xpos, playerdata.ypos));

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        for (const auto& obj : boxes) {
            obj->draw(cam);
        }
        

        

        

        //kid.draw(cam);

        //terrain.draw(currt, cam);
        
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
        playerdata.movingleft = true;
        playerdata.movingright = false;
        playerdata.xpos -= dts * 50 * 3 / 32;
		
	}
	else if (keys[GLFW_KEY_RIGHT])
	{
        playerdata.movingleft = false;
        playerdata.movingright = true;
        playerdata.xpos += dts * 50 * 3 / 32;
	}
    if (!keys[GLFW_KEY_LEFT])
    {
        playerdata.movingleft = false;
    }
    if (!keys[GLFW_KEY_RIGHT])
    {
        playerdata.movingright = false;
    }
	
	//if (keys[GLFW_KEY_UP])
	//{
    //    playerdata.ypos += dts * 50 * 3 / 32;
    //
	//}
	//else if (keys[GLFW_KEY_DOWN])
	//{
    //    playerdata.ypos -= dts * 50 * 3 / 32;
	//}

    if (keys[GLFW_KEY_C] && !playerdata.jumpheld)
    {
        playerdata.jumpheld = true;
        playerdata.jump();
    }
    else if (!keys[GLFW_KEY_C])
    {
        playerdata.jumpheld = false;
    }

    if (keys[GLFW_KEY_R])
    {
        playerdata.xpos = playerdata.playerstart.x;
        playerdata.ypos = playerdata.playerstart.y;
        playerdata.vspeed = 0;
        cam = Camera(glm::vec3(0.0f, 4.0f, 12.0f),
            glm::vec3(0.0f, 0.0f, -1.0f),
            glm::vec3(0.0f, 1.0f, 0.0f));
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
    if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
        std::cout << cam.front.x << ", " << cam.front.y << ", " << cam.front.z << std::endl;
    
    playerdata.tick(dts);
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


