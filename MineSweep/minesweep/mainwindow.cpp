#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "gamemodel.h"
#include <QDebug>
#include <QPainter>
#include <QMouseEvent>
#include <QTimer>
#include <QMessageBox>

// 全局设置
const int blockSize = 20;   // 方块大小
const int offsetX = 5;      // 横向边距
const int offsetY = 5;      // 纵向边距
const int spaceY = 70;      // 上方空白用于绘制记分板

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    // 创建计时数字标签
    timeLabel = new QLabel(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}
