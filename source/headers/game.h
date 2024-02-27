#ifndef GAME_H
#define GAME_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "headers/game_object.h"
#include "headers/snake_object.h"

// Represents the current state of the game
enum GameState {
	GAME_ACTIVE,
	GAME_MENU,
	GAME_WIN, 
    GAME_LOSE
};


// Represents the four possible (collision) directions
enum Direction {
	UP,
	RIGHT,
	DOWN,
	LEFT
};
// Defines a Collision typedef that represents collision data
typedef std::tuple<bool, Direction, glm::vec2> Collision; // <collision?, what direction?, difference vector center - closest point>


const float BASE_UNIT(60.0f);

const glm::vec2 PLAYER_SIZE(BASE_UNIT, BASE_UNIT);



class Game
{
public:
    // game state
    GameState               State;
    bool                    Keys[1024];
    int                     Width, Height;
    // constructor/destructor
    Game(unsigned int width, unsigned int height);
    ~Game();
    // initialize game state (load all shaders/textures/levels)
    void Init();
    // game loop
    void Update(float dt, GLFWwindow* window);
    void ProcessInput(float dt);
    void Render() const;
    void RenderGrid() const;
    void DoCollisions();
    template<typename Obj>
    void Respawn(Obj*& obj);
    void Restart();
    void ResetPlayer();
};


#endif