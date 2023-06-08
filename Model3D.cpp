#define TINYOBJLOADER_IMPLEMENTATION
#include "Model3D.h"

using namespace Model;

/*  Constructor
 *  Parameters: 
 *      sMeshPath = Path to 3D Model (.obj file)
 *      sVertPath = Path to Vertex Shader
 *      sFragPath = Path to Fragment Shader
 */
Model3D::Model3D(std::string sMeshPath, std::string sVertPath, std::string sFragPath)
{
    this->transform_matrix= glm::mat4(1.0f);
    this->LoadShaders(sVertPath, sFragPath);
	this->LoadModel(sMeshPath);
    this->VertexInit();
}

/* Creates the shader program
 * Parameters: 
 *      sVertPath = Path to Vertex Shader
 *      sFragPath = Path to Fragment Shader
 */
void Model3D::LoadShaders(std::string sVertPath, std::string sFragPath)
{
    this->shaderProgram = glCreateProgram();

    //Vertex Shader
    std::fstream vertSrc(sVertPath);
    std::stringstream vertBuff;
    vertBuff << vertSrc.rdbuf();
    std::string vertS = vertBuff.str();
    const char* v = vertS.c_str();

    GLuint vertShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertShader, 1, &v, NULL);
    glCompileShader(vertShader);
    glAttachShader(shaderProgram, vertShader);

    //Fragment Shader
    std::fstream fragSrc(sFragPath);
    std::stringstream fragBuff;
    fragBuff << fragSrc.rdbuf();
    std::string fragS = fragBuff.str();
    const char* f = fragS.c_str();

    GLuint fragShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragShader, 1, &f, NULL);
    glCompileShader(fragShader);
    glAttachShader(shaderProgram, fragShader);

    glLinkProgram(shaderProgram);
}

/* Load the Model -> Gets the Model's vertices
 * Parameters:
 *      sMeshPath = Path to 3D Model (.obj file)
 */
void Model3D::LoadModel(std::string sMeshPath)
{
    std::string path = sMeshPath;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> material;
    std::string warning, error;

    bool success = tinyobj::LoadObj(
        &this->attributes,
        &shapes,
        &material,
        &warning,
        &error,
        path.c_str());

    for (size_t i = 0; i < shapes[0].mesh.indices.size(); i++)
    {
        this->mesh_indices.push_back(shapes[0].mesh.indices[i].vertex_index);
    }
}

/* Generate and Pass VAO, VBO, and EBO */
 
void Model3D::VertexInit()
{
    //Generate
    glGenVertexArrays(1, &this->VAO);
    glGenBuffers(1, &this->VBO);
    glGenBuffers(1, &this->EBO);

    //Use VAO
    glBindVertexArray(this->VAO);
    //Use shaders
    glUseProgram(this->shaderProgram);

    //Assign to VAO
    glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
    glBufferData(GL_ARRAY_BUFFER,                               
        sizeof(GL_FLOAT) * this->attributes.vertices.size(),    
        &this->attributes.vertices[0],                          
        GL_STATIC_DRAW
    );
    glVertexAttribPointer(     
        0, 
        3, 
        GL_FLOAT,
        GL_FALSE,
        3 * sizeof(GL_FLOAT),
        (void*)0
    );
    glEnableVertexAttribArray(0);

    //Assign to VAO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 
                    sizeof(GLuint) * this->mesh_indices.size(), 
                    this->mesh_indices.data(), 
                    GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    //End of VAO use
    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

/*  Sets the Transform matrix of the object
 *  Parameters:
 *      translate:  translate by
 *      pitch:      rotate by . . . along x-axis
 *      yaw:        rotate by . . . along y-axis
 *      roll:       rotate by . . . along z-axis
 *      scale:      scale by
 */
void Model3D::SetTransform(glm::vec3 translate, float pitch, float yaw, float roll, float scale)
{
    this->transform_matrix = glm::translate(this->transform_matrix, translate);
    this->transform_matrix = glm::scale(this->transform_matrix, glm::vec3(scale));
    this->transform_matrix = glm::rotate(this->transform_matrix, glm::radians(pitch), glm::vec3(1.0f, 0.0f, 0.0f));
    this->transform_matrix = glm::rotate(this->transform_matrix, glm::radians(yaw), glm::vec3(0.0f, 1.0f, 0.0f));
    this->transform_matrix = glm::rotate(this->transform_matrix, glm::radians(roll), glm::vec3(0.0f, 0.0f, 1.0f));
}

void Model3D::Move(glm::vec3 translate)
{
}

void Model3D::Rotate(Axis axis, float Increment)
{
}

/*  Draws the object
 *  Parameters:
 *      view_matrix:        view matrix used
 *      projection_matrix:  projection matrix used
 */
void Model3D::DrawModel(glm::mat4 view_matrix, glm::mat4 projection_matrix)
{

    unsigned int transformLoc = glGetUniformLocation(this->shaderProgram, "transform");
    glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(this->transform_matrix));

    unsigned int viewLoc = glGetUniformLocation(this->shaderProgram, "view");
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view_matrix));

    unsigned int projectionLoc = glGetUniformLocation(this->shaderProgram, "projection");
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection_matrix));

    glBindVertexArray(this->VAO);

    glDrawElements(
        GL_TRIANGLES,
        this->mesh_indices.size(),
        GL_UNSIGNED_INT,
        0
    );
}

/* Frees up memory of VAO, VBO, and EBO used */
void Model3D::CleanUp()
{
    glDeleteVertexArrays(1, &this->VAO);
    glDeleteBuffers(1, &this->VBO);
    glDeleteBuffers(1, &this->EBO);
}