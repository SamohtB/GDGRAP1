#include "SkyBox.hpp"

using namespace Model;

SkyBox::SkyBox(std::string sVertPath, std::string sFragPath)
{
    LoadShaders(sVertPath, sFragPath);
    VertexInit();
    LoadTexture();
}

SkyBox::~SkyBox()
{
    glDeleteVertexArrays(1, &this->VAO);
    glDeleteBuffers(1, &this->VBO);
    glDeleteBuffers(1, &this->EBO);
}

void SkyBox::LoadShaders(std::string sVertPath, std::string sFragPath)
{
    //Vertex Shader
    std::fstream vertSrc(sVertPath);
    std::stringstream vertBuff;
    vertBuff << vertSrc.rdbuf();
    std::string vertS = vertBuff.str();
    const char* v = vertS.c_str();

    GLuint vertShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertShader, 1, &v, NULL);
    glCompileShader(vertShader);


    //Fragment Shader
    std::fstream fragSrc(sFragPath);
    std::stringstream fragBuff;
    fragBuff << fragSrc.rdbuf();
    std::string fragS = fragBuff.str();
    const char* f = fragS.c_str();

    GLuint fragShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragShader, 1, &f, NULL);
    glCompileShader(fragShader);

    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertShader);
    glAttachShader(shaderProgram, fragShader);

    glLinkProgram(shaderProgram);

    this->shaderProgram = shaderProgram;
}

void SkyBox::VertexInit()
{
    glGenVertexArrays(1, &this->VAO);
    glGenBuffers(1, &this->VBO);
    glGenBuffers(1, &this->EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(this->skyboxVertices), &this->skyboxVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GL_FLOAT), (void*)0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GL_INT) * 36, &skyboxIndices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
}

void SkyBox::LoadTexture()
{
    glGenTextures(1, &this->texture);
    glBindTexture(GL_TEXTURE_CUBE_MAP, this->texture);

    //Avoid Pixilated texture
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    //Avoid Tiling
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    for (unsigned int i = 0; i < 6; i++)
    {
        int w, h, skyCChannel;
        stbi_set_flip_vertically_on_load(false);

        unsigned char* data = stbi_load(facesSkybox[i].c_str(), &w, &h, &skyCChannel, 0);

        if (data)
        {
            glTexImage2D(
                GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                0,
                GL_RGB,
                w,
                h,
                0,
                GL_RGB,
                GL_UNSIGNED_BYTE,
                data
            );

            stbi_image_free(data);
        }
    }

    stbi_set_flip_vertically_on_load(true);
}

void SkyBox::Draw(glm::mat4 view_matrix, glm::mat4 projection_matrix)
{
    glUseProgram(this->shaderProgram);

    glm::mat4 sky_view = glm::mat4(1.0f);
    sky_view = glm::mat4(glm::mat3(view_matrix));

    unsigned int skyProjectionLoc = glGetUniformLocation(this->shaderProgram, "projection");
    glUniformMatrix4fv(skyProjectionLoc, 1, GL_FALSE, glm::value_ptr(projection_matrix));

    unsigned int skyViewLoc = glGetUniformLocation(this->shaderProgram, "view");
    glUniformMatrix4fv(skyViewLoc, 1, GL_FALSE, glm::value_ptr(sky_view));

    glBindVertexArray(VAO);
    glActiveTexture(0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, this->texture);

    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
}