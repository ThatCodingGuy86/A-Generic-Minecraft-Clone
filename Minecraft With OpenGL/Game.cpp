#include "Game.h"


    // game state
GameState    State;
bool         Keys[1024];
unsigned int Width, Height;

Shader shader();
Shader textShader();

// constructor/destructor
Game(unsigned int width, unsigned int height)
    {
        Width = width;
        Height = height;

        std::cout << "Initilizing Game class\n";
    }
    ~Game()
    {

    }
    // initialize game state (load all shaders/textures/levels)
    void Init()
    {

    }
    // game loop
    void ProcessInput(float dt, int mouseX, int mouseY, int zoomLevel)
    {

    }
    void Update(float dt)
    {

    }
    void Render()
    {
    }
};

Game::Game(unsigned int width, unsigned int height)
    : State(GAME_ACTIVE), Keys(), Width(width), Height(height)
{

}

Game::~Game()
{

}

void Game::Init()
{

}

void Game::Update(float dt)
{

}

void Game::ProcessInput(float dt)
{

}

void Game::Render()
{

}