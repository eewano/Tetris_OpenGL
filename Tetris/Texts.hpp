#ifndef Texts_hpp
#define Texts_hpp

#include "Sprite.hpp"

static constexpr int TEXT_VERTS_COUNT = 4;
static Vec2f NEXT_SIZE = { 0.6f, 0.2f };
static Vec2f SCORE_SIZE = { 0.6f, 0.2f };

class Texts : public Sprite<TEXT_VERTS_COUNT>
{
public:
    Texts(Vec2f aPos, Vec2f aSize);
    ~Texts();
};

#endif /* Texts_hpp */
