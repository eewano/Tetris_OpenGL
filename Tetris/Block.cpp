#include "Block.hpp"
#include <iostream>
#include "GameManager.hpp"

Block::Block()
{
    std::cout << "Block default constructor called.\n";
}

Block::Block(Vec2f aPos)
{
    InitInfo(BLOCK_SIZE, aPos);
}

Block::Block(int aX, int aY)
{
    mPosition = { aX, aY };
    Vec2f pos = { FIELD_BOT_LEFT.x + BLOCK_SIZE.x / 2 * aX,
        FIELD_BOT_LEFT.y + BLOCK_SIZE.y / 2 * aY };
    
    InitInfo(pos, BLOCK_SIZE);
}

Block::~Block()
{
}

void Block::Move(const Vec2i &amount)
{
    mPosition += amount;
    ApplyPosition();
}

void Block::SetPosition(const Vec2i& aPos)
{
    mPosition = aPos;
    ApplyPosition();
}

void Block::InitInfo(Vec2f aPos, Vec2f aSize)
{
    vertex[0] = geometry[0] = { -aSize.x / 2, +aSize.y / 2 };
    vertex[1] = geometry[1] = { +aSize.x / 2, +aSize.y / 2 };
    vertex[2] = geometry[2] = { +aSize.x / 2, -aSize.y / 2 };
    vertex[3] = geometry[3] = { -aSize.x / 2, -aSize.y / 2 };
    
    uv[0] = { 0.0, 1.0 };
    uv[1] = { 1.0, 1.0 };
    uv[2] = { 1.0, 0.0 };
    uv[3] = { 0.0, 0.0 };
    
    pos = aPos;
    size = aSize * 0.5f;
}

void Block::ApplyPosition()
{
    pos = { FIELD_BOT_LEFT.x + BLOCK_SIZE.x / 2 * mPosition.x,
        FIELD_BOT_LEFT.y + BLOCK_SIZE.y / 2 * mPosition.y };
    
    for(size_t i = 0; i < BLOCK_VERTS_COUNT; i++)
    {
        geometry[i] = pos + vertex[i];
    }
}
