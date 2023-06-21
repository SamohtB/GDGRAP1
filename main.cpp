#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <cmath>

#include "Model3D.h"

glm::vec3 translate = glm::vec3(0.0f, 0.0f, -5.0f);
glm::vec3 scale = glm::vec3(0.1f, 0.1f, 0.1f);
glm::vec3 rotateX = glm::vec3(1.0f, 0.0f, 0.0f);
glm::vec3 rotateY = glm::vec3(0.0f, 1.0f, 0.0f);
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 10.0f);
glm::vec3 cameraCenter = glm::vec3(0.0f, 0.0f, 0.0f);               //camera's Target
float thetaX = 0.0f;
float thetaY = 0.0f;
float FOV = 60.0f;

void Key_Callback(GLFWwindow* window, int key, int scancode, int action, int mod)
{
    //WASD
    if (key == GLFW_KEY_D && (action == GLFW_PRESS || action == GLFW_REPEAT))
    {
        cameraPos.x -= 0.1f;
        cameraCenter.x -= 0.1f;
    }
    if (key == GLFW_KEY_A && (action == GLFW_PRESS || action == GLFW_REPEAT))
    {
        cameraPos.x += 0.1f;
        cameraCenter.x += 0.1f;
    }
    if (key == GLFW_KEY_W && (action == GLFW_PRESS || action == GLFW_REPEAT))
    {
        translate.y += 0.1f;
    }
    if (key == GLFW_KEY_S && (action == GLFW_PRESS || action == GLFW_REPEAT))
    {
        translate.y -= 0.1f;
    }

    //ROTATE Y
    if (key == GLFW_KEY_UP && (action == GLFW_PRESS || action == GLFW_REPEAT))
    {
        thetaX -= 0.1f;
    }
    if (key == GLFW_KEY_DOWN && (action == GLFW_PRESS || action == GLFW_REPEAT))
    {
        thetaX += 0.1f;
    }

    //ROTATE X
    if (key == GLFW_KEY_LEFT && (action == GLFW_PRESS || action == GLFW_REPEAT))
    {
        thetaY -= 0.1f;
    }
    if (key == GLFW_KEY_RIGHT && (action == GLFW_PRESS || action == GLFW_REPEAT))
    {
        thetaY += 0.1f;
    }

    //SCALE
    if (key == GLFW_KEY_Q && (action == GLFW_PRESS || action == GLFW_REPEAT))
    {
        scale.x += 0.1f;
        scale.y += 0.1f;
        scale.z += 0.1f;
    }
    if (key == GLFW_KEY_E && (action == GLFW_PRESS || action == GLFW_REPEAT))
    {
        scale.x -= 0.1f;
        scale.y -= 0.1f;
        scale.z -= 0.1f;
    }

    //ZOOM
    if (key == GLFW_KEY_Z && (action == GLFW_PRESS || action == GLFW_REPEAT))
    {
        FOV -= 1.0f;
    }
    if (key == GLFW_KEY_X && (action == GLFW_PRESS || action == GLFW_REPEAT))
    {
        FOV += 1.0f;
    }
}

int main(void)
{
    GLFWwindow* window;
    float width = 600.0f;
    float height = 600.0f;

    glm::mat4 identity_matrix = glm::mat4(1.0f);

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow((int)width, (int)height, "Thomas Banatin", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    gladLoadGL();

    Model::Model3D bunny("3D/djSword.obj", "Shaders/sample.vert", "Shaders/sample.frag", "3D/ayaya.png");

    glfwSetKeyCallback(window, Key_Callback);

    glViewport( 0,                      //min x
                0,                      //min y
                (GLsizei)width,         //max x
                (GLsizei)height);       //max y

    /*
    // ORTHOGRAPHIC PROJECTION
    glm::mat4 projection = glm::ortho(
        2.0f,           //left most
        -2.0f,          //right most
        2.0f,           //bottom most
        -2.0f,          //top most
        -1.0f,          //Z near
        1.0f           //Z far
    );      
    */

    glEnable(GL_DEPTH_TEST);

    //Timer function set to 0
    glfwSetTime(0.0f);

    //For Delta Time Function
    float previousTime = (float)glfwGetTime();

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        glm::mat4 cameraPosMatrix = glm::translate(identity_matrix, cameraPos * -1.0f);

        glm::vec3 worldUp = glm::normalize(glm::vec3(0.0f, 1.0f, 0.0f));    //World's Up

        glm::vec3 F = (cameraCenter - cameraPos);
        F = glm::normalize(F);
        glm::vec3 R = glm::cross(F, worldUp);
        glm::vec3 U = glm::cross(R, F);

        glm::mat4 cameraOrientation = glm::mat4(1.0f);

        //Matrix[C][R]
        //R
        cameraOrientation[0][0] = R.x;
        cameraOrientation[1][0] = R.y;
        cameraOrientation[2][0] = R.z;
        //U
        cameraOrientation[0][1] = U.x;
        cameraOrientation[1][1] = U.y;
        cameraOrientation[2][1] = U.z;
        //-F
        cameraOrientation[0][2] = -F.x;
        cameraOrientation[1][2] = -F.y;
        cameraOrientation[2][2] = -F.z;

        //glm::mat4 view_matrix = cameraOrientation * cameraPosMatrix;

        float currentTime = (float)glfwGetTime();
        float deltaTime = currentTime - previousTime;
        previousTime = currentTime;

        //rotation
        thetaY += 1.01f * deltaTime;

        glm::mat4 view_matrix = glm::lookAt(cameraPos, cameraCenter, worldUp);
        glm::mat4 projection = glm::perspective(glm::radians(FOV), (height / width), 0.1f, 100.0f);
        glm::mat4 transform_matrix = glm::translate(identity_matrix, translate);
        transform_matrix = glm::scale(transform_matrix, scale);
        transform_matrix = glm::rotate(transform_matrix, thetaX, rotateX);
        transform_matrix = glm::rotate(transform_matrix, thetaY, rotateY);
       
        bunny.DrawModel(transform_matrix, view_matrix, projection);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    bunny.CleanUp();

    glfwTerminate();
    return 0;
}