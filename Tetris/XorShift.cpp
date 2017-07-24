#include "XorShift.hpp"
#include <random>

XorShift::XorShift()
{
    std::random_device rd;
    x = rd();
    y = 362436069;
    z = 521288629;
    w = 88675123;
}

XorShift::XorShift(uint32_t aX, uint32_t aY, uint32_t aZ, uint32_t aW)
: x(aX)
, y(aY)
, z(aZ)
, w(aW)
{
}

XorShift::~XorShift()
{
}

uint32_t XorShift::operator()()
{
    return Next();
}

//ブロックをランダムに選ぶ際に使われる乱数
uint32_t XorShift::operator()(uint32_t max)
{
    return Next(max);
}

uint32_t XorShift::operator()(uint32_t min, uint32_t max)
{
    return Next(min, max);
}

//ブロックをランダムに選ぶ際に使われる乱数
uint32_t XorShift::Next()
{
    uint32_t t;
    
    t = x ^ (x << 11);
    x = y;
    y = z;
    z = w;
    
    return w = (w ^ (w >> 19)) ^ (t ^ (t >> 8));
}

//ブロックをランダムに選ぶ際に使われる乱数 max:MINO_TYPE_MAX(7)
uint32_t XorShift::Next(uint32_t max)
{
    return Next() % max;
}

uint32_t XorShift::Next(uint32_t min, uint32_t max)
{
    return min + Next() % (max - min + 1);
}
