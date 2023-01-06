#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPainter>
#include "mypushbuttom.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setFixedSize(631,540);
    setWindowTitle("走迷宫");
    setWindowIcon(QIcon(":/image/mouse1.jpg"));

    //开始按钮
    MyPushButtom *btn_bt=new MyPushButtom(":/image/1.png");
    btn_bt->setParent(this);
    btn_bt->move(this->width()*0.5-btn_bt->width()*0.5,this->height()*0.55);

    //编辑按钮
    MyPushButtom *btn_edit=new MyPushButtom(":/image/2.png");
    btn_edit->setParent(this);
    btn_edit->move(this->width()*0.5-btn_bt->width()*0.5,this->height()*0.7);

    //退出按钮
    MyPushButtom *btn_en=new MyPushButtom(":/image/3.png");
    btn_en->setParent(this);
    btn_en->move(this->width()*0.5-btn_bt->width()*0.5,this->height()*0.85);

    //点击开始游戏进入游戏界面
    connect(btn_bt,&MyPushButtom::clicked,this,[=](){
        this->hide();
        mazewindow = new MazeWindow;
        mazewindow->show();
        //监听游戏界面中的返回信号
        connect(mazewindow,&MazeWindow::chooseBack,this,[=](){
            mazewindow->hide();
            delete mazewindow;
            mazewindow=NULL;
            this->show();
        });
    });

    //点击编辑按钮编辑地图
    connect(btn_edit,&MyPushButtom::clicked,this,[=](){
        this->hide();
        editmazewindow =new EditMazeWindow;
        editmazewindow->show();
        connect(editmazewindow,&EditMazeWindow::chooseBack,this,[=](){
            editmazewindow->hide();
            delete editmazewindow;
            editmazewindow=NULL;
            this->show();
        });
    });

    //点击退出按钮退出
    connect(btn_en,&MyPushButtom::clicked,this,[=](){
        this->close();
    });
}

void MainWindow::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    QPixmap pix;
    pix.load(":/image/mouse2.jpg");
    painter.drawPixmap(0,0,this->width(),this->height(),pix);
}

MainWindow::~MainWindow()
{
    delete ui;
}

