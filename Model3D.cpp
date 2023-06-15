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
    pos_x = 0.0f;
    pos_y = 0.0f;
    pos_z = 0.0f;
    rot_pitch = 0.0f;
    rot_yaw = 0.0f;
    rot_roll = 0.0f;
    scl_x = 1.0f;
    scl_y = 1.0f;
    scl_z = 1.0f;

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
void Model3D::SetTransform(glm::vec3 translate, float pitch, float yaw, float roll, glm::vec3 scale)
{
    pos_x = translate.x;
    pos_y = translate.y;
    pos_z = translate.z;
    rot_pitch = pitch;
    rot_yaw = yaw;
    rot_roll = roll;
    scl_x = scale.x;
    scl_y = scale.y;
    scl_z = scale.z;
}

/* Creates the transform matrix fro existing internal parameters */
glm::mat4 Model3D::BuildTransformMatrix()
{
    glm::mat4 transform_matrix = glm::mat4(1.0f);
    transform_matrix = glm::translate(transform_matrix, glm::vec3(pos_x, pos_y, pos_z));
    transform_matrix = glm::scale(transform_matrix, glm::vec3(scl_x, scl_y, scl_z));
    transform_matrix = glm::rotate(transform_matrix, glm::radians(rot_pitch), glm::vec3(1.f, 0.f, 0.f));
    transform_matrix = glm::rotate(transform_matrix, glm::radians(rot_yaw), glm::vec3(0.f, 1.f, 0.f));
    transform_matrix = glm::rotate(transform_matrix, glm::radians(rot_roll), glm::vec3(0.f, 0.f, 1.f));
    return transform_matrix;
}

/*  Draws the object
 *  Parameters:
 *      view_matrix:        view matrix used
 *      projection_matrix:  projection matrix used
 */
void Model3D::DrawModel(glm::mat4 view_matrix, glm::mat4 projection_matrix)
{
    glm::mat4 transform_matrix = BuildTransformMatrix();

    unsigned int transformLoc = glGetUniformLocation(this->shaderProgram, "transform");
    glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform_matrix));

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