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

enum Axis {X =0, Y, Z};

namespace Model
{

	class Model3D
	{
		public:
			Model3D(std::string sMeshPath, std::string sVertPath, std::string sFragPath);

			void DrawModel(glm::mat4 view_matrix, glm::mat4 projection_matrix);
			void SetTransform(glm::vec3 translate, float pitch, float yaw, float roll, glm::vec3 scale);
			void CleanUp();
		
		private:
			glm::mat4 BuildTransformMatrix();
			void LoadShaders(std::string sVertPath, std::string sFragPath);
			void LoadModel(std::string sMeshPath);
			void VertexInit();

		private:
			GLuint VAO, VBO, EBO;
			tinyobj::attrib_t attributes;
			std::vector<GLuint> mesh_indices;

			float pos_x;
			float pos_y;
			float pos_z;
			float rot_pitch;
			float rot_yaw;
			float rot_roll;
			float scl_x;
			float scl_y;
			float scl_z;

			GLuint shaderProgram;
	};
}

#endif

