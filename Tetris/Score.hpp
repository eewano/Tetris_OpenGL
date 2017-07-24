#ifndef Score_hpp
#define Score_hpp

#include "Sprite.hpp"

static constexpr int SCORE_VERTS_COUNT = 4;
static Vec2f NUM_SIZE = { 0.1f, 0.2f };

class Score : public Sprite<SCORE_VERTS_COUNT>
{
public:
    Score(Vec2f aPos, Vec2f aSize);
    ~Score();
    
public:
    void Update(int aNum);
    void RefreshUv(int index);
    
private:
    int mNum;
};

#endif /* Score_hpp */
