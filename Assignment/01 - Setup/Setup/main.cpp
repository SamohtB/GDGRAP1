#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cmath>

#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"

#include <string>
#include <iostream>

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

    glfwSetKeyCallback(window, Key_Callback);

    std::fstream vertSrc("Shaders/sample.vert");
    std::stringstream vertBuff;
    vertBuff << vertSrc.rdbuf();
    std::string vertS = vertBuff.str();
    const char* v = vertS.c_str();

    std::fstream fragSrc("Shaders/sample.frag");
    std::stringstream fragBuff;
    fragBuff << fragSrc.rdbuf();
    std::string fragS = fragBuff.str();
    const char* f = fragS.c_str();

    GLuint vertShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertShader, 1, &v, NULL);
    glCompileShader(vertShader);

    GLuint fragShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragShader, 1, &f, NULL);
    glCompileShader(fragShader);

    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertShader);
    glAttachShader(shaderProgram, fragShader);
    glLinkProgram(shaderProgram);   
    


    std::string path = "3D/bunny.obj";
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> material;
    std::string warning, error;

    tinyobj::attrib_t attributes;

    bool success = tinyobj::LoadObj(
        &attributes,
        &shapes,
        &material,
        &warning,
        &error,
        path.c_str());

    std::vector<GLuint> mesh_indices;
    for (int i = 0; i < shapes[0].mesh.indices.size(); i++)
    {
        mesh_indices.push_back(shapes[0].mesh.indices[i].vertex_index);
    }

    GLfloat vertices[]{
        0.0f, 0.5f, 0.0f,       //point 1
        -0.5f, -0.5f, 0.0f,     //point 2
        0.5f, -0.5f, 0.0f       //point 3
    };

    GLuint indices[]{
        0, 1, 2
    };

    GLuint VAO, VBO, EBO;

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    //tells opengl that we're working on this VAO
    glBindVertexArray(VAO);

    glUseProgram(shaderProgram);

    //Assign a VBO to the VAO
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(
        GL_ARRAY_BUFFER,        //type of buffer
        sizeof(GL_FLOAT) * attributes.vertices.size(),       //size in bytes
        &attributes.vertices[0],               //data array
        GL_STATIC_DRAW
    );

    //tell openGL what to do with the data
    glVertexAttribPointer(
        0, //position data in the transform
        3, //xyz
        GL_FLOAT, //array data type
        GL_FALSE,
        3 * sizeof(GL_FLOAT),
        (void*)0
    );

    //enable index 0
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * mesh_indices.size(), mesh_indices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        unsigned int xLoc = glGetUniformLocation(shaderProgram, "x");
        unsigned int yLoc = glGetUniformLocation(shaderProgram, "y");
        glUniform1f(xLoc, x_mod);
        glUniform1f(yLoc, y_mod);

        glBindVertexArray(VAO);
        glDrawElements(
            GL_TRIANGLES,
            mesh_indices.size(),
            GL_UNSIGNED_INT,
            0
        );

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

    glfwTerminate();
    return 0;
}