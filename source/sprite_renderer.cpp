#include "headers/sprite_renderer.h"
#include <iostream>

SpriteRenderer::SpriteRenderer(Shader& shader)
{
    this->shader = shader;
    this->initRenderData();
}

SpriteRenderer::~SpriteRenderer()
{
    glDeleteVertexArrays(1, &this->quadVAO);
}

void SpriteRenderer::DrawSprite(Texture2D texture, glm::vec2 position, glm::vec2 size, float rotate, glm::vec3 color, glm::vec2 atlasOffset)
{
    // prepare transformations
    this->shader.Use();
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(position, 0.0f));  // first translate (transformations are: scale happens first, then rotation, and then final translation happens; reversed order)

    model = glm::translate(model, glm::vec3(0.5f * size.x, 0.5f * size.y, 0.0f)); // move origin of rotation to center of quad
    model = glm::rotate(model, glm::radians(rotate), glm::vec3(0.0f, 0.0f, 1.0f)); // then rotate
    model = glm::translate(model, glm::vec3(-0.5f * size.x, -0.5f * size.y, 0.0f)); // move origin back

    model = glm::scale(model, glm::vec3(size, 1.0f)); // last scale

    this->shader.SetMatrix4("model", model);

    // render textured quad
    this->shader.SetVector3f("spriteColor", color);

    glActiveTexture(GL_TEXTURE0);
    texture.Bind();

    glBindVertexArray(this->quadVAO);

    if (atlasOffset.x != -1 || atlasOffset.y!=-1)
    {
        glm::vec2 atlasSize = glm::vec2(192, 192);//should be automatic

        // Calculate texture coordinates based on atlas offset and size
        float texLeft = atlasOffset.x / atlasSize.x;
        float texRight = (atlasOffset.x + 64) / atlasSize.x;
        float texTop = atlasOffset.y / atlasSize.y;
        float texBottom = (atlasOffset.y + 64) / atlasSize.y;
        float texCoords[] = {
            // pos      // tex
            0.0f, 1.0f, texLeft, texBottom,
            1.0f, 0.0f, texRight, texTop,
            0.0f, 0.0f, texLeft, texTop,

            0.0f, 1.0f, texLeft, texBottom,
            1.0f, 1.0f, texRight, texBottom,
            1.0f, 0.0f, texRight, texTop
        };
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(texCoords), texCoords, GL_STATIC_DRAW);
    }





    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}

void SpriteRenderer::SetTextureCoordinates(float texCoords[], int size)
{
    glBindVertexArray(this->quadVAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float)*size, texCoords, GL_STATIC_DRAW);
    glBindVertexArray(0);
}


void SpriteRenderer::initRenderData()
{
    // configure VAO/VBO
    //unsigned int VBO;
    
    //to-do: USE EBO
    float vertices[] = {
        // pos      // tex
        0.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 0.0f,

        0.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 1.0f, 1.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f
    };

    glGenVertexArrays(1, &this->quadVAO);
    glBindVertexArray(this->quadVAO);

    //unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);  // unbind VAO
}