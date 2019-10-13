#include "gamemodel.h"
#include <time.h>
#include <stdlib.h>

GameModel::GameModel()
    : mRow(INIT_ROW),
      mCol(INIT_COL),
      totalMineNumber(INIT_MineCount),
      timerSeconds(INIT_Time),
      gameState(PLAYING)
{

}

GameModel::~GameModel()
{

}

void GameModel::createGame(int row, int col, int mineCount, GameLevel level)
{
    // 清空已有的游戏地图
    gameMap.clear();
    // 设置游戏变量
    mRow = row;
    mCol = col;
    totalMineNumber = mineCount;
    curMineNumber = mineCount;
    gameState = PLAYING;
    gameLevel = level;
    // 初始化地图
    for (int i = 0; i < mRow; i++)
    {
        std::vector<MineBlock> lineBlocks;   // 一行的雷
        for(int j = 0; j < mCol; j++)
        {
            MineBlock mineBlock;
            mineBlock.curState = UNDIG;      // 每个雷初始化都为未挖
            mineBlock.valueFlag = 0;         // 状态默认都是0
            lineBlocks.push_back(mineBlock); // 一行一行初始化方块
        }
        gameMap.push_back(lineBlocks);       // 一行初始化结束后在进行下一行
    }
    // 随机布雷
    srand((unsigned int)time(0));
    int k = totalMineNumber;
    while(k > 0)
    {
        // 埋雷并防止重叠
        int pRow = rand() % mRow;
        int pCol = rand() % mCol;
        if(gameMap[pRow][pCol].valueFlag != -1)
        {
             gameMap[pRow][pCol].valueFlag = -1;
             k--;
        }
    }
    // 计算雷周围的方块数字
    for(int i = 0; i < mRow; i++)
    {
        for(int j = 0; j < mCol; j++)
        {
            // 周围八个方块（排除自己，在地图范围内）的数字根据雷的数目叠加
            // y为行偏移量，x为列偏移量
            // 前提条件是本方块不是雷
            if(gameMap[i][j].valueFlag != -1)
            {
                // 以自己为中心的九宫格计算
                for(int y = -1; y <= 1; y++)
                {
                    for(int x = -1; x <= 1; x++)
                    {
                        if(i + y >= 0
                        && i + y < mRow
                        && j + x >= 0
                        && j + x < mCol
                        && gameMap[i + y][j + x].valueFlag == -1
                        && !(x == 0 && y == 0))
                        {
                            // 方块数字加1
                            gameMap[i][j].valueFlag++;
                        }
                    }
                }
            }
        }
    }
}

void GameModel::restartGame()
{
    createGame(mRow, mCol, totalMineNumber, gameLevel);
}

void GameModel::digMine(int m, int n)
{
    // 正常方块且标记未挖，则标记转为已挖
    if(gameMap[m][n].valueFlag > 0 && gameMap[m][n].curState == UNDIG)
    {
        gameMap[m][n].curState = DIGGED;
    }
    // 遇到空白块（数字0）就往四周递归挖雷,把连接的空白块全部翻开
    if(gameMap[m][n].valueFlag == 0 && gameMap[m][n].curState == UNDIG)
    {
        gameMap[m][n].curState = DIGGED;
        for(int y = -1; y <= 1; y++)
        {
            for(int x = -1; x <= 1; x++)
            {
                if(m + y >= 0
                && m + y < mRow
                && n + x >= 0
                && n + x < mCol
                && !(x == 0 && y == 0))
                {
                    digMine(m + y, n + x);
                }
            }
        }
    }
    // 踩雷
    if(gameMap[m][n].valueFlag == -1)
    {
        gameState = OVER;
        gameMap[m][n].curState = BOMB;
    }
    // 检查游戏输赢,并作调整
    checkGame();
}

void GameModel::markMine(int m, int n)
{
    // 如果标记错了，就记为错误标记，在ui层游戏结束时做绘制区分
    // 注意这里有个逻辑，如果一个方块标记两次会回到未挖掘的状态
    if(gameMap[m][n].curState == UNDIG)
    {
        if(gameMap[m][n].valueFlag == -1)
        {
            gameMap[m][n].curState = MARKED;
        }
        else
        {
            gameState = FAULT;
            gameMap[m][n].curState = WRONGMARK;
        }
        curMineNumber--; // 做了标记就默认雷减一
    }
    else if(gameMap[m][n].curState == MARKED || gameMap[m][n].curState == WRONGMARK)
    {
        gameMap[m][n].curState = UNDIG;
        gameState = PLAYING;
        curMineNumber++; // 雷数加回来
    }
    // 检查游戏输赢,并作调整
    checkGame();
}

void GameModel::checkGame()
{
    // 游戏结束，显示所有雷
    if(gameState == OVER)
    {
        // 输了就显示所有的雷以及标错的雷
        for(int i = 0; i < mRow; i++)
        {
            for(int j = 0; j < mCol; j++)
            {
                if(gameMap[i][j].valueFlag == -1)
                {
                    gameMap[i][j].curState = BOMB;
                }
            }
        }
        return;
    }
    // 如果雷排完了，且所有方块都挖出或者标记
    if(gameState != FAULT)
    {
        for(int i = 0; i < mRow; i++)
        {
            for(int j = 0; j < mCol; j++)
            {
                if(gameMap[i][j].curState == UNDIG)
                {
                    gameState = PLAYING;
                    return;
                }
            }
        }
        // 否则既没有错误标记游戏状态又不是输或者进行中，游戏就是赢了
        gameState = WIN;
    }
}
