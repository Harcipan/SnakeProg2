#ifndef SNAKE_OBJECT_H
#define SNAKE_OBJECT_H

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "game_object.h"
#include "texture.h"


class Snake : public GameObject {
public:
    int Width = 900;
    int Height = 900;

    Snake* next;
    glm::vec2 PrevPosition;
    Texture2D headSprite;
    // Constructor(s)
    Snake();
    Snake(glm::vec2 pos, glm::vec2 size, Texture2D sprite, Texture2D head, glm::vec3 color = glm::vec3(1.0f), glm::vec2 velocity = glm::vec2(1.0f, 0.0f), float rotation = 0.0f);
    ~Snake();
    // Functions specific to Snake
    void Move();
    void Grow();
    void Draw(SpriteRenderer& renderer);
    bool CheckSelfCollision();
    //Snake* Respawn();
};
#endif
