#pragma once
#ifndef CONTROLLER_GAME_HPP
#define CONTROLLER_GAME_HPP

#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include <iostream>
#include <cmath>

#include "Model3D.hpp"
#include "SkyBox.hpp"

namespace Controller
{
	using namespace Model;

	class Game
	{
		public:
			Game();
			void Run();
		
		private:
			/* GameObjects */
			Model3D* bunny;
			SkyBox* skybox;

		private:
			void Initialize();
			void ProcessInput();
			void Update(float tDeltaTime);
			void Render();

		private:
			/* Game Window Properties */
			GLFWwindow* gameWindow;
			const float SCREEN_WIDTH = 600.0f;
			const float SCREEN_HEIGHT = 600.0f;
			const float FRAME_RATE_LIMIT = 60.0f;
			const float ASPECT_RATIO =  SCREEN_HEIGHT / SCREEN_WIDTH;

			/* Transform Properties */
			glm::vec3 translate = glm::vec3(0.0f, 0.0f, -5.0f);
			glm::vec3 scale = glm::vec3(0.1f, 0.1f, 0.1f);

			const glm::vec3 pitchAxis = glm::vec3(1.0f, 0.0f, 0.0f);
			const glm::vec3 yawAxis = glm::vec3(0.0f, 1.0f, 0.0f);

			float pitchTheta = 0.0f;
			float yawTheta = 0.0f;
			float FOV = 60.0f;

			/* Camera Properties */
			glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 10.0f);
			glm::vec3 cameraCenter = glm::vec3(0.0f, 0.0f, 0.0f);
			const glm::vec3 worldUp = glm::normalize(glm::vec3(0.0f, 1.0f, 0.0f));

			/* MATRICES */
			glm::mat4 transform_matrix;
			glm::mat4 view_matrix;
			glm::mat4 projection_matrix;

			/* Light Properties */
			glm::vec3 lightPos = glm::vec3(-10, 3, 0);
			glm::vec3 lightColor = glm::vec3(1.0f, 1.0f, 1.0f);

			float ambientStr = 0.2f;
			glm::vec3 ambientColor = lightColor;

			float specStr = 0.5f;
			float specPhong = 16;
	};
}


#endif // !SYSTEM_GAME_H



