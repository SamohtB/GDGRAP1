#include "Game.hpp"

using namespace Controller;

Game::Game()
{
    Initialize();
    bunny = new Model3D("3D/Mesh/plane.obj", "Shaders/sample.vert", "Shaders/sample.frag", "3D/Texture/brickwall.jpg",
        "3D/Texture/brickwall_normal.jpg");
    skybox = new SkyBox("Shaders/skybox.vert", "Shaders/skybox.frag");

    translate = glm::vec3(0.0f, 0.0f, 0.0f);
    scale = glm::vec3(4.f, 4.f, 4.f);

    pitchTheta = 0.0f;
    yawTheta = 0.0f;
    rollTheta = -90.0f;
    FOV = 60.0f;

    cameraPos = glm::vec3(0.0f, 0.0f, 10.0f);
    cameraCenter = glm::vec3(0.0f, 0.0f, 0.0f);

    lightPos = glm::vec3(0.0f, 3.0f, 10.0f);
    lightColor = glm::vec3(1.0f, 1.0f, 1.0f);

    ambientStr = 0.2f;
    ambientColor = lightColor;

    specStr = 0.5f;
    specPhong = 16;
}

void Game::Initialize()
{
    if (!glfwInit())
    {
        exit(-1);
    }

    gameWindow = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Thomas Banatin", NULL, NULL);

    if (!gameWindow)
    {
        glfwTerminate();
        exit(-1);
    }

    glfwMakeContextCurrent(gameWindow);
    gladLoadGL();

    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
}

void Game::Run()
{
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    glfwSetTime(0.0f);
    double tLastUpdate = 0.0f;
    float tTimePerFrame = 1.f / FRAME_RATE_LIMIT;

    while (!glfwWindowShouldClose(gameWindow))
    {
        this->ProcessInput();
        tLastUpdate += glfwGetTime();
        glfwSetTime(0.0f);

        while (tLastUpdate > tTimePerFrame)
        {
            tLastUpdate -= tTimePerFrame;
            this->Update(tTimePerFrame);
        }

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        this->Render();

        glfwSwapBuffers(gameWindow);

        glfwPollEvents();
    }

    glfwTerminate();
}

void Game::ProcessInput()
{

}

void Game::Update(float tDeltaTime)
{
    view_matrix = glm::lookAt(cameraPos, cameraCenter, worldUp);
    projection_matrix = glm::perspective(glm::radians(FOV), (ASPECT_RATIO), 0.1f, 100.0f);

    transform_matrix = glm::translate(glm::mat4(1.0f), translate);
    transform_matrix = glm::scale(transform_matrix, scale);
    transform_matrix = glm::rotate(transform_matrix, glm::radians(pitchTheta), pitchAxis);
    transform_matrix = glm::rotate(transform_matrix, glm::radians(yawTheta), yawAxis);
    transform_matrix = glm::rotate(transform_matrix, glm::radians(rollTheta), rollAxis);

    //pitchTheta += 1.0f * tDeltaTime;
}

void Game::Render()
{
    glDepthMask(GL_FALSE);
    glDepthFunc(GL_LEQUAL);

    skybox->Draw(view_matrix, projection_matrix);

    glDepthMask(GL_TRUE);
    glDepthFunc(GL_LESS);
    
    bunny->Draw(transform_matrix, view_matrix, projection_matrix, lightPos, lightColor,
        ambientStr, ambientColor, specStr, specPhong, cameraPos);
}