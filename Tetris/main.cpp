#include <iostream>
#include <glad/glad.h>
#include <glad/glad.c>
#include <GLFW/glfw3.h>
#include <linmath.h>
#include "Utility.hpp"
#include "Shader.hpp"
#include "Sprite.hpp"
#include "Define.hpp"
#include "Loader.hpp"
#include "XorShift.hpp"
#include "Input.hpp"
#include "Texts.hpp"
#include "Score.hpp"
#include "Block.hpp"
#include "GameManager.hpp"
#include "TetriMino.hpp"

Input input;
XorShift xorShift;
Shader shader;

GLFWwindow* window = nullptr;

std::unique_ptr<Texts> scoreText;
std::unique_ptr<Texts> nextText;
std::unique_ptr<Score> scoreDigit01;
std::unique_ptr<Score> scoreDigit02;
std::unique_ptr<Score> scoreDigit03;
std::unique_ptr<Score> scoreDigit04;
std::unique_ptr<Block> blockList[FIELD_HEIGHT][FIELD_WIDTH];
std::unique_ptr<GameManager> gameManager;
std::unique_ptr<TetriMino> currentMino;
std::unique_ptr<TetriMino> nextMino;

GLuint scoreTextId;
GLuint nextTextId;
GLuint scoreId;
GLuint blockId;

bool gameIsOver = false;

void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

int LibraryInit();
void SetUpBmp();
void ProcessInput();
void DropMino();
void Draw();
void ProcessGameover();



//----------------------------------------------------------------------------------------
int main(int argc, const char * argv[]) {
    
    if(LibraryInit() == -1)
    {
        exit(EXIT_FAILURE);
    }
    
    shader.SetUp();
    
    SetUpBmp();
    
    scoreText = std::make_unique<Texts>(Vec2f{ 0.15f, 0.3f }, SCORE_SIZE);
    nextText = std::make_unique<Texts>(Vec2f{ 0.3f, -0.1f }, NEXT_SIZE);
    scoreDigit01 = std::make_unique<Score>(Vec2f{ 0.54f, 0.3f }, NUM_SIZE);
    scoreDigit02 = std::make_unique<Score>(Vec2f{ 0.48f, 0.3f }, NUM_SIZE);
    scoreDigit03 = std::make_unique<Score>(Vec2f{ 0.42f, 0.3f }, NUM_SIZE);
    scoreDigit04 = std::make_unique<Score>(Vec2f{ 0.36f, 0.3f }, NUM_SIZE);
    
    for(int i = 0; i < FIELD_SIZE.y; i++)
    {
        for(int j = 0; j < FIELD_SIZE.x; j++)
        {
            blockList[i][j] = std::make_unique<Block>(j, i);
            blockList[i][j]->SetColor({ 0.722f, 0.525f, 0.043f, 1.0f });
        }
    }
    
    gameManager = std::make_unique<GameManager>();
    currentMino = std::make_unique<TetriMino>(Vec2i{ FIELD_WIDTH / 2, FIELD_HEIGHT });
    nextMino = std::make_unique<TetriMino>(Vec2i{ FIELD_WIDTH / 2, FIELD_HEIGHT });
    
    //ここで次に出るテトリミノのタイプと位置を設定しておく
    nextMino->SetType(currentMino->mNextType);
    nextMino->SetPosition(Vec2i{ NEXT_TETRIMINO_POS.x, NEXT_TETRIMINO_POS.y });
    
    while(glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
          glfwWindowShouldClose(window) == 0)
    {
        if(gameManager->IsGameOver() == true)
        {
            ProcessGameover();
        }
        else
        {
            gameManager->Process();
            ProcessInput();
            DropMino();
        }
        
        input.Update();
        input.ResetNow();
        
        Draw();
        
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    glfwDestroyWindow(window);
    glfwTerminate();
    
    return 0;
}
//----------------------------------------------------------------------------------------



void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if(action == GLFW_PRESS)
    {
        input.mKeyStates[key].pressed = true;
    }
    else if(action == GLFW_RELEASE)
    {
        input.mKeyStates[key].pressed = false;
    }
}

