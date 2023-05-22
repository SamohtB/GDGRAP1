#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cmath>

#include "Model3D.h"

float x_mod = 0;
float y_mod = 0;

void Key_Callback(GLFWwindow* window, int key, int scancode, int action, int mod)
{
    if (key == GLFW_KEY_D && action == GLFW_PRESS)
    {
        x_mod += 0.1f;
    }
    if (key == GLFW_KEY_A && action == GLFW_PRESS)
    {
        x_mod -= 0.1f;
    }
    if (key == GLFW_KEY_W && action == GLFW_PRESS)
    {
        y_mod += 0.1f;
    }
    if (key == GLFW_KEY_S && action == GLFW_PRESS)
    {
        y_mod -= 0.1f;
    }

}

int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Thomas Banatin", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    gladLoadGL();

    Model::Model3D bunny("3D/bunny.obj", "Shaders/sample.vert", "Shaders/sample.frag");

    glfwSetKeyCallback(window, Key_Callback);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        unsigned int xLoc = glGetUniformLocation(bunny.getShaderProgram(), "x");
        unsigned int yLoc = glGetUniformLocation(bunny.getShaderProgram(), "y");
        glUniform1f(xLoc, x_mod);
        glUniform1f(yLoc, y_mod);

        glBindVertexArray(bunny.getVAO());
        glDrawElements(
            GL_TRIANGLES,
            bunny.getMeshIndices().size(),
            GL_UNSIGNED_INT,
            0
        );

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    bunny.CleanUp();

    glfwTerminate();
    return 0;
}