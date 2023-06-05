#pragma once
#ifndef MODEL_OBJECT_H
#define MODEL_OBJECT_H

#include "tiny_obj_loader.h"

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
			Model3D(std::string sMeshPath, std::string sVertPath, std::string sFragPath);

			GLuint getShaderProgram();

			void DrawModel(glm::mat4 transform_matrix, glm::mat4 view_matrix, glm::mat4 projection_matrix);
			void SetColor(const glm::vec3& color);
			void CleanUp();
		
		private:
			void LoadShaders(std::string sVertPath, std::string sFragPath);
			void LoadModel(std::string sMeshPath);
			void VertexInit();

		private:
			GLuint VAO, VBO, EBO;
			tinyobj::attrib_t attributes;
			std::vector<GLuint> mesh_indices;

			GLuint shaderProgram;
	};
}

#endif

