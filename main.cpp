#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <cmath>

#include "Model3D.h"

float x_mod = 0.0f;
float y_mod = 0.0f;

void Key_Callback(GLFWwindow* window, int key, int scancode, int action, int mod)
{
    if (key == GLFW_KEY_W && (action == GLFW_PRESS || action == GLFW_REPEAT))
    {
        y_mod += 0.5f;
    }
    if (key == GLFW_KEY_S && (action == GLFW_PRESS || action == GLFW_REPEAT))
    {
        y_mod -= 0.5f;
    }
    if (key == GLFW_KEY_A && (action == GLFW_PRESS || action == GLFW_REPEAT))
    {
        x_mod += 0.5f;
    }
    if (key == GLFW_KEY_D && (action == GLFW_PRESS || action == GLFW_REPEAT))
    {
        x_mod -= 0.5f;
    }
}

/* Spawns Objects and Stores them in a vector named container
 * Also handles the spawn input
 */
void SpawnObject(std::string sMeshPath, std::string sVertPath, std::string sFragPath, std::vector<Model::Model3D*>* container, 
    glm::vec3 translate, float pitch, float yaw, float roll, float scale)
{
     Model::Model3D* temp = new Model::Model3D(sMeshPath, sVertPath, sFragPath);
     temp->SetTransform(translate, pitch, yaw, roll, scale);
     container->push_back(temp);
}

int main(void)
{
    GLFWwindow* window;

    // Window Width and Height
    float width = 600.0f;
    float height = 600.0f;

    //Stores all the 3D Models
    std::vector<Model::Model3D*> vecModel;
    
    //Offset because we use the camera's position to spawn the object
    //then we add the  offset to make it spawn in front of the camera
    glm::vec3 spawnOffset = glm::vec3(0.0f, 0.0f, -10.0f);

    //Constant vectors and matrices
    glm::vec3 worldUp = glm::normalize(glm::vec3(0.0f, 1.0f, 0.0f));    //World's Up
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

    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT);

        //View Matrix inside the while loop to we can move the camera
        glm::vec3 cameraPos = glm::vec3(x_mod, y_mod, 10.0f);
        glm::vec3 cameraCenter = glm::vec3(x_mod, y_mod, 0.0f);
        glm::mat4 view_matrix = glm::lookAt(cameraPos, cameraCenter, worldUp);

        if (vecModel.empty())
        {
            SpawnObject("3D/blasterA.obj", "Shaders/sample.vert", "Shaders/sample.frag", &vecModel, cameraPos + spawnOffset, 90.0f, -90.0f, 90.0f, 5.0f);
        }

        //Spawn Handler
        int state = glfwGetKey(window, GLFW_KEY_SPACE);
        if (state == GLFW_PRESS)
        {
            SpawnObject(
                "3D/blasterA.obj",          //Path to 3D Model
                "Shaders/sample.vert",      //Path to vertex shader
                "Shaders/sample.frag",      //Path to fragment shader
                &vecModel,                  //vector container for storage
                cameraPos + spawnOffset,    //translation
                90.0f,                      //along x rotation
                -90.0f,                     //along y rotation
                90.0f,                      //along z rotation
                5.0f                        //scale
            );
        }

        //Draw Call, we pass on the view and projection matrices
        for (int i = 0; i < vecModel.size(); i++)
        {
            vecModel[i]->DrawModel(view_matrix, projection);
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    //Destroy VBOs, VAOs, and EBOs
    for (int i = 0; i < vecModel.size(); i++)
    {
        vecModel[i]->CleanUp();
    }

    glfwTerminate();
    return 0;
}