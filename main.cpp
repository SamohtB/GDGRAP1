#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <cmath>

#include "Model3D.h"

glm::vec3 translate = glm::vec3(0.0f, 0.0f, -5.0f);
glm::vec3 scale = glm::vec3(2.0f, 2.0f, 2.0f);
glm::vec3 rotate = glm::vec3(0.0f, 1.0f, 0.0f);

void Key_Callback(GLFWwindow* window, int key, int scancode, int action, int mod)
{
    if (key == GLFW_KEY_D && action == GLFW_PRESS)
    {
        translate.x += 0.1f;
    }
    if (key == GLFW_KEY_A && action == GLFW_PRESS)
    {
        translate.x -= 0.1f;
    }
    if (key == GLFW_KEY_W && action == GLFW_PRESS)
    {
        translate.y += 0.1f;
    }
    if (key == GLFW_KEY_S && action == GLFW_PRESS)
    {
        translate.y -= 0.1f;
    }

}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{

    if (yoffset > 0)
    {
        //scale.x += 0.1f;
        //scale.y += 0.1f;
        //scale.z += 0.1f;
        translate.z += 0.1f;
    }
    else if(yoffset < 0)
    {
        //scale.x -= 0.1f;
        //scale.y -= 0.1f;
        //scale.z -= 0.1f;
        translate.z -= 0.1f;
    }

}

int main(void)
{
    GLFWwindow* window;
    float _width = 1000;
    float _height = 1000;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(_width, _height, "Thomas Banatin", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    gladLoadGL();

    glfwSetKeyCallback(window, Key_Callback);
    glfwSetScrollCallback(window, scroll_callback);

    glViewport( 0,          //min x
                0,          //min y
                _width,      //max x
                _height);    //max y

    Model::Model3D bunny("3D/bunny.obj", "Shaders/sample.vert", "Shaders/sample.frag");

    glm::mat4 identity_matrix = glm::mat4(1.0f);
    float theta = 0.0f;

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

    glm::mat4 projection = glm::perspective(glm::radians(60.0f), (_height / _width), 0.1f, 100.0f);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);
        
        glm::mat4 transform_matrix = glm::translate(identity_matrix, translate);
        transform_matrix = glm::scale(transform_matrix, scale);
        transform_matrix = glm::rotate(transform_matrix, theta, rotate);
       
        bunny.DrawModel(transform_matrix, projection);

        theta += 0.001f;
        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    bunny.CleanUp();

    glfwTerminate();
    return 0;
}