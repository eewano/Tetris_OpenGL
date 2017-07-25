#ifndef TetriMino_hpp
#define TetriMino_hpp

#include <memory>
#include <cassert>
#include "Block.hpp"
#include "Vec2.hpp"
#include "Vec4.hpp"

struct BlockType;

static const int BLOCK_TYPE_MAX = 7;
static const int BLOCK_MAX = 4;

class TetriMino
{
public:
    TetriMino();
    TetriMino(Vec2i aPos); //aPos.x:7 aPos.y:24
    ~TetriMino();
    
public:
    void SetType(int type);
    void SetPosition(const Vec2i& aPos);
    void ResetAndNext(int type);
    void UpdateNextType();
    void Move(const Vec2i& amount);
    void RotateLeft();
    void RotateRight();
    void Draw(int texId);
    
    int GetNextType() const;
    
public:
    static BlockType blockTypes[BLOCK_TYPE_MAX];
    Vec2i mPosition;
    std::unique_ptr<Block> mBlocks[BLOCK_MAX];
    int mType;
    int mNextType;
};

struct BlockType
{
    Vec2i offset[BLOCK_MAX];
    Vec4f color;
};

#endif /* TetriMino_hpp */
