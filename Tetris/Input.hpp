#ifndef Input_hpp
#define Input_hpp

static constexpr int KEY_MAX = 512;

struct KeyState
{
    bool pressed;
    bool lastPressed;
};

class Input
{
public:
    void Update();
    void ResetNow();
    bool GetButtonDown(int key);
    
public:
    KeyState mKeyStates[KEY_MAX];
};

#endif /* Input_hpp */
