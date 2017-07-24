#ifndef Block_hpp
#define Block_hpp

#include "Sprite.hpp"

static constexpr int BLOCK_VERTS_COUNT = 4;
static float BLOCK_SIDE_LENGTH = 0.09f;
static Vec2f BLOCK_SIZE = { BLOCK_SIDE_LENGTH, BLOCK_SIDE_LENGTH };

class Block : public Sprite<BLOCK_VERTS_COUNT>
{
public:
    Block();
    Block(Vec2f aPos);
    Block(int aX, int aY);
    ~Block();
    
public:
    void Move(const Vec2i& amount);
    void SetPosition(const Vec2i& aPos);
    
private:
    void InitInfo(Vec2f aPos, Vec2f aSize);
    void ApplyPosition();
    
public:
    Vec2i mPosition{};
    Vec2i mOffset{};
};

#endif /* Block_hpp */
