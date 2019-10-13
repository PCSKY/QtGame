#ifndef GAMEMODEL_H
#define GAMEMODEL_H

#include <vector>

// 方块的装填
enum Blockstate
{
    UNDIG,      // 未挖
    DIGGED,     // 已挖
    MARKED,     // 标记
    BOMB,       // 地雷
    WRONGMARK   // 错误标记
};

// 地图方块类
struct MineBlock
{
    Blockstate curState;   // 当前状态
    int valueFlag;         // 0-8为数字，-1表示雷
};

// 游戏状态
enum GameState
{
    PLAYING,   // 游戏中
    FAULT,     // 出错
    OVER,      // 输
    WIN        // 赢
};

// 游戏难度
enum GameLevel
{
    BASIC,
    MEDIUM,
    HARD
};

// 游戏默认参数
const int INIT_ROW = 15;
const int INIT_COL = 20;
const int INIT_MineCount = 50;
const int INIT_Time = 0;

// 游戏逻辑模型类
class GameModel
{
public:
    GameModel();
    virtual ~GameModel();

public:
    void createGame(int row = INIT_ROW, int col = INIT_COL, int mineCount = INIT_MineCount, GameLevel level = MEDIUM); // 初始化游戏
    void restartGame();           // 按当前参数重新开始游戏
    void digMine(int m, int n);   //挖雷, m是行， n是列
    void markMine(int m, int n);  // 标记雷
    void checkGame();             // 检查游戏输赢

public:
    std::vector<std::vector<MineBlock>> gameMap;   // 游戏地图
    int mRow;   // 地图行数
    int mCol;   // 地图列数
    int totalMineNumber;   // 地雷总数
    int curMineNumber;     // 当前地雷数量
    int timerSeconds;      // 秒表计数器
    GameState gameState;   // 游戏状态
    GameLevel gameLevel;   // 游戏等级
};

#endif // GAMEMODEL_H
