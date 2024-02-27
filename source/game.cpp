#include "headers/game.h"
#include "headers/resource_manager.h"
#include "headers/sprite_renderer.h"
#include "headers/particle_generator.h"
#include "irrklang/irrKlang.h"
using namespace irrklang;

//#include "headers/sub_texture.h"
#include <iostream>
#include "headers/random_numbers.h"


// Game-related State data
SpriteRenderer  *Renderer;
Snake* SnakePlayer;
GameObject* Apple; 
GameObject* Barrier;
ParticleGenerator* Particles;
ISoundEngine* SoundEngine = createIrrKlangDevice();

int screenWidth, screenHeight;
float movementTimer = 0.0f;
const float MOVEMENT_INTERVAL = 0.5f;

Game::Game(unsigned int width, unsigned int height) : State(GAME_MENU), Keys(), Width(width), Height(height) { }

Game::~Game()
{
    delete Renderer;
    Snake* currentSegment = SnakePlayer;
    while (currentSegment != nullptr) {
        Snake* nextSegment = currentSegment->next;
        delete currentSegment;
        currentSegment = nextSegment;
    }
    delete Particles;
}

void Game::Restart()
{
    ResetPlayer();
    Respawn(Apple);
    this->State = GAME_ACTIVE;
}


void Game::Init()
{
    //-----SHADERS-----//
    // load shaders
    ResourceManager::LoadShader("source/resource/shaders/sprite.vs", "source/resource/shaders/sprite.fs", nullptr, "sprite");
    ResourceManager::LoadShader("source/resource/shaders/particle.vs", "source/resource/shaders/particle.fs", nullptr, "particles");

    glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(this->Width), static_cast<float>(this->Height), 0.0f, -1.0f, 1.0f);
    
    ResourceManager::GetShader("sprite").Use().SetInteger("image", 0);
    ResourceManager::GetShader("sprite").SetMatrix4("projection", projection);

    ResourceManager::GetShader("particles").Use().SetInteger("particles", 0);
    ResourceManager::GetShader("particles").SetMatrix4("projection", projection);

    //------TEXTURES-----//
    ResourceManager::LoadTexture("source/resource/textures/background.jpg", false, "background");
    ResourceManager::LoadTexture("source/resource/textures/awesome.png", true, "ball");
    ResourceManager::LoadTexture("source/resource/textures/block.png", false, "block");
    ResourceManager::LoadTexture("source/resource/textures/block_solid.png", false, "block_solid");
    ResourceManager::LoadTexture("source/resource/textures/paddle.png", true, "paddle");
    ResourceManager::LoadTexture("source/resource/textures/particle.png", true, "particle");
    ResourceManager::LoadTexture("source/resource/textures/snake-graphics.png", true, "snakeAtlas");

    // set render-specific controls
    Shader shader = ResourceManager::GetShader("sprite");
    Renderer = new SpriteRenderer(shader);
    Shader Pshader = ResourceManager::GetShader("particles");//to-do: use particles
    Particles = new ParticleGenerator(Pshader, ResourceManager::GetTexture("particle"), 500);

    //----game objects----//
    ResetPlayer();
    glm::vec2 size = glm::vec2(50.0f, 50.0f);
    glm::vec2 Pos = glm::vec2(this->Width / 2.0f - BASE_UNIT * 1 - size.x / 2.0f, this->Height / 2.0f - BASE_UNIT * 1 - size.y / 2.0f);
    Apple = new GameObject(Pos, size, ResourceManager::GetTexture("snakeAtlas"), glm::vec3(1.0f), glm::vec2(0, 0), glm::vec2(0 * 64, 0 * 64));

    SoundEngine->setSoundVolume(0.01f);
    SoundEngine->play2D("source/resource/audio/snakeTheme.mp3", true);
}

void Game::Update(float dt, GLFWwindow* window)
{
    //window meret lekerdezes, szeretnem ha elegansabb lenne de egyenlore eleg
    glfwGetFramebufferSize(window, &screenWidth, &screenHeight);
    this->Width = screenWidth;
    this->Height = screenHeight;

    switch (this->State) {
    case GAME_MENU:
        std::cout << "Menu: Press Enter to start." << std::endl;
        break;

    case GAME_ACTIVE:
        movementTimer += dt;//diszkret mozgas

        if (movementTimer >= MOVEMENT_INTERVAL)
        {
            SnakePlayer->Move();
            movementTimer = 0;
        }
        this->DoCollisions();
        break;

    case GAME_WIN:
        std::cout << "You won: Press Enter to start." << std::endl;
        break;

    case GAME_LOSE:
        std::cout << "You lose: Press Enter to start." << std::endl;
        break;
    }
}

void Game::ProcessInput(float dt)
{
    if (this->State == GAME_ACTIVE)
    {
        // kigyo mozgatas, ha gomb megnyomva, es nem arra megy mar, es nincs arra teste
        if (this->Keys[GLFW_KEY_A] && (SnakePlayer->Velocity.x == 0))
        {
            SnakePlayer->Velocity.x = -1;
            SnakePlayer->Velocity.y = 0;
        }
        if (this->Keys[GLFW_KEY_D] && (SnakePlayer->Velocity.x == 0))
        {
            SnakePlayer->Velocity.x = 1;
            SnakePlayer->Velocity.y = 0;
        }

        if (this->Keys[GLFW_KEY_W] && (SnakePlayer->Velocity.y == 0))
        {
            SnakePlayer->Velocity.y = -1;
            SnakePlayer->Velocity.x = 0;
        }
        if (this->Keys[GLFW_KEY_S] && (SnakePlayer->Velocity.y == 0))
        {
            SnakePlayer->Velocity.y = 1;
            SnakePlayer->Velocity.x = 0;
        }
        //rotate sprites
    }
    else {
        if (this->Keys[GLFW_KEY_ENTER])
        {
            std::cout << "enterPressed";
            Restart();
        }
    }
}

