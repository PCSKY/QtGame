#include "widget.h"
#include <QPainter>
#include <time.h>
#include <QTimer>
#include <QString>
#include <QMessageBox>
#include <QVBoxLayout>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    // 初始化界面(左上角的坐标为0,0)
    resize(MARGIN*12+(MAP_COL+3)*BLOCK_SIZE, MARGIN*2+MAP_ROW*BLOCK_SIZE);
    // 初始化按钮
    start = new QPushButton(this);
    start->setText("start");
    start->setFocusPolicy(Qt::NoFocus);
    start->move(MARGIN*3+MAP_COL*BLOCK_SIZE,MARGIN+4*BLOCK_SIZE);
    connect(start, SIGNAL(clicked()), this, SLOT(InitGame()));
    pause = new QPushButton(this);
    pause->setText("pause");
    pause->setFocusPolicy(Qt::NoFocus);
    pause->move(MARGIN*3+MAP_COL*BLOCK_SIZE,MARGIN+8*BLOCK_SIZE);
    connect(pause, SIGNAL(clicked()), this, SLOT(PauseResumeGame()));
    runAI = new QPushButton(this);
    runAI->setText("AI");
    runAI->setFocusPolicy(Qt::NoFocus);
    runAI->move(MARGIN*3+MAP_COL*BLOCK_SIZE,MARGIN+12*BLOCK_SIZE);
}

Widget::~Widget()
{

}

// 更新界面
void Widget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    // 绘制游戏场景
    painter.setBrush(Qt::white);   // 白色背景
    painter.setPen(Qt::blue);      // 画笔
    painter.drawRect(MARGIN, MARGIN, MAP_COL*BLOCK_SIZE, MAP_ROW*BLOCK_SIZE);
    // 绘制贪吃蛇
    painter.setBrush(Qt::black);   // 黑色蛇身
    painter.setPen(Qt::white);
    for(int i = 0; i < snake.size(); i++)
    {
        painter.drawRect(MARGIN+snake[i].x()*BLOCK_SIZE, MARGIN+snake[i].y()*BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE);
    }
    // 绘制果实
    painter.setBrush(Qt::blue);
    painter.drawEllipse(MARGIN+foodPoint.x()*BLOCK_SIZE, MARGIN+foodPoint.y()*BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE);
    // 绘制分数
    //绘制游戏分数
    painter.setPen(Qt::black);
    painter.setFont(QFont("Arial",14));
    painter.drawText(MARGIN*3+MAP_COL*BLOCK_SIZE,MARGIN+2*BLOCK_SIZE,"score: "+QString::number(score));
}

// 判断按键
void Widget::keyPressEvent(QKeyEvent *event)
{
    //贪吃蛇的方向是个状态机,注意各个方向之间切换有限制,不能180度掉头
    switch(event->key())
    {
    case Qt::Key_Up:
        if(dir!=DOWN)
            dir=UP;
        break;
    case Qt::Key_Down:
        if(dir!=UP)
            dir=DOWN;
        break;
    case Qt::Key_Left:
        if(dir!=RIGHT)
            dir=LEFT;
        break;
    case Qt::Key_Right:
        if(dir!=LEFT)
            dir=RIGHT;
        break;
    case Qt::Key_P:
        PauseResumeGame();
        break;
    default:
        break;
    }
}

// 更新蛇(地图的左上角坐标为(0,0))
void Widget::SnakeUpdate()
{
    //贪吃蛇移动的策略是每次删除尾部，然后添加新的头部，维护一个动态链表
    switch (dir)
    {
    case UP:
        snake.push_front(QPoint(snake.front().x(), snake.front().y()-1));
        break;
    case DOWN:
        snake.push_front(QPoint(snake.front().x(), snake.front().y()+1));
        break;
    case LEFT:
        snake.push_front(QPoint(snake.front().x()-1, snake.front().y()));
        break;
    case RIGHT:
        snake.push_front(QPoint(snake.front().x()+1, snake.front().y()));
        break;
    default:
        break;
    }
    // 如果吃到果实，则不删除尾部，否则删除尾部更新头部
    if(snake.contains(foodPoint))
    {
        score += 1;
        GenerateFood();
    }
    else
    {
        snake.pop_back();
    }
    // 每次更新都判断游戏是否结束
    if(IsGameOver())
    {
        GameOver();
        return;   // 在重绘前结束函数
    }
    // 重绘(调用paintevent)
    update();
}

// 初始化游戏
void Widget::InitGame()
{
    // 初始化贪吃蛇,初始长度为4,头部在前
    if(snake.size() > 0) snake.clear();
    for(int i = 3; i >= 0; i--)
    {
        snake.push_back(QPoint(i, 0));
    }
    // 一开始往右走
    dir = RIGHT;
    // 初始化果实
    srand(time(0));
    GenerateFood();
    // 初始化游戏分数
    score = 0;
    // 初始化暂停标志
    isPause = false;
    // 初始化计时器
    gameTimer = new QTimer(this);
    connect(gameTimer, SIGNAL(timeout()), this, SLOT(SnakeUpdate())); // 链接槽函数
    gameTimer->start(TIME_INTERVAL);   // 初始化定时器间隔
}

// 游戏结束
void Widget::GameOver()
{
    gameTimer->stop();
    QMessageBox::information(this,"failed","game over!");
}

// 暂停与恢复
void Widget::PauseResumeGame()
{
    //暂停定时器
    if(!isPause)
    {
        isPause = !isPause;
        gameTimer->stop();
    }
    // 恢复定时器
    else
    {
        isPause = !isPause;
        gameTimer->start(TIME_INTERVAL);
    }
}

// 更新果实
void Widget::GenerateFood()
{
    // 随机产生位置
    foodPoint.setX(rand()%MAP_COL);
    foodPoint.setY(rand()%MAP_ROW);
    // 如果与蛇的身体重复，则重新生成
    if(snake.contains(foodPoint))
    {
        GenerateFood();
    }
}

// 是否游戏结束
bool Widget::IsGameOver()
{
    int x=snake.front().x();
    int y=snake.front().y();
    // 出界
    if(x<0 || x>MAP_COL-1 || y<0 || y>MAP_ROW-1)
        return true;
    // 吃自己(最小能吃到自己的第四格身体)
    for(int i = 3; i<snake.size(); i++)
        if(snake[i]==snake.front())
            return true;
    return false;
}
