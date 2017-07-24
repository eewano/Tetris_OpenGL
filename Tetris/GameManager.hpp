#ifndef GameManager_hpp
#define GameManager_hpp

#include "Vec2.hpp"
#include "Block.hpp"
#include "TetriMino.hpp"

static const int SENTINELS_COUNT = 1;
static const int FIELD_WIDTH = 10 + SENTINELS_COUNT * 2;
static const int FIELD_HEIGHT = 20 + SENTINELS_COUNT * 2;
static const int DROP_INTERVAL = 10;

static Vec2i FIELD_SIZE = { FIELD_WIDTH, FIELD_HEIGHT };
static Vec2f FIELD_BOT_LEFT = { -0.55f, -0.475f };

class GameManager
{
public:
    GameManager();
    ~GameManager();
    
public:
    void Process();
    bool IsMovable(const Block& block, int horizontal, int vertical);
    bool IsMovable(const TetriMino& mino, int horizontal, int vertical);
    void PlaceCurrent(const Block& block);
    void PlaceCurrent(const TetriMino& mino);
    bool IsGameOver();
    
public:
    bool mExists[FIELD_HEIGHT][FIELD_WIDTH];
    bool mToBeDropped = false;
    bool mToBeRotated = false;
    
private:
    int mDropCount = 0;
    bool IsLineFilled();
    void MoveLine(int from, int to);
};

#endif /* GameManager_hpp */
