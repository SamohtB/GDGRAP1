#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <cmath>

#include "Model3D.h"
#include "main.h"

//Camera LookAt and Camera Position
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 10.0f);
glm::vec3 cameraCenter = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 worldUp = glm::normalize(glm::vec3(0.0f, 1.0f, 0.0f));    //World's Up

bool firstMouse = true;
float lastX = 300.0f;
float lastY = 300.0f;
float yaw = -90.0f;
float pitch = 0.0f;

float previousTime = 0.0f;
float deltaTime = 0.0f;

void Key_Callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    float movementSpeed = 200.0f;
    glm::vec3 cameraFront = glm::normalize(cameraCenter - cameraPos);
    glm::vec3 cameraRight = glm::normalize(glm::cross(cameraFront, worldUp));


    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_W) == GLFW_REPEAT)
    {
        cameraPos += movementSpeed * deltaTime * cameraFront;
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_S) == GLFW_REPEAT)
    {
        cameraPos -= movementSpeed * deltaTime * cameraFront;
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_A) == GLFW_REPEAT)
    {
        cameraPos -= movementSpeed * deltaTime * cameraRight;
        cameraCenter -= movementSpeed * deltaTime * cameraRight;
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_D) == GLFW_REPEAT)
    {
        cameraPos += movementSpeed * deltaTime * cameraRight;
        cameraCenter += movementSpeed * deltaTime * cameraRight;
    }
}

/* Mouse Function for rotation */
void Mouse_Callback(GLFWwindow* window, double xpos, double ypos)
{
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_1) == GLFW_RELEASE) { firstMouse = true;  return; }
    float xPos = (float)xpos;
    float yPos = (float)ypos;
    
    if (firstMouse)
    {
        lastX = xPos;
        lastY = yPos;
        firstMouse = false;
    }

    float xoffset = (float)xpos - lastX;
    float yoffset = lastY - (float)ypos; // reversed since y-coordinates go from bottom to top
    lastX = (float)xpos;
    lastY = (float)ypos;

    float sensitivity = 0.1f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw += xoffset;
    pitch += yoffset;

    if (pitch > 89.f)
        pitch = 89.f;
    if (pitch < -89.f)
        pitch = -89.f;
    
    glm::vec3 front;
    front.x = cos(glm::radians(yaw) * cos(glm::radians(pitch)));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw) * cos(glm::radians(pitch)));
    cameraCenter = glm::normalize(front);
}

/* Spawns Objects and Stores them in a vector named container
 * Also handles the spawn input
 */
void SpawnObject(std::string sMeshPath, std::string sVertPath, std::string sFragPath, std::vector<Model::Model3D*>* container, 
    glm::vec3 translate, float pitch, float yaw, float roll, glm::vec3 scale)
{
     Model::Model3D* temp = new Model::Model3D(sMeshPath, sVertPath, sFragPath);
     temp->SetTransform(translate, pitch, yaw, roll, scale);
     container->push_back(temp);
}

void SpawnModel(int state, std::vector<Model::Model3D*>& vecModel, glm::vec3& spawnPos)
{
    if (state == GLFW_PRESS)
    {
        //if timer reacher 3, allow spawn
        if (glfwGetTime() >= 3.0f)
        {
            std::cout << "SPAWN!" << std::endl;

            /* Blaster A, Blaster B, and Blaster C are taken from kenny.nl's blaster kit */
            SpawnObject(
                "3D/blasterA.obj",          //Path to 3D Model
                "Shaders/sample.vert",      //Path to vertex shader
                "Shaders/sample.frag",      //Path to fragment shader
                &vecModel,                  //vector container for storage
                spawnPos,                   //translation
                pitch,                       //along x rotation
                yaw,                       //along y rotation
                0.0f,                       //along z rotation
                glm::vec3(5.f, 5.f, 5.f)    //scale
            );

            glfwSetTime(0.0f);
        }
        else if (glfwGetTime() < 3.0f)
        {
            std::cout << "NOT YET READY!! CD: " << glfwGetTime() << std::endl;
        }
    }
}

int main(void)
{
    GLFWwindow* window;

    // Window Width and Height
    float width = 600.0f;
    float height = 600.0f;

    //Stores all the 3D Models
    std::vector<Model::Model3D*> vecModel;

    //Constant vectors and matrices
    glm::mat4 projection = glm::perspective(glm::radians(60.0f), (height / width), 0.1f, 100.0f);
    glm::mat4 identity_matrix = glm::mat4(1.0f);             

    if (!glfwInit())
        return -1;

    window = glfwCreateWindow((int)width, (int)height, "Thomas Banatin", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    gladLoadGL();

    glfwSetKeyCallback(window, Key_Callback);
    glfwSetCursorPosCallback(window, Mouse_Callback);
     
    //Timer function set to 0
    glfwSetTime(3.0f);

    //For Delta Time Function
    previousTime = (float)glfwGetTime();

    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT);

        float currentTime = (float)glfwGetTime();
        deltaTime = currentTime - previousTime;
        previousTime = currentTime;

        //View Matrix inside the while loop to we can move the camera
        glm::mat4 view_matrix = glm::lookAt(cameraPos, cameraPos + cameraCenter, worldUp);

        /* * * * * * Spawn Script * * * * * */

        //If space bar is pressed
        int state = glfwGetKey(window, GLFW_KEY_SPACE);

        //Spawn Location -> Position in front of camera with an offset
        glm::vec3 spawnPos = cameraPos + (10.f * cameraCenter);
        
        //Spawns the first game object
        /* BlasterA, Blaster B, and Blaster C are taken from kenny.nl */
        if (vecModel.empty())
        {
            SpawnModel(GLFW_PRESS, vecModel, spawnPos);
        }

        //Spawn Handler
        SpawnModel(state, vecModel, spawnPos);


        /* * * * * * * * * * * * * * * * * */

        //Draw Call, we pass on the view and projection matrices
        for (size_t i = 0; i < vecModel.size(); i++)
        {
            vecModel[i]->DrawModel(view_matrix, projection);
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    //Destroy VBOs, VAOs, and EBOs
    for (size_t i = 0; i < vecModel.size(); i++)
    {
        vecModel[i]->CleanUp();
    }

    glfwTerminate();
    return 0;
}