void Game::RenderGrid() const
{
    // Set grid color
    glm::vec3 gridColor(0.5f, 0.5f, 0.5f);

    for (int x = 0; x < this->Width; x += BASE_UNIT)
    {
        for (int y = 0; y < this->Height; y += BASE_UNIT)
        {
            // Draw a square at each grid position
            Renderer->DrawSprite(ResourceManager::GetTexture("block"), glm::vec2(x, y), glm::vec2(static_cast<float>(BASE_UNIT)), 0.0f, gridColor);
        }
    }
}

void Game::Render() const
{
    RenderGrid();

    float texCoords2[] = {
        // pos      // tex
        0.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 0.0f,

        0.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 1.0f, 1.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f
    };
    Renderer->SetTextureCoordinates(texCoords2, sizeof(texCoords2) / sizeof(float));
    if (this->State == GAME_ACTIVE)
    {
        Renderer->DrawSprite(ResourceManager::GetTexture("background"), glm::vec2(0.0f, 0.0f), glm::vec2(this->Width, this->Height), 0.0f, glm::vec3(1.0f));
        RenderGrid();
        Apple->Draw(*Renderer);
        SnakePlayer->Draw(*Renderer);
    }
}

void Game::ResetPlayer()
{
    // Delete the snake and all its segments
    Snake* currentSegment = SnakePlayer;
    while (currentSegment != nullptr) {
        Snake* nextSegment = currentSegment->next;
        delete currentSegment;
        currentSegment = nextSegment;
    }

    // Create a new snake
    glm::vec2 playerPos = glm::vec2(this->Width / 2.0f - PLAYER_SIZE.x / 2.0f, this->Height / 2 - PLAYER_SIZE.y / 2.0f);
    SnakePlayer = new Snake(playerPos, PLAYER_SIZE, ResourceManager::GetTexture("snakeAtlas"), ResourceManager::GetTexture("snakeAtlas"), glm::vec3(1.0f), glm::vec2(1, 0), 0.0f);
    SnakePlayer->Grow();
    SnakePlayer->Grow();
}

// collision detection
Collision CheckCollision(GameObject& one, GameObject& two) // AABB - AABB collision
{
    // collision x-axis?
    bool collisionX = one.Position.x + one.Size.x >= two.Position.x &&
        two.Position.x + two.Size.x >= one.Position.x;
    // collision y-axis?
    bool collisionY = one.Position.y + one.Size.y >= two.Position.y &&
        two.Position.y + two.Size.y >= one.Position.y;
    // collision only if on both axes

    if (collisionX && collisionY) {
        glm::vec2 difference = { 0.0f, 0.0f };
        if (one.Position.x < two.Position.x) {
            difference.x = two.Position.x - (one.Position.x + one.Size.x);
        }
        else {
            difference.x = one.Position.x - (two.Position.x + two.Size.x);
        }
        if (one.Position.y < two.Position.y) {
            difference.y = two.Position.y - (one.Position.y + one.Size.y);
        }
        else {
            difference.y = one.Position.y - (two.Position.y + two.Size.y);
        }
        //Direction direction = VectorDirection(difference);
        return std::make_tuple(true, UP, difference);
    }

    return std::make_tuple(false, Direction::UP, glm::vec2(0.0f, 0.0f));
}

template<typename Obj>
void Game::Respawn(Obj*& obj)
{
    Collision collision;

        
    bool isCollision = true;

    while (isCollision) //amig utkoznek
    {
        glm::vec2 random = glm::vec2(getRandomInt(2, int(Width/BASE_UNIT) - 1),getRandomInt(2, int(Height/BASE_UNIT) - 1));
        glm::vec2 Pos = glm::vec2(BASE_UNIT * random.x + BASE_UNIT/2 - obj->Size.x/2.0f, BASE_UNIT * random.y + BASE_UNIT / 2 - obj->Size.y / 2.0f);
        obj->Position = Pos;
        //std::cout << random.x << "randY:" << random.y<<std::endl;
        isCollision = false;
        Snake* currentSegment = SnakePlayer;
        while (currentSegment != nullptr) {
            collision = CheckCollision(*currentSegment, *obj);
            if (std::get<0>(collision))
            {
                isCollision = true;
                break;
            }
            currentSegment = currentSegment->next;
        }
    }
}

void Game::DoCollisions()
{
    Collision collision = CheckCollision(*SnakePlayer, *Apple);//csak a head es az apple kozott nezi a collisiont
    if (std::get<0>(collision))
    {
        SnakePlayer->Grow();
        //Apple->Respawn();
        Respawn(Apple);
        SoundEngine->play2D("source/resource/audio/bleep.mp3", false);
    }
    if (SnakePlayer->CheckSelfCollision())
    {
        this->State = GAME_LOSE;
    }
}