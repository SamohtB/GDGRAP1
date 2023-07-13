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

			void DrawSkybox(glm::mat4 viewMatrix, glm::mat4 projectionMatrix);
			void CleanUp();
		
		private:
			GLuint LoadShaders(std::string sVertPath, std::string sFragPath);
			void LoadTexture(std::string sTexPath);
			void LoadSkyboxTexture();
			void LoadModel(std::string sMeshPath);
			void SkyboxInit();
			void VertexInit();

		private:
			GLuint VAO, VBO, VBO_UV;
			GLuint skybox_VAO, skybox_VBO, skybox_EBO;
			tinyobj::attrib_t attributes;
			std::vector<GLuint> mesh_indices;
			std::vector<GLfloat> fullVertexData;

			GLuint shaderProgram;
			GLuint skyboxProgram;

			int img_width;
			int img_height;
			int colorChannels;
			GLuint texture;

			unsigned int skyboxTexture;

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

			/*
				  7--------6
				 /|       /|
				4--------5 |
				| |      | |
				| 3------|-2
				|/       |/
				0--------1
			*/
			//Vertices for the cube
			float skyboxVertices[24]{
				-1.f, -1.f, 1.f, //0
				1.f, -1.f, 1.f,  //1
				1.f, -1.f, -1.f, //2
				-1.f, -1.f, -1.f,//3
				-1.f, 1.f, 1.f,  //4
				1.f, 1.f, 1.f,   //5
				1.f, 1.f, -1.f,  //6
				-1.f, 1.f, -1.f  //7
			};

			//Skybox Indices
			unsigned int skyboxIndices[36]{
				1,2,6,
				6,5,1,

				0,4,7,
				7,3,0,

				4,5,6,
				6,7,4,

				0,3,2,
				2,1,0,

				0,1,5,
				5,4,0,

				3,7,6,
				6,2,3
			};

			std::string facesSkybox[6]{
				"Skybox/rainbow_rt.png",
				"Skybox/rainbow_lf.png",
				"Skybox/rainbow_up.png",
				"Skybox/rainbow_dn.png",
				"Skybox/rainbow_ft.png",
				"Skybox/rainbow_bk.png"
			};
	};
}

#endif

