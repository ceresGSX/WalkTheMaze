#ifndef MAZEWINDOW_H
#define MAZEWINDOW_H

#include <QMainWindow>
#include "point.h"
#include <QTimer>

class MazeWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MazeWindow(QWidget *parent = nullptr);

    void paintEvent(QPaintEvent *);//绘图事件
    void keyPressEvent(QKeyEvent *);//重写keyPressEvent监听键盘事件

    //初始坐标位置
    int mouse_x_pos=6;
    int mouse_y_pos=6;
    int remain_time=30;//设置剩余闯关时间
    bool reachable=true;//能否达到
    bool auto_findway=false;//是否开启自动寻路
    int mp[13][13];
    point pre[13][13];
    bool success=false; //鼠鼠是否已达到目的地
    bool gameover=false;//游戏是否结束
    QTimer *timer1=NULL;

    void findway();

signals:
    void chooseBack();
};

#endif // MAZEWINDOW_H
