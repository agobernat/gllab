




#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <stdlib.h>
#include <time.h>

//#include <learnopengl/filesystem.h>
#include <shader_t.h>

#include <iostream>
#include "resourcemanager.hpp"
#include "macros.hpp"
#include "sprite.hpp"
#include "terrain.hpp"
#include "terraingen.hpp"
#include "camera.hpp"
#include "model.hpp"

#define STB_IMAGE_WRITE_IMPLEMENTATION
//#include "stb_image.h"
#define TINYGLTF_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#define STBI_MSC_SECURE_CRT
#include "tiny_gltf.h"




void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
void countfps();
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
//float* genheightmap(int size);
void mouse_callback_init(GLFWwindow* window, double xposIn, double yposIn);

Camera cam;
// settings



bool keys[1024];
glm::vec3 position;
float dts;
float prevt;
float start;

float currt;





int main()
{
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
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Engine v0.2", NULL, NULL);
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


    glEnable(GL_DEPTH_TEST);


    // build and compile our shader zprogram
    // ------------------------------------
    Shader ourShader("3d1st.vert", "5.1.transform.frag");
    Shader tesTerrainShader("terrain.vert", "terrain.frag", nullptr, "terrain.tesc", "terrain.tese");
    Shader treeShader("tree1.vert", "tree1.frag");
    //Shader ourShader("5.1.transform.vert", "5.1.transform.frag");
	position = glm::vec3(0, 0, 0);



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







    tinygltf::Model model;
    tinygltf::TinyGLTF loader;
    std::string err;
    std::string warn;
    std::string mdlpath("resources\\models\\cliff.gltf");
    GameModel tree;
    
    bool ret = loader.LoadASCIIFromFile(&model, &err, &warn, mdlpath.c_str());
    //bool ret = loader.LoadBinaryFromFile(&model, &err, &warn, argv[1]); // for binary glTF(.glb)
    
    if (!warn.empty()) {
        printf("Warn: %s\n", warn.c_str());
    }
    
    if (!err.empty()) {
        printf("Err: %s\n", err.c_str());
    }
    
    if (!ret) {
        printf("Failed to parse glTF\n");
        return -1;
    }


    std::cout << model.buffers.size();
    tree.vao = tree.bindModel(model);
   

 
    Sprite box = Sprite(ourShader);
    Sprite kid = Sprite(ourShader);

    cam = Camera(glm::vec3(0.0f, -5.0f, 4.0f),
                 glm::vec3(0.0f, 1.0f, 0.0f),
                 glm::vec3(0.0f, 0.0f, 1.0f));

    int hmapsize = 16;
    Terrain terrain(tesTerrainShader, hmapsize, 16, texture8, 6969);
    

    

    terrain.loadChunk(0, 0);
    terrain.loadChunk(-1, 0);
    terrain.loadChunk(0, -1);
    terrain.loadChunk(-1, -1);
    terrain.loadChunk(1, 0);
    terrain.loadChunk(-2, 0);
    //terrain.unloadChunk(0, 0);


    

	prevt = (float)glfwGetTime();
	start = prevt;
    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        currt = (float)glfwGetTime();
        processInput(window);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);



        treeShader.use();
        auto modelmat = tree.model_rot;
        glm::mat4 projection = glm::mat4(1.0f);
        float rot = 0.0f;
        //modelmat = glm::translate(modelmat, glm::vec3(-position.x, position.y, position.z));
        modelmat = glm::rotate(modelmat, glm::radians(rot), glm::vec3(1.0f, 0.0f, 0.0f));

        projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 10000.0f);
        
        treeShader.setMat4("projection", projection);
        treeShader.setMat4("view", cam.view());
        treeShader.setMat4("model", modelmat);

        tree.drawModel(tree.vao, model);






        
        kid.Draw(glm::vec3(-position.x * 2, position.y, position.z), texture2, cam.view());
        box.Draw(position, texture1, cam.view());
        kid.Draw(glm::vec3(-position.x, position.y, position.z), texture2, cam.view());
        
        
        terrain.draw(currt, cam);
        
        glfwSwapBuffers(window);

        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    //glDeleteVertexArrays(1, &VAO);
    //glDeleteBuffers(1, &VBO);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
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
		position.x += dts;
		
	}
	else if (keys[GLFW_KEY_RIGHT])
	{
		position.x -= dts;
	}
	
	if (keys[GLFW_KEY_UP])
	{
		position.y += dts;

	}
	else if (keys[GLFW_KEY_DOWN])
	{
		position.y -= dts;
	}

    float cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
        cameraSpeed = 250.0f * dts;
    else
    {
        cameraSpeed = 25.0f * dts;
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
        std::cout << "x:" << cam.lastX << ", y:" << cam.lastY << std::endl;
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

