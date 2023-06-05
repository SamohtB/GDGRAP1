#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <cmath>

#include "Model3D.h"

int main(void)
{
    GLFWwindow* window;
    float width = 600.0f;
    float height = 600.0f;

    glm::mat4 identity_matrix = glm::mat4(1.0f);
    glm::vec3 translate = glm::vec3(0.0f, 0.0f, -5.0f);
    glm::vec3 scale = glm::vec3(15.0f, 15.0f, 15.0f);

    glm::vec3 worldUp = glm::normalize(glm::vec3(0.0f, 1.0f, 0.0f));    //World's Up

    glm::vec3 cameraPos_front = glm::vec3(0.0f, 0.0f, 10.0f);
    glm::vec3 cameraPos_side = glm::vec3(4.0f, 0.0f, 10.0f);
    glm::vec3 cameraPos_top = glm::vec3(-4.0f, 0.0f, 10.0f);

    glm::vec3 cameraCenter_front = glm::vec3(0.0f, 5.0f, 0.0f);               //camera's Target
    glm::vec3 cameraCenter_side = glm::vec3(4.0f, 1.0f, 0.0f);               //camera's Target
    glm::vec3 cameraCenter_top = glm::vec3(-4.0f, 0.0f, 0.0f);               //camera's Target

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

    Model::Model3D bunny("3D/bunny.obj", "Shaders/sample.vert", "Shaders/sample.frag");

    glm::mat4 transform_matrix_front = glm::translate(identity_matrix, translate);
    glm::mat4 transform_matrix_side = glm::translate(identity_matrix, translate);
    glm::mat4 transform_matrix_top = glm::translate(identity_matrix, translate);

    transform_matrix_front = glm::scale(transform_matrix_front, scale);
    transform_matrix_side = glm::scale(transform_matrix_side, scale);
    transform_matrix_top = glm::scale(transform_matrix_top, scale);

    glm::mat4 view_matrix_front = glm::lookAt(cameraPos_front, cameraCenter_front, worldUp);
    glm::mat4 view_matrix_side = glm::lookAt(cameraPos_side, cameraCenter_side, worldUp);
    glm::mat4 view_matrix_top = glm::lookAt(cameraPos_top, cameraCenter_top, worldUp);

    transform_matrix_front = glm::rotate(transform_matrix_front, glm::radians(90.0f), glm::vec3(0.f, 1.0f, 0.0f));
    transform_matrix_top = glm::rotate(transform_matrix_top, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));

    glm::mat4 projection_matrix = glm::perspective(glm::radians(60.0f), (height / width), 0.1f, 100.0f);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);
       
        bunny.SetColor(glm::vec3(1.0f, 0.0f, 0.0f));
        bunny.DrawModel(transform_matrix_front, view_matrix_front, projection_matrix);

        bunny.SetColor(glm::vec3(0.0f, 1.0f, 0.0f));
        bunny.DrawModel(transform_matrix_side, view_matrix_side, projection_matrix);

        bunny.SetColor(glm::vec3(0.0f, 0.0f, 1.0f));
        bunny.DrawModel(transform_matrix_top, view_matrix_top, projection_matrix);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    bunny.CleanUp();

    glfwTerminate();
    return 0;
}