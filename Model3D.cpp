#define TINYOBJLOADER_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#include "Model3D.h"

using namespace Model;

Model3D::Model3D(std::string sMeshPath, std::string sVertPath, std::string sFragPath, std::string sTexPath)
{
    this->LoadShaders(sVertPath, sFragPath);
    this->LoadTexture(sTexPath);
	this->LoadModel(sMeshPath);
    this->VertexInit();
}

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

void Model3D::LoadTexture(std::string sTexPath)
{
    stbi_set_flip_vertically_on_load(true);

    const char* path = sTexPath.c_str();
    unsigned char* tex_bytes = stbi_load(path, &this->img_width, &this->img_height, &this->colorChannels, 0);

    glGenTextures(1, &this->texture);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, this->texture);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, this->img_width, this->img_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, tex_bytes);

    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(tex_bytes);
}

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

    for (size_t i = 0; i < shapes[0].mesh.indices.size(); i++)
    {
        tinyobj::index_t vData = shapes[0].mesh.indices[i];

        fullVertexData.push_back(attributes.vertices[vData.vertex_index * 3]);      // X pos
        fullVertexData.push_back(attributes.vertices[vData.vertex_index * 3 + 1]);  // Y pos
        fullVertexData.push_back(attributes.vertices[vData.vertex_index * 3 + 2]);  // Z pos

        fullVertexData.push_back(attributes.normals[vData.normal_index * 3]);       // R nor
        fullVertexData.push_back(attributes.normals[vData.normal_index * 3 + 1]);   // S nor
        fullVertexData.push_back(attributes.normals[vData.normal_index * 3 + 2]);   // T nor

        fullVertexData.push_back(attributes.texcoords[vData.texcoord_index * 2]);       // U tex
        fullVertexData.push_back(attributes.texcoords[vData.texcoord_index * 2 + 1]);   // V tex
    }
}

void Model3D::VertexInit()
{
    glGenVertexArrays(1, &this->VAO);
    glGenBuffers(1, &this->VBO);
    //glGenBuffers(1, &this->VBO_UV);
    //glGenBuffers(1, &this->EBO);

    //tells opengl that we're working on this VAO
    glBindVertexArray(this->VAO);

    //use shaders
    glUseProgram(this->shaderProgram);

    //Assign a VBO to the VAO
    glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GL_FLOAT) * this->fullVertexData.size(), this->fullVertexData.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GL_FLOAT), (void*)0);

    GLuint normalsPtr = 3 * sizeof(GLfloat);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GL_FLOAT), (void*)normalsPtr);

    GLuint uvPtr = 6 * sizeof(GLfloat);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GL_FLOAT), (void*)uvPtr);


    ////Assign a EBO to the VAO
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
    //glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * this->mesh_indices.size(), this->mesh_indices.data(), GL_STATIC_DRAW);
    ////Assign a VBO_UV to the VAO
    //glBindBuffer(GL_ARRAY_BUFFER, this->VBO_UV);
    //glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * (sizeof(this->UV) / sizeof(this->UV[0])), &this->UV[0], GL_STATIC_DRAW);
    //glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Model3D::DrawModel(glm::mat4 transform_matrix, glm::mat4 view_matrix, glm::mat4 projection_matrix, 
    glm::vec3 lightPos, glm::vec3 lightColor, float ambientStr, glm::vec3 ambientColor, glm::vec3 cameraPos,
    float specStr, float specPhong)
{
    unsigned int transformLoc = glGetUniformLocation(this->shaderProgram, "transform");
    glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform_matrix));

    unsigned int viewLoc = glGetUniformLocation(this->shaderProgram, "view");
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view_matrix));

    unsigned int projectionLoc = glGetUniformLocation(this->shaderProgram, "projection");
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection_matrix));

    GLuint tex0Address = glGetUniformLocation(this->shaderProgram, "tex0");
    glBindTexture(GL_TEXTURE_2D, this->texture);
    glUniform1i(tex0Address, 0);

    GLuint lightAddress = glGetUniformLocation(this->shaderProgram, "lightPos");
    glUniform3fv(lightAddress, 1, glm::value_ptr(lightPos));

    GLuint lightColorAddress = glGetUniformLocation(this->shaderProgram, "lightColor");
    glUniform3fv(lightColorAddress, 1, glm::value_ptr(lightColor));

    GLuint ambientStrAddress = glGetUniformLocation(this->shaderProgram, "ambientStr");
    glUniform1f(ambientStrAddress, ambientStr);

    GLuint ambientColorAddress = glGetUniformLocation(this->shaderProgram, "ambientColor");
    glUniform3fv(ambientColorAddress, 1, glm::value_ptr(ambientColor));

    GLuint cameraPosAddress = glGetUniformLocation(this->shaderProgram, "cameraPos");
    glUniform3fv(cameraPosAddress, 1, glm::value_ptr(cameraPos));

    GLuint specStrAddress = glGetUniformLocation(this->shaderProgram, "specStr");
    glUniform1f(specStrAddress, specStr);

    GLuint specPhongAddress = glGetUniformLocation(this->shaderProgram, "specPhong");
    glUniform1f(specPhongAddress, specPhong);

    glBindVertexArray(this->VAO);

    glDrawArrays(GL_TRIANGLES, 0, this->fullVertexData.size() / 8);

    //glDrawElements(
    //    GL_TRIANGLES,
    //    this->mesh_indices.size(),
    //    GL_UNSIGNED_INT,
    //    0
    //);
}

void Model3D::CleanUp()
{
    glDeleteVertexArrays(1, &this->VAO);
    glDeleteBuffers(1, &this->VBO);
    glDeleteBuffers(1, &this->EBO);
}