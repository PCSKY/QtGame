#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QPaintEvent>
#include <QKeyEvent>
#include <QPushButton>

const int BLOCK_SIZE = 15;     // 单个方块单元的边长
const int MARGIN = 5;          // 场景边距
const int MAP_ROW = 25;        // 地图行数
const int MAP_COL = 25;        // 地图列数

const int TIME_INTERVAL = 500;       // 定时器间隔

enum Direction   // 方向
{
    UP,
    DOWN,
    LEFT,
    RIGHT
};

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = nullptr);
    ~Widget();
    virtual void paintEvent(QPaintEvent *event);   // 界面刷新
    virtual void keyPressEvent(QKeyEvent *event);  // 键盘监听

public:
    void GameOver();         // 游戏结束
    void GenerateFood();     // 随机产生果实
    bool IsGameOver();       // 判断是否游戏结束

private slots:
    void SnakeUpdate();      // 贪吃蛇增长
    void InitGame();         // 初始化游戏
    void PauseResumeGame();  // 暂停和恢复游戏

private:
    QTimer *gameTimer;       // 游戏计时器
    bool isPause;            // 暂停标志
    QPoint foodPoint;        // 果实坐标
    QList<QPoint> snake;     // 贪吃蛇(用列表存储，每个元素都是一个坐标)
    Direction dir;           // 方向
    int score;               // 分数
    QPushButton *start;      // 开始按钮
    QPushButton *pause;      // 结束按钮
    QPushButton *runAI;      // 运行AI
};

#endif // WIDGET_H