int LibraryInit()
{
    std::cout << "Current directory is " << GetCurrentWorkingDir().c_str() << ".\n";
    
    if(!glfwInit())
    {
        exit(EXIT_FAILURE);
    }
    
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    window = glfwCreateWindow(WINDOW_SIZE.x, WINDOW_SIZE.y, "Tetris", NULL, NULL);
    
    if(!window)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    
    glfwSetErrorCallback(ErrorCallback);
    glfwSetKeyCallback(window, KeyCallback);
    
    glfwMakeContextCurrent(window);
    auto addr = (GLADloadproc)glfwGetProcAddress;
    gladLoadGLLoader(addr);
    glfwSwapInterval(1);
    
    return 0;
}

void SetUpBmp()
{
    scoreTextId = LoadBmp("Score.bmp");
    nextTextId = LoadBmp("Next.bmp");
    scoreId = LoadBmp("Numbers.bmp");
    blockId = LoadBmp("BlockWhite.bmp");
}

void ProcessInput()
{
    if(input.GetButtonDown(GLFW_KEY_A))
    {
        if(gameManager->IsMovable(*currentMino, -1, 0) == true)
        {
            currentMino->Move({ -1, 0 });
        }
    }
    else if(input.GetButtonDown(GLFW_KEY_D))
    {
        if(gameManager->IsMovable(*currentMino, +1, 0) == true)
        {
            currentMino->Move({ +1, 0 });
        }
    }
    else if(input.GetButtonDown(GLFW_KEY_K))
    {
        //四角タイプのテトリミノ(mType=2)だけは回転を受け付けない
        if(currentMino->mType != 2)
        {
            if(gameManager->IsRotatableLeft(*currentMino))
            {
                currentMino->RotateLeft();
            }
        }
    }
    else if(input.GetButtonDown(GLFW_KEY_L))
    {
        if(currentMino->mType != 2)
        {
            if(gameManager->IsRotatableRight(*currentMino))
            {
                currentMino->RotateRight();
            }
        }
    }
}

void DropMino()
{
    if(gameManager->mToBeDropped == true)
    {
        if(gameManager->IsMovable(*currentMino, 0, -1) == true)
        {
            currentMino->Move({ 0, -1 });
        }
        else
        {
            gameManager->PlaceCurrent(*currentMino);
            
            for(size_t i = 0; i < BLOCK_MAX; i++)
            {
                auto pos = currentMino->mBlocks[i]->mPosition;
                blockList[pos.y][pos.x]->SetColor(currentMino->blockTypes[currentMino->mType].color);
            }
            
            gameManager->DeleteLines();
            
            //設置したら次のテトリミノに変更して出現ポイントに移動する
            currentMino->ResetAndNext(currentMino->GetNextType());
            currentMino->UpdateNextType();
            //ネクストテトリミノを更新する
            nextMino->SetType(currentMino->mNextType);
            nextMino->SetPosition(Vec2i{ NEXT_TETRIMINO_POS.x, NEXT_TETRIMINO_POS.y });
        }
        gameManager->mToBeDropped = false;
    }
}

void Draw()
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearDepth(1.0);
    
    scoreText->Draw(scoreTextId);
    nextText->Draw(nextTextId);
    scoreDigit01->Draw(scoreId);
    scoreDigit02->Draw(scoreId);
    scoreDigit03->Draw(scoreId);
    scoreDigit04->Draw(scoreId);
    
    for(size_t i = 0; i < FIELD_SIZE.y; i++)
    {
        for(size_t j = 0; j < FIELD_SIZE.x; j++)
        {
            if(gameManager->mExists[i][j] == true)
            {
                blockList[i][j]->Draw(blockId);
            }
        }
    }
    
    currentMino->Draw(blockId);
    nextMino->Draw(blockId);
}

void ProcessGameover()
{
    if(gameIsOver == false)
    {
        for(int i = 0; i < FIELD_SIZE.y; i++)
        {
            for(int j = 0; j < FIELD_SIZE.x; j++)
            {
                blockList[i][j]->SetColor({ 0.4f, 0.4f, 0.4f, 1.0f });
            }
        }
        
        std::cout << "Game is over !\n";
        gameIsOver = true;
    }
}
