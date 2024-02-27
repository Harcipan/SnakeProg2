#include "headers/game_object.h"

#include <iostream>

GameObject::GameObject() 
    : Position(0.0f, 0.0f), Size(1.0f, 1.0f), Velocity(0.0f), Color(1.0f), Rotation(0.0f), Sprite(), IsSolid(false), Destroyed(false), SubTexture(0.0f) { }

GameObject::GameObject(glm::vec2 pos, glm::vec2 size, Texture2D sprite, glm::vec3 color, glm::vec2 velocity, glm::vec2 subtexture, float rotation) 
    : Position(pos), Size(size), Velocity(velocity), Color(color), Rotation(rotation), Sprite(sprite), IsSolid(false), Destroyed(false), SubTexture(subtexture) { }

void GameObject::Draw(SpriteRenderer &renderer)
{
    renderer.DrawSprite(this->Sprite, this->Position, this->Size, this->Rotation, this->Color, SubTexture);//glm::vec2(0 * 64, 3 * 64)
}
/*
void GameObject::Respawn() {


    int Width = 900, Height = 900;
    int BASE_UNIT = 60;


    int random = rand() % 10 / 2;
    glm::vec2 Pos = glm::vec2(Width / 2.0f - BASE_UNIT * random - this->Size.x / 2.0f, Height / 2.0f - BASE_UNIT * random - this->Size.y / 2.0f);

    Collision collision = CheckCollision(*SnakePlayer, *Apple);
    while (std::get<0>(collision))
    {
        int random = rand() % 10 / 2;
        glm::vec2 Pos = glm::vec2(Width / 2.0f - BASE_UNIT * random - this->Size.x / 2.0f, Height / 2.0f - BASE_UNIT * random - this->Size.y / 2.0f);
    }

    this->Position = Pos;
}*/