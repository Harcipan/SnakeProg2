#include "headers/snake_object.h"
#include "headers/game.h"

#include <iostream>




// Constructor(s)
Snake::Snake() : GameObject(), next(nullptr), PrevPosition(glm::vec2(0))
{

}
Snake::Snake(glm::vec2 pos, glm::vec2 size, Texture2D sprite,Texture2D head, glm::vec3 color, glm::vec2 velocity, float rotation)
	: GameObject(pos, size, sprite, color, velocity, glm::vec2(0.0f),rotation)
{
    PrevPosition = pos - velocity * BASE_UNIT;
    headSprite = head;
    next = nullptr;
}

Snake::~Snake()
{
    
}

float calculateRotation(const glm::vec2& velocity) {
    // Calculate the angle of the velocity vector relative to the positive x-axis
    float angle = static_cast<float>(std::atan2(velocity.y, velocity.x) * 180.0f / 3.1415926);

    // Convert angle to positive value if negative
    if (angle < 0)
        angle += 360.0f;

    // Round the angle to the nearest 90 degrees
    float roundedAngle = std::round(angle / 90.0f) * 90.0f;

    return roundedAngle;
}

// Functions specific to Snake
void Snake::Move() {
    int screenWidth = 900;
    int screenHeight = 900;
    float offset = (BASE_UNIT / 2 - this->Size.x / 2);

    PrevPosition = Position;
    Position += BASE_UNIT * Velocity;

    if (Position.x < 0)
        Position.x = screenWidth + offset - BASE_UNIT;
    else if (Position.x > screenWidth)
        Position.x = offset;

    else if (Position.y < 0)
        Position.y = screenHeight + offset - BASE_UNIT;
    else if (Position.y > screenHeight)
        Position.y = offset;

    Snake* currentSegment = this;
    glm::vec2 prevVelocity = this->Velocity;

    currentSegment->Rotation = calculateRotation(currentSegment->Velocity);

    glm::vec2 positionS = currentSegment->PrevPosition;
    currentSegment = currentSegment->next;
    while (currentSegment != nullptr) {
        currentSegment->PrevPosition = currentSegment->Position;
        currentSegment->Position = positionS;
        positionS = currentSegment->PrevPosition;

        //update velocity (fejtol lefele minden lepes utan)
        glm::vec2 temp = currentSegment->Velocity;
        currentSegment->Velocity = prevVelocity;
        prevVelocity = temp;

        currentSegment->Rotation = calculateRotation(currentSegment->Velocity);

        currentSegment = currentSegment->next;
    }
}

void Snake::Grow() {
    // Find the last segment
    Snake* lastSegment = this;
    while (lastSegment->next != nullptr) {
        lastSegment = lastSegment->next;
    }

    Snake* newSegment = new Snake(lastSegment->PrevPosition, lastSegment->Size, lastSegment->Sprite, lastSegment->headSprite, lastSegment->Color, lastSegment->Velocity, calculateRotation(lastSegment->Velocity));
    // Link the new segment to the end of the snake

    lastSegment->next = newSegment;
}


void Snake::Draw(SpriteRenderer& renderer) {
    Snake* currentSegment = this;
    //head
    renderer.DrawSprite(currentSegment->headSprite, currentSegment->Position, currentSegment->Size, currentSegment->Rotation, currentSegment->Color, glm::vec2(1*64, 1*64));
    currentSegment = currentSegment->next;
    //body
    while (currentSegment->next != nullptr) {
        if(currentSegment->next->Rotation == currentSegment->Rotation)
            renderer.DrawSprite(currentSegment->Sprite, currentSegment->Position, currentSegment->Size, currentSegment->Rotation, currentSegment->Color, glm::vec2(1*64,0*64));
        else
        {
            //std::cout << "kov rot" << currentSegment->next->Rotation << "jelenlegi rot" << currentSegment->Rotation << std::endl;
            if (currentSegment->next->Rotation-90 == currentSegment->Rotation || currentSegment->next->Rotation == int(currentSegment->Rotation + 90) % 360)
                renderer.DrawSprite(currentSegment->Sprite, currentSegment->Position, currentSegment->Size, currentSegment->Rotation, currentSegment->Color, glm::vec2(0 * 64, 1 * 64));
            else
                renderer.DrawSprite(currentSegment->Sprite, currentSegment->Position, currentSegment->Size, currentSegment->Rotation + 90, currentSegment->Color, glm::vec2(0 * 64, 1 * 64));
        }
        currentSegment = currentSegment->next;
    }
    //tail
    renderer.DrawSprite(currentSegment->Sprite, currentSegment->Position, currentSegment->Size, currentSegment->Rotation, currentSegment->Color, glm::vec2(2* 64,0 * 64));
}


bool Snake::CheckSelfCollision() {
    Snake* currentSegment = this;
    while (currentSegment->next != nullptr) {
        Snake* movingSegment = currentSegment->next;
        while (movingSegment != nullptr) {
            if (currentSegment->Position == movingSegment->Position)
                return true;
            movingSegment = movingSegment->next;
        }
        currentSegment = currentSegment->next;
    }
    return false;
}

//resetplayer helyett, (felesleges bonyolitas?)
#if 0
Snake* Snake::Respawn()
{
    // Delete the snake and all its segments
    Snake* currentSegment = this;
    while (currentSegment != nullptr) {
        Snake* nextSegment = currentSegment->next;
        delete currentSegment;
        currentSegment = nextSegment;
    }

    // Create a new snake
    glm::vec2 playerPos = glm::vec2(this->Width / 2.0f - PLAYER_SIZE.x / 2.0f, this->Height / 2 - PLAYER_SIZE.y / 2.0f);
    Snake* head = new Snake(playerPos, PLAYER_SIZE, this->headSprite, this->Sprite, glm::vec3(1.0f), glm::vec2(1, 0));
    head->Grow();
    head->Grow();
    return head;
}
#endif