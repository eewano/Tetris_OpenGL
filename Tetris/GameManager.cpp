#include "GameManager.hpp"

GameManager::GameManager()
{
    //各マスにブロックが存在するかの初期設定
    //左下(j=0,i=0)から右上(j=12,i=22)に向かって処理される
    for(size_t i = 0; i < FIELD_HEIGHT; i++)
    {
        for(size_t j = 0; j < FIELD_WIDTH; j++)
        {
            //FIELD_WIDTH:12 SENTINELS_COUNT:1
            if(j >= FIELD_WIDTH - SENTINELS_COUNT ||
               j < SENTINELS_COUNT ||
               i < SENTINELS_COUNT)
            {
                mExists[i][j] = true;
            }
            else
            {
                mExists[i][j] = false;
            }
            
            /*別の書き方*/
//            mExists[i][j] = j >=
//            FIELD_WIDTH - SENTINELS_COUNT ||
//            j < SENTINELS_COUNT ||
//            i < SENTINELS_COUNT;
        }
    }
}

GameManager::~GameManager()
{
}

void GameManager::Process()
{
    mDropCount++;
    if(mDropCount % DROP_INTERVAL == 0)
    {
        mToBeDropped = true;
    }
}

bool GameManager::IsMovable(const Block &block, int horizontal, int vertical)
{
    if(mExists[ block.mPosition.y + vertical ][ block.mPosition.x + horizontal ] == false)
    {
        return true;
    }
    else
    {
        return false;
    }
    
    /*別の書き方*/
//    return !mExists[ block.mPosition.y + vertical ][ block.mPosition.x + horizontal ];
}

bool GameManager::IsMovable(const TetriMino &mino, int horizontal, int vertical)
{
    for(size_t i = 0; i < BLOCK_MAX; i++)
    {
        if(mExists[ mino.mBlocks[i]->mPosition.y + vertical ][ mino.mBlocks[i]->mPosition.x + horizontal ] == true)
        {
            return false;
        }
    }
    return true;
}

bool GameManager::IsRotatableLeft(const TetriMino &mino)
{
    for(size_t i = 0; i < BLOCK_MAX; i++)
    {
        auto temp = mino.mBlocks[i]->mOffset;
        if(mExists[ mino.mPosition.y + temp.x ][ mino.mPosition.x + (-temp.y) ] == true)
        {
            return false;
        }
    }
    return true;
}

bool GameManager::IsRotatableRight(const TetriMino &mino)
{
    for(size_t i = 0; i < BLOCK_MAX; i++)
    {
        auto temp = mino.mBlocks[i]->mOffset;
        if(mExists[ mino.mPosition.y + (-temp.x) ][ mino.mPosition.x + temp.y ] == true)
        {
            return false;
        }
    }
    return true;
}

void GameManager::PlaceCurrent(const Block& block)
{
    mExists[block.mPosition.y][block.mPosition.x] = true;
}

void GameManager::PlaceCurrent(const TetriMino& mino)
{
    for(size_t i = 0; i < BLOCK_MAX; i++)
    {
        if(mino.mBlocks[i]->mPosition.y >= FIELD_HEIGHT)
        {
            continue;
        }
        mExists[ mino.mBlocks[i]->mPosition.y ][ mino.mBlocks[i]->mPosition.x ] = true;
    }
}

void GameManager::DeleteLines()
{
    int targetLine = SENTINELS_COUNT;
    int deleteCount = 0;
    
    for(int i = SENTINELS_COUNT; i < FIELD_HEIGHT; i++)
    {
        MoveLine(i, targetLine);
        if(IsLineFilled(i) == false)
        {
            targetLine++;
        }
        else
        {
            deleteCount++;
        }
    }
    
    switch(deleteCount)
    {
        case 0:
            break;
        case 1:
            playerScore += 50;
            break;
        case 2:
            playerScore += 100;
            break;
        case 3:
            playerScore += 300;
            break;
        case 4:
            playerScore += 800;
            break;
        default:
            std::cout << "Unknown deleteCount.\n";
            break;
    }
}

void GameManager::MoveLine(int from, int to)
{
    for(size_t i = SENTINELS_COUNT; i < FIELD_WIDTH - SENTINELS_COUNT; i++)
    {
        mExists[to][i] = mExists[from][i];
    }
}

bool GameManager::IsGameOver()
{
    //エラー防止の為、[最上段 - 1]の中央4マスにブロックが置かれたらゲームオーバー
    for(size_t i = (FIELD_WIDTH / 2) - 1; i < (FIELD_WIDTH / 2) + 1; i++)
    {
        if(mExists[ FIELD_HEIGHT - SENTINELS_COUNT - 1 ][i] == true)
        {
            return true;
        }
    }
    return false;
}

bool GameManager::IsLineFilled(int y)
{
    for(size_t i = SENTINELS_COUNT; i < FIELD_WIDTH - SENTINELS_COUNT; i++)
    {
        if(mExists[y][i] == false)
        {
            return false;
        }
    }
    return true;
}
