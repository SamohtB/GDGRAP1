#define TINYOBJLOADER_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#include "Model3D.hpp"

using namespace Model;

Model3D::Model3D(std::string strMeshPath, std::string strVertPath, std::string strFragPath, 
    std::string strTexPath, std::string strNormPath)
{
    LoadShaders(strVertPath, strFragPath);
	LoadModel(strMeshPath);
    VertexInit();
    LoadTexture(strTexPath);
    LoadNormals(strNormPath);
}

Model3D::~Model3D()
{
    glDeleteVertexArrays(1, &this->VAO);
    glDeleteBuffers(1, &this->VBO);
    glDeleteTextures(1, &this->texture);
    glDeleteTextures(1, &this->normal_texture);
}

void Model3D::LoadShaders(std::string strVertPath, std::string strFragPath)
{
    //Vertex Shader
    std::fstream vertSrc(strVertPath);
    std::stringstream vertBuff;
    vertBuff << vertSrc.rdbuf();
    std::string vertS = vertBuff.str();
    const char* v = vertS.c_str();

    std::fstream fragSrc(strFragPath);
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
    glDeleteShader(vertShader);
    glDeleteShader(fragShader);

    this->shaderProgram = shaderProgram;
}

void Model3D::LoadModel(std::string strMeshPath)
{
    std::string path = strMeshPath;
    tinyobj::attrib_t attributes;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> material;
    std::string warning, error;

    bool success = tinyobj::LoadObj(
        &attributes,
        &shapes,
        &material,
        &warning,
        &error,
        path.c_str());


    std::vector<GLuint> mesh_indices;

    for (size_t i = 0; i < shapes[0].mesh.indices.size(); i++)
    {
        mesh_indices.push_back(shapes[0].mesh.indices[i].vertex_index);
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

    glBindVertexArray(this->VAO);
    glUseProgram(this->shaderProgram);

    glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GL_FLOAT) * this->fullVertexData.size(), this->fullVertexData.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GL_FLOAT), (void*)0);

    GLuint normalsPtr = 3 * sizeof(GLfloat);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GL_FLOAT), (void*)normalsPtr);

    GLuint uvPtr = 6 * sizeof(GLfloat);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GL_FLOAT), (void*)uvPtr);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Model3D::LoadTexture(std::string strTexPath)
{
    stbi_set_flip_vertically_on_load(true);
    int img_width, img_height, colorChannels;

    const char* path = strTexPath.c_str();
    unsigned char* tex_bytes = stbi_load(path, &img_width, &img_height, &colorChannels, 0);

    glGenTextures(1, &this->texture);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, this->texture);

    std::string strFileType = strTexPath.substr(strTexPath.length() - 3);

    if (strFileType == "png")
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img_width, img_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, tex_bytes);
    }
    else if (strFileType == "jpg")
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, img_width, img_height, 0, GL_RGB, GL_UNSIGNED_BYTE, tex_bytes);
    }

    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(tex_bytes);
}

void Model3D::LoadNormals(std::string strNormPath)
{
    stbi_set_flip_vertically_on_load(true);
    int img_width, img_height, colorChannels;

    const char* path = strNormPath.c_str();
    unsigned char* tex_bytes = stbi_load(path, &img_width, &img_height, &colorChannels, 0);

    glGenTextures(1, &this->normal_texture);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, this->normal_texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

    std::string strFileType = strNormPath.substr(strNormPath.length() - 3);

    if (strFileType == "png")
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img_width, img_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, tex_bytes);
    }
    else if (strFileType == "jpg")
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, img_width, img_height, 0, GL_RGB, GL_UNSIGNED_BYTE, tex_bytes);
    }

    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(tex_bytes);
}

void Model3D::Draw(glm::mat4 transform_matrix, glm::mat4 view_matrix, glm::mat4 projection_matrix,
    glm::vec3 light_pos, glm::vec3 light_color, float ambient_str, glm::vec3 ambient_color,
    float spec_str, float spec_phong, glm::vec3 camera_pos)
{
    glUseProgram(this->shaderProgram);

    unsigned int transformLoc = glGetUniformLocation(this->shaderProgram, "transform");
    glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform_matrix));

    unsigned int viewLoc = glGetUniformLocation(this->shaderProgram, "view");
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view_matrix));

    unsigned int projectionLoc = glGetUniformLocation(this->shaderProgram, "projection");
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection_matrix));

    glActiveTexture(GL_TEXTURE0);
    GLuint tex0Loc = glGetUniformLocation(this->shaderProgram, "tex0");
    glBindTexture(GL_TEXTURE_2D, this->texture);
    glUniform1i(tex0Loc, 0);

    glActiveTexture(GL_TEXTURE1);
    GLuint tex1Loc = glGetUniformLocation(this->shaderProgram, "norm_tex");
    glBindTexture(GL_TEXTURE_2D, this->normal_texture);
    glUniform1i(tex1Loc, 1);

    GLuint lightAddress = glGetUniformLocation(this->shaderProgram, "lightPos");
    glUniform3fv(lightAddress, 1, glm::value_ptr(light_pos));

    GLuint lightColorAddress = glGetUniformLocation(this->shaderProgram, "lightColor");
    glUniform3fv(lightColorAddress, 1, glm::value_ptr(light_color));

    GLuint ambientStrAddress = glGetUniformLocation(this->shaderProgram, "ambientStr");
    glUniform1f(ambientStrAddress, ambient_str);

    GLuint ambientColorAddress = glGetUniformLocation(this->shaderProgram, "ambientColor");
    glUniform3fv(ambientColorAddress, 1, glm::value_ptr(ambient_color));

    GLuint specStrAddress = glGetUniformLocation(this->shaderProgram, "specStr");
    glUniform1f(specStrAddress, spec_str);

    GLuint specPhongAddress = glGetUniformLocation(this->shaderProgram, "specPhong");
    glUniform1f(specPhongAddress, spec_phong);

    GLuint cameraPosAddress = glGetUniformLocation(this->shaderProgram, "cameraPos");
    glUniform3fv(cameraPosAddress, 1, glm::value_ptr(camera_pos));

    glBindVertexArray(this->VAO);

    glDrawArrays(GL_TRIANGLES, 0, this->fullVertexData.size() / 8);
}