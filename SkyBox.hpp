#pragma once
#ifndef MODEL_SKYBOX_HPP
#define MODEL_SKYBOX_HPP

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
	class SkyBox
	{
		public:
			SkyBox(std::string strVertPath, std::string strFragPath);
			~SkyBox();
			void Draw(glm::mat4 view_matrix, glm::mat4 projection_matrix);

		private:
			void LoadShaders(std::string sVertPath, std::string sFragPath);
			void LoadTexture();
			void VertexInit();

		private:
			GLuint VAO, VBO, EBO;
			GLuint shaderProgram;
			GLuint texture;

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
			const float skyboxVertices[24]{
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
			const  unsigned int skyboxIndices[36]{
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

			const std::string facesSkybox[6]{
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
