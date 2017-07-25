#include "TetriMino.hpp"
#include "GameManager.hpp"
#include "XorShift.hpp"

extern XorShift xorShift;

BlockType TetriMino::blockTypes[BLOCK_TYPE_MAX] =
{
    //minoTypes[0] 凸タイプ ピンク
    { { Vec2i{ 0, 0 }, Vec2i{ 0, -1 }, Vec2i{ -1, 0 }, Vec2i{ +1, 0 } },
        { 1.0f, 0.0f, 1.0f, 1.0f }, true },
    
    //minoTypes[1] 棒タイプ ライトブルー
    { { Vec2i{ 0, 0 }, Vec2i{ 0, -1 }, Vec2i{ 0, -2 }, Vec2i{ 0, +1 } },
        { 0.0f, 1.0f, 1.0f, 1.0f }, true },
    
    //minoTypes[2] 四角タイプ イエロー
    { { Vec2i{ 0, 0 }, Vec2i{ +1, 0 }, Vec2i{ 0, -1 }, Vec2i{ +1, -1 } },
        { 1.0f, 1.0f, 0.0f, 1.0f }, false },
    
    //minoTypes[3] Lタイプ ホワイト
    { { Vec2i{ 0, 0 }, Vec2i{ 0, +1 }, Vec2i{ 0, -1 }, Vec2i{ +1, -1 } },
        { 1.0f, 1.0f, 1.0f, 1.0f }, true },
    
    //minoTypes[4] 逆Lタイプ ブルー
    { { Vec2i{ 0, 0 }, Vec2i{ 0, +1 }, Vec2i{ 0, -1 }, Vec2i{ -1, -1 } },
        { 0.0f, 0.0f, 1.0f, 1.0f }, true },
    
    //minoTypes[5] 右ズレタイプ イエローグリーン
    { { Vec2i{ 0, 0 }, Vec2i{ +1, 0 }, Vec2i{ 0, -1 }, Vec2i{ -1, -1 } },
        { 0.0f, 1.0f, 0.0f, 1.0f }, true },
    
    //minoTypes[6] 左ズレタイプ レッド
    { {Vec2i{ 0, 0 }, Vec2i{ -1, 0 }, Vec2i{ 0, -1 }, Vec2i{ +1, -1 } },
        { 1.0f, 0.0f, 0.0f, 1.0f }, true }
};

TetriMino::TetriMino()
{
}

TetriMino::TetriMino(Vec2i aPos)
{
    //mNextTypeは0で初期化されていないので、コンストラクタ生成時にテトリミノのタイプを
    //設定しておかないとcassertに引っ掛かる事がある
    UpdateNextType();
    
    //Blockを4つ組み合わせる
    for(size_t i = 0; i < BLOCK_MAX; i++)
    {
        mBlocks[i] = std::make_unique<Block>(0, 0);
    }
    
    mPosition = aPos;
    //最初に出現するブロックの選出と成形
    SetType(mNextType);
    SetPosition(mPosition);
    UpdateNextType();
}

TetriMino::~TetriMino()
{
}

void TetriMino::SetType(int type)
{
    //ランダムで選んだ整数は、BLOCK_TYPE_MAXよりも絶対に低い事が条件
    assert(type < BLOCK_TYPE_MAX);
    
    mType = type;
    for(size_t i = 0; i < BLOCK_MAX; i++)
    {
        mBlocks[i]->mOffset = blockTypes[type].offset[i];
        mBlocks[i]->SetColor(blockTypes[type].color);
    }
}

void TetriMino::SetPosition(const Vec2i& aPos)
{
    mPosition = aPos;
    for(size_t i = 0; i < BLOCK_MAX; i++)
    {
        mBlocks[i]->SetPosition(aPos + mBlocks[i]->mOffset);
    }
}

void TetriMino::ResetAndNext(int type)
{
    SetType(type);
    SetPosition({ FIELD_WIDTH / 2, FIELD_HEIGHT });
}

void TetriMino::UpdateNextType()
{
    mNextType = xorShift(BLOCK_TYPE_MAX);
}

void TetriMino::Move(const Vec2i &amount)
{
    mPosition += amount;
    SetPosition(mPosition);
}

void TetriMino::RotateLeft()
{
    if(blockTypes->isRotatable == true)
    {
        for(size_t i = 0; i < BLOCK_MAX; i++)
        {
            auto temp = mBlocks[i]->mOffset;
            mBlocks[i]->mOffset.x = -temp.y;
            mBlocks[i]->mOffset.y = temp.x;
        }
    }
    SetPosition(mPosition);
}

void TetriMino::RotateRight()
{
    if(blockTypes->isRotatable == true)
    {
        for(size_t i = 0; i < BLOCK_MAX; i++)
        {
            auto temp = mBlocks[i]->mOffset;
            mBlocks[i]->mOffset.x = temp.y;
            mBlocks[i]->mOffset.y = -temp.x;
        }
    }
    SetPosition(mPosition);
}

void TetriMino::Draw(int texId)
{
    for(size_t i = 0; i < BLOCK_MAX; i++)
    {
        mBlocks[i]->Draw(texId);
    }
}

int TetriMino::GetNextType() const
{
    return mNextType;
}
