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
			~Model3D();

			void Draw(glm::mat4 transform_matrix, glm::mat4 view_matrix, glm::mat4 projection_matrix,
				glm::vec3 light_pos, glm::vec3 light_color, float ambient_str, glm::vec3 ambient_color, 
				float spec_str, float spec_phong, glm::vec3 camera_pos);
		
		private:
			void LoadShaders(std::string sVertPath, std::string sFragPath);
			void LoadTexture(std::string sTexPath);
			void LoadModel(std::string sMeshPath);
			void VertexInit();

		private:
			GLuint VAO, VBO;
			std::vector<GLfloat> fullVertexData;

			GLuint shaderProgram;
			GLuint texture;
	};
}

#endif

