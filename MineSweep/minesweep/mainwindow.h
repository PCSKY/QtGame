#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>

namespace Ui {
class MainWindow;
}

// 前置声明游戏逻辑模型类
class GameModel;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    virtual void paintEvent(QPaintEvent *event);       // 界面重绘
    virtual void mousePressEvent(QMouseEvent *event);  // 鼠标控制

private:
    Ui::MainWindow *ui;

    GameModel *game;   // 游戏
    QTimer *timer;     // 计时器
    QLabel *timeLabel; // 计时数字

    void handleGameState(GameModel *game); // 处理游戏状态

private slots:
    void onStartGameClicked();    // 开始游戏
    void onLevelChooseClicked();  // 选择游戏难度
    void onQuitClicked();         // 退出游戏
    void updateTimer();           // 计时
};

#endif // MAINWINDOW_H
