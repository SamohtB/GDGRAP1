#pragma once
#ifndef MODEL_OBJECT_H
#define MODEL_OBJECT_H

#include "tiny_obj_loader.h"
#include "stb_image.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>
#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>

namespace Model
{

	class Model3D
	{
		public:
			Model3D(std::string sMeshPath, std::string sVertPath, std::string sFragPath, std::string sTexPath);

			void DrawModel(glm::mat4 transform_matrix, glm::mat4 view_matrix, glm::mat4 projection_matrix, 
				glm::vec3 lightPos, glm::vec3 lightColor, float ambientStr, glm::vec3 ambientColor, 
				glm::vec3 cameraPos, float specStr, float specPhong);
			void CleanUp();
		
		private:
			void LoadShaders(std::string sVertPath, std::string sFragPath);
			void LoadTexture(std::string sTexPath);
			void LoadModel(std::string sMeshPath);
			void VertexInit();

		private:
			GLuint VAO, VBO, VBO_UV, EBO;
			tinyobj::attrib_t attributes;
			std::vector<GLuint> mesh_indices;
			std::vector<GLfloat> fullVertexData;

			GLuint shaderProgram;

			int img_width;
			int img_height;
			int colorChannels;
			GLuint texture;

			GLfloat UV[16]{
				0.f, 1.f,
				0.f, 0.f,
				1.f, 1.f,
				1.f, 0.f,
				1.f, 1.f,
				1.f, 0.f,
				0.f, 1.f,
				0.f, 0.f
			};
	};
}

#endif